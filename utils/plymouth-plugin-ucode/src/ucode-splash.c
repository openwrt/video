#include <dlfcn.h>
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/inotify.h>
#include <time.h>
#include <unistd.h>

#include <libubox/blobmsg_json.h>
#include <libubus.h>

#include <plutovg/plutovg.h>

#include <ucode/compiler.h>
#include <ucode/lib.h>
#include <ucode/source.h>
#include <ucode/vm.h>

#include "ply-boot-splash-plugin.h"
#include "ply-event-loop.h"
#include "ply-key-file.h"
#include "ply-list.h"
#include "ply-logger.h"
#include "ply-pixel-buffer.h"
#include "ply-pixel-display.h"
#include "ply-region.h"
#include "ply-rectangle.h"
#include "ply-trigger.h"

#define DEFAULT_FRAME_RATE 30.0

typedef struct
{
	ply_pixel_display_t *display;
} view_t;

#define UBUS_SOCKET_DIR "/var/run/ubus"

struct _ply_boot_splash_plugin
{
	struct ubus_event_handler ubus_listener;
	ply_boot_splash_mode_t mode;
	struct ubus_context *ubus;
	ply_fd_watch_t *ubus_watch;
	ply_fd_watch_t *wait_watch;
	ply_event_loop_t *loop;
	ply_list_t *views;
	int wait_fd;
	uc_parse_config_t config;
	uc_program_t *program;
	uc_source_t *source;
	uc_vm_t vm;
	char *script_path;
	char *theme_dir;
	double frame_interval;
	double started_at;
	bool vm_ready;
	bool animating;
};

static ply_boot_splash_plugin_t *plugin_instance;


static double
now_seconds (void)
{
	struct timespec ts;

	clock_gettime (CLOCK_MONOTONIC, &ts);

	return ts.tv_sec + ts.tv_nsec / 1000000000.0;
}

static const char *
mode_name (ply_boot_splash_mode_t mode)
{
	switch (mode) {
	case PLY_BOOT_SPLASH_MODE_BOOT_UP:
		return "boot-up";
	case PLY_BOOT_SPLASH_MODE_SHUTDOWN:
		return "shutdown";
	case PLY_BOOT_SPLASH_MODE_REBOOT:
		return "reboot";
	case PLY_BOOT_SPLASH_MODE_UPDATES:
		return "updates";
	case PLY_BOOT_SPLASH_MODE_SYSTEM_UPGRADE:
		return "system-upgrade";
	case PLY_BOOT_SPLASH_MODE_FIRMWARE_UPGRADE:
		return "firmware-upgrade";
	case PLY_BOOT_SPLASH_MODE_SYSTEM_RESET:
		return "system-reset";
	default:
		return "invalid";
	}
}

static uc_value_t *
script_theme (ply_boot_splash_plugin_t *plugin)
{
	if (!plugin->vm_ready)
		return NULL;

	return uc_vm_registry_get (&plugin->vm, "plymouth.theme");
}

static void
script_call (ply_boot_splash_plugin_t *plugin, const char *name,
	     uc_value_t **args, size_t nargs)
{
	uc_value_t *theme;
	uc_value_t *fn;
	size_t i;

	theme = script_theme (plugin);
	fn = theme ? ucv_object_get (theme, name, NULL) : NULL;

	if (!ucv_is_callable (fn)) {
		for (i = 0; i < nargs; i++)
			ucv_put (args[i]);

		return;
	}

	uc_vm_stack_push (&plugin->vm, ucv_get (theme));
	uc_vm_stack_push (&plugin->vm, ucv_get (fn));

	for (i = 0; i < nargs; i++)
		uc_vm_stack_push (&plugin->vm, args[i]);

	if (uc_vm_call (&plugin->vm, true, nargs) == EXCEPTION_NONE)
		ucv_put (uc_vm_stack_pop (&plugin->vm));
	else
		ply_error ("ucode splash: %s() raised an exception", name);
}

static void
event_emit (ply_boot_splash_plugin_t *plugin, const char *name,
	    uc_value_t *detail)
{
	uc_value_t *args[2];

	args[0] = ucv_string_new (name);
	args[1] = detail;

	script_call (plugin, "event", args, 2);
}

static uc_value_t *
uc_plymouth_damage (uc_vm_t *vm, size_t nargs)
{
	ply_boot_splash_plugin_t *plugin = plugin_instance;
	ply_list_node_t *node;
	int x, y, w, h;
	view_t *view;

	if (!plugin)
		return NULL;

	x = ucv_to_integer (uc_fn_arg (0));
	y = ucv_to_integer (uc_fn_arg (1));
	w = ucv_to_integer (uc_fn_arg (2));
	h = ucv_to_integer (uc_fn_arg (3));

	if (w <= 0 || h <= 0)
		return ucv_boolean_new (false);

	node = ply_list_get_first_node (plugin->views);

	while (node != NULL) {
		view = ply_list_node_get_data (node);
		ply_pixel_display_draw_area (view->display, x, y, w, h);
		node = ply_list_get_next_node (plugin->views, node);
	}

	return ucv_boolean_new (true);
}

static uc_value_t *
uc_plymouth_screens (uc_vm_t *vm, size_t nargs)
{
	ply_boot_splash_plugin_t *plugin = plugin_instance;
	ply_list_node_t *node;
	uc_value_t *entry;
	uc_value_t *list;
	view_t *view;

	if (!plugin)
		return NULL;

	list = ucv_array_new (vm);
	node = ply_list_get_first_node (plugin->views);

	while (node != NULL) {
		view = ply_list_node_get_data (node);
		entry = ucv_object_new (vm);

		ucv_object_add (entry, "width",
				ucv_int64_new (ply_pixel_display_get_width (view->display)));
		ucv_object_add (entry, "height",
				ucv_int64_new (ply_pixel_display_get_height (view->display)));
		ucv_object_add (entry, "scale",
				ucv_int64_new (ply_pixel_display_get_device_scale (view->display)));

		ucv_array_push (list, entry);
		node = ply_list_get_next_node (plugin->views, node);
	}

	return list;
}

static uc_value_t *
uc_plymouth_mode (uc_vm_t *vm, size_t nargs)
{
	if (!plugin_instance)
		return NULL;

	return ucv_string_new (mode_name (plugin_instance->mode));
}

static uc_value_t *
uc_plymouth_theme_dir (uc_vm_t *vm, size_t nargs)
{
	if (!plugin_instance || !plugin_instance->theme_dir)
		return NULL;

	return ucv_string_new (plugin_instance->theme_dir);
}

static uc_value_t *
uc_plymouth_elapsed (uc_vm_t *vm, size_t nargs)
{
	if (!plugin_instance)
		return NULL;

	return ucv_double_new (now_seconds () - plugin_instance->started_at);
}

static uc_value_t *
uc_plymouth_log (uc_vm_t *vm, size_t nargs)
{
	uc_value_t *msg = uc_fn_arg (0);
	char *str;

	str = ucv_to_string (vm, msg);

	if (str) {
		ply_trace ("ucode splash: %s", str);
		free (str);
	}

	return NULL;
}

static const uc_function_list_t plymouth_functions[] = {
	{ "damage", uc_plymouth_damage },
	{ "screens", uc_plymouth_screens },
	{ "mode", uc_plymouth_mode },
	{ "theme_dir", uc_plymouth_theme_dir },
	{ "elapsed", uc_plymouth_elapsed },
	{ "log", uc_plymouth_log }
};

static void
on_exception (uc_vm_t *vm, uc_exception_t *ex)
{
	ply_error ("ucode splash: %s", ex->message ? ex->message : "exception");
}

static void
on_ubus_event (struct ubus_context *ctx, struct ubus_event_handler *listener,
	       const char *type, struct blob_attr *msg)
{
	ply_boot_splash_plugin_t *plugin = plugin_instance;
	uc_value_t *detail;
	char *json;

	if (!plugin || !plugin->vm_ready)
		return;

	json = msg ? blobmsg_format_json (msg, true) : NULL;

	detail = ucv_object_new (&plugin->vm);
	ucv_object_add (detail, "type", ucv_string_new (type ? type : ""));
	ucv_object_add (detail, "data", ucv_string_new (json ? json : "{}"));

	free (json);

	event_emit (plugin, "ubus", detail);
}

static void
on_ubus_object (struct ubus_context *ctx, struct ubus_object_data *obj,
		void *priv)
{
	ply_boot_splash_plugin_t *plugin = priv;
	uc_value_t *detail;

	if (!plugin->vm_ready || !obj->path)
		return;

	detail = ucv_object_new (&plugin->vm);
	ucv_object_add (detail, "type", ucv_string_new ("ubus.object.add"));
	ucv_object_add (detail, "data",
			ucv_string_new_length (obj->path, strlen (obj->path)));
	ucv_object_add (detail, "path", ucv_string_new (obj->path));
	ucv_object_add (detail, "present", ucv_boolean_new (true));

	event_emit (plugin, "ubus", detail);
}

static void
on_ubus_readable (void *user_data, int fd)
{
	ply_boot_splash_plugin_t *plugin = user_data;

	if (plugin->ubus)
		ubus_handle_event (plugin->ubus);
}

static bool
ubus_setup (ply_boot_splash_plugin_t *plugin)
{
	plugin->ubus = ubus_connect (NULL);

	if (!plugin->ubus)
		return false;

	plugin->ubus_listener.cb = on_ubus_event;
	ubus_register_event_handler (plugin->ubus, &plugin->ubus_listener, "*");

	plugin->ubus_watch = ply_event_loop_watch_fd (plugin->loop,
						      plugin->ubus->sock.fd,
						      PLY_EVENT_LOOP_FD_STATUS_HAS_DATA,
						      on_ubus_readable, NULL,
						      plugin);

	event_emit (plugin, "ubus-connected", NULL);

	/* objects that registered before the splash connected are state,
	 * not events, and would otherwise never be seen */
	ubus_lookup (plugin->ubus, NULL, on_ubus_object, plugin);

	return true;
}

static void
wait_stop (ply_boot_splash_plugin_t *plugin)
{
	if (plugin->wait_watch) {
		ply_event_loop_stop_watching_fd (plugin->loop,
						 plugin->wait_watch);
		plugin->wait_watch = NULL;
	}

	if (plugin->wait_fd >= 0) {
		close (plugin->wait_fd);
		plugin->wait_fd = -1;
	}
}

/* ubusd creates the socket directory itself, well after the splash starts.
 */
static bool
wait_arm (ply_boot_splash_plugin_t *plugin)
{
	char path[PATH_MAX];
	char *slash;

	strncpy (path, UBUS_SOCKET_DIR, sizeof(path) - 1);
	path[sizeof(path) - 1] = '\0';

	while (inotify_add_watch (plugin->wait_fd, path, IN_CREATE) < 0) {
		slash = strrchr (path, '/');

		if (slash == NULL || slash == path)
			return false;

		*slash = '\0';
	}

	return true;
}

static void
on_ubus_appeared (void *user_data, int fd)
{
	ply_boot_splash_plugin_t *plugin = user_data;
	char buf[4096];

	while (read (fd, buf, sizeof(buf)) > 0)
		;

	wait_arm (plugin);

	if (ubus_setup (plugin))
		wait_stop (plugin);
}

static void
ubus_wait (ply_boot_splash_plugin_t *plugin)
{
	plugin->wait_fd = inotify_init1 (IN_NONBLOCK | IN_CLOEXEC);

	if (plugin->wait_fd < 0)
		return;

	if (!wait_arm (plugin)) {
		close (plugin->wait_fd);
		plugin->wait_fd = -1;
		return;
	}

	plugin->wait_watch = ply_event_loop_watch_fd (plugin->loop,
						      plugin->wait_fd,
						      PLY_EVENT_LOOP_FD_STATUS_HAS_DATA,
						      on_ubus_appeared, NULL,
						      plugin);
}

static void
ubus_teardown (ply_boot_splash_plugin_t *plugin)
{
	wait_stop (plugin);

	if (plugin->ubus_watch) {
		ply_event_loop_stop_watching_fd (plugin->loop,
						 plugin->ubus_watch);
		plugin->ubus_watch = NULL;
	}

	if (plugin->ubus) {
		ubus_free (plugin->ubus);
		plugin->ubus = NULL;
	}
}

static void
vm_teardown (ply_boot_splash_plugin_t *plugin)
{
	if (!plugin->vm_ready)
		return;

	uc_vm_free (&plugin->vm);
	plugin->vm_ready = false;

	if (plugin->program) {
		uc_program_put (plugin->program);
		plugin->program = NULL;
	}

	if (plugin->source) {
		uc_source_put (plugin->source);
		plugin->source = NULL;
	}

	uc_search_path_free (&plugin->config.module_search_path);
}

static void
ucode_symbols_publish (void)
{
	static void *handle;

	if (handle)
		return;

	/* plymouth dlopens plugins RTLD_LOCAL, so libucode arrives in a
	 * private scope and the modules the VM loads cannot relocate
	 * against it */
	handle = dlopen (UCODE_SONAME, RTLD_NOW | RTLD_GLOBAL);

	if (!handle)
		ply_error ("ucode splash: %s: %s", UCODE_SONAME, dlerror ());
}

static bool
vm_setup (ply_boot_splash_plugin_t *plugin)
{
	uc_value_t *theme = NULL;
	uc_value_t *scope;
	char *err = NULL;
	uc_value_t *mod;

	ucode_symbols_publish ();

	plugin->config = uc_default_parse_config;
	/* the default is template mode, which would print the theme */
	plugin->config.raw_mode = true;
	/* plymouthd owns the process signal disposition */
	plugin->config.setup_signal_handlers = false;

	/* the copied vector points at static storage and
	 * uc_search_path_init() appends rather than resets, so pushing
	 * onto it would realloc() memory that was never allocated */
	plugin->config.module_search_path.count = 0;
	plugin->config.module_search_path.entries = NULL;

	uc_search_path_init (&plugin->config.module_search_path);
	uc_search_path_add (&plugin->config.module_search_path,
			    "/usr/lib/ucode/*.so");
	uc_search_path_add (&plugin->config.module_search_path,
			    "/usr/share/ucode/*.uc");

	plugin->source = uc_source_new_file (plugin->script_path);

	if (!plugin->source) {
		ply_error ("ucode splash: cannot read %s", plugin->script_path);
		uc_search_path_free (&plugin->config.module_search_path);
		return false;
	}

	plugin->program = uc_compile (&plugin->config, plugin->source, &err);

	if (!plugin->program) {
		ply_error ("ucode splash: %s", err ? err : "compile failed");
		free (err);
		uc_source_put (plugin->source);
		plugin->source = NULL;
		uc_search_path_free (&plugin->config.module_search_path);
		return false;
	}

	uc_vm_init (&plugin->vm, &plugin->config);
	uc_vm_exception_handler_set (&plugin->vm, on_exception);

	scope = uc_vm_scope_get (&plugin->vm);
	uc_stdlib_load (scope);

	mod = ucv_object_new (&plugin->vm);
	uc_function_list_register (mod, plymouth_functions);
	ucv_object_add (scope, "plymouth", mod);

	plugin->vm_ready = true;

	if (uc_vm_execute (&plugin->vm, plugin->program, &theme) != STATUS_OK) {
		ply_error ("ucode splash: %s failed to run",
			   plugin->script_path);
		ucv_put (theme);
		vm_teardown (plugin);
		return false;
	}

	if (ucv_type (theme) != UC_OBJECT) {
		ply_error ("ucode splash: %s returned no theme object",
			   plugin->script_path);
		ucv_put (theme);
		vm_teardown (plugin);
		return false;
	}

	uc_vm_registry_set (&plugin->vm, "plymouth.theme", theme);

	return true;
}

static void
on_draw (void *user_data, ply_pixel_buffer_t *pixel_buffer, int x, int y,
	 int width, int height, ply_pixel_display_t *display)
{
	ply_boot_splash_plugin_t *plugin = user_data;
	plutovg_surface_t *surface;
	plutovg_canvas_t *canvas;
	ply_rectangle_t painted;
	ply_rectangle_t size;
	uc_value_t *args[5];
	uint32_t *data;

	if (!plugin->vm_ready)
		return;

	data = ply_pixel_buffer_get_argb32_data (pixel_buffer);

	if (!data)
		return;

	ply_pixel_buffer_get_size (pixel_buffer, &size);

	surface = plutovg_surface_create_for_data ((unsigned char *)data,
						   size.width, size.height,
						   size.width * 4);

	if (!surface)
		return;

	canvas = plutovg_canvas_create (surface);
	plutovg_surface_destroy (surface);

	if (!canvas)
		return;

	plutovg_canvas_rect (canvas, x, y, width, height);
	plutovg_canvas_clip (canvas);

	args[0] = ucv_resource_create (&plugin->vm, "plutovg.canvas", canvas);

	if (!args[0]) {
		plutovg_canvas_destroy (canvas);
		return;
	}

	args[1] = ucv_int64_new (x);
	args[2] = ucv_int64_new (y);
	args[3] = ucv_int64_new (width);
	args[4] = ucv_int64_new (height);

	script_call (plugin, "paint", args, 5);

	/* drawing through the raw ARGB pointer bypasses the helpers that
	 * record what changed, so plymouth would upload nothing */
	painted.x = x;
	painted.y = y;
	painted.width = width;
	painted.height = height;
	ply_region_add_rectangle (ply_pixel_buffer_get_updated_areas (pixel_buffer),
				  &painted);
}

static void
on_frame (void *user_data, ply_event_loop_t *loop)
{
	ply_boot_splash_plugin_t *plugin = user_data;
	uc_value_t *args[1];

	if (!plugin->animating)
		return;

	args[0] = ucv_double_new (now_seconds () - plugin->started_at);
	script_call (plugin, "frame", args, 1);

	ply_event_loop_watch_for_timeout (plugin->loop, plugin->frame_interval,
					  on_frame, plugin);
}

static ply_boot_splash_plugin_t *
create_plugin (ply_key_file_t *key_file)
{
	ply_boot_splash_plugin_t *plugin;
	double fps;
	char *rate;
	char *val;

	plugin = calloc (1, sizeof(*plugin));

	if (!plugin)
		return NULL;

	plugin->views = ply_list_new ();
	plugin->wait_fd = -1;
	plugin->frame_interval = 1.0 / DEFAULT_FRAME_RATE;

	val = ply_key_file_get_value (key_file, "ucode", "ScriptFile");
	plugin->script_path = val;

	val = ply_key_file_get_value (key_file, "ucode", "ThemeDir");
	plugin->theme_dir = val;

	rate = ply_key_file_get_value (key_file, "ucode", "FrameRate");

	if (rate) {
		fps = atof (rate);

		if (fps > 0.0)
			plugin->frame_interval = 1.0 / fps;

		free (rate);
	}

	plugin_instance = plugin;

	return plugin;
}

static void
destroy_plugin (ply_boot_splash_plugin_t *plugin)
{
	ply_list_node_t *node;
	view_t *view;

	if (!plugin)
		return;

	vm_teardown (plugin);

	node = ply_list_get_first_node (plugin->views);

	while (node != NULL) {
		view = ply_list_node_get_data (node);
		free (view);
		node = ply_list_get_next_node (plugin->views, node);
	}

	ply_list_free (plugin->views);

	free (plugin->script_path);
	free (plugin->theme_dir);
	free (plugin);

	plugin_instance = NULL;
}

static void
add_pixel_display (ply_boot_splash_plugin_t *plugin,
		   ply_pixel_display_t *display)
{
	view_t *view;

	view = calloc (1, sizeof(*view));

	if (!view)
		return;

	view->display = display;
	ply_list_append_data (plugin->views, view);

	if (plugin->vm_ready)
		ply_pixel_display_set_draw_handler (display, on_draw, plugin);
}

static void
remove_pixel_display (ply_boot_splash_plugin_t *plugin,
		      ply_pixel_display_t *display)
{
	ply_list_node_t *node;
	view_t *view;

	node = ply_list_get_first_node (plugin->views);

	while (node != NULL) {
		view = ply_list_node_get_data (node);

		if (view->display == display) {
			ply_pixel_display_set_draw_handler (display, NULL,
							    NULL);
			ply_list_remove_node (plugin->views, node);
			free (view);
			return;
		}

		node = ply_list_get_next_node (plugin->views, node);
	}
}

static void
add_text_display (ply_boot_splash_plugin_t *plugin,
		  ply_text_display_t *display)
{
}

static void
remove_text_display (ply_boot_splash_plugin_t *plugin,
		     ply_text_display_t *display)
{
}

static bool
show_splash_screen (ply_boot_splash_plugin_t *plugin, ply_event_loop_t *loop,
		    ply_buffer_t *boot_buffer, ply_boot_splash_mode_t mode)
{
	ply_list_node_t *node;
	uc_value_t *args[1];
	view_t *view;

	plugin->loop = loop;
	plugin->mode = mode;
	plugin->started_at = now_seconds ();

	if (!plugin->script_path) {
		ply_error ("ucode splash: theme sets no ScriptFile");
		return false;
	}


	if (!vm_setup (plugin))
		return false;

	node = ply_list_get_first_node (plugin->views);

	while (node != NULL) {
		view = ply_list_node_get_data (node);
		ply_pixel_display_set_draw_handler (view->display, on_draw,
						    plugin);
		node = ply_list_get_next_node (plugin->views, node);
	}

	args[0] = ucv_string_new (mode_name (mode));
	script_call (plugin, "setup", args, 1);

	plugin->animating = true;
	ply_event_loop_watch_for_timeout (plugin->loop, plugin->frame_interval,
					  on_frame, plugin);

	if (!ubus_setup (plugin))
		ubus_wait (plugin);

	return true;
}

static void
update_status (ply_boot_splash_plugin_t *plugin, const char *status)
{
	event_emit (plugin, "status", ucv_string_new (status ? status : ""));
}

static void
on_boot_output (ply_boot_splash_plugin_t *plugin, const char *output,
		size_t size)
{
	event_emit (plugin, "output", ucv_string_new_length (output, size));
}

static void
on_boot_progress (ply_boot_splash_plugin_t *plugin, double duration,
		  double fraction_done)
{
	uc_value_t *detail;

	detail = ucv_object_new (&plugin->vm);
	ucv_object_add (detail, "duration", ucv_double_new (duration));
	ucv_object_add (detail, "fraction", ucv_double_new (fraction_done));

	event_emit (plugin, "progress", detail);
}

static void
on_root_mounted (ply_boot_splash_plugin_t *plugin)
{
	event_emit (plugin, "root-mounted", NULL);
}

static void
system_update (ply_boot_splash_plugin_t *plugin, int progress)
{
	event_emit (plugin, "system-update", ucv_int64_new (progress));
}

static void
display_message (ply_boot_splash_plugin_t *plugin, const char *message)
{
	event_emit (plugin, "message", ucv_string_new (message ? message : ""));
}

static void
hide_message (ply_boot_splash_plugin_t *plugin, const char *message)
{
	event_emit (plugin, "hide-message",
		    ucv_string_new (message ? message : ""));
}

static void
display_normal (ply_boot_splash_plugin_t *plugin)
{
	event_emit (plugin, "normal", NULL);
}

static void
display_password (ply_boot_splash_plugin_t *plugin, const char *prompt,
		  int bullets)
{
	event_emit (plugin, "password", ucv_int64_new (bullets));
}

static void
display_question (ply_boot_splash_plugin_t *plugin, const char *prompt,
		  const char *entry_text)
{
	event_emit (plugin, "question", ucv_string_new (prompt ? prompt : ""));
}

static void
become_idle (ply_boot_splash_plugin_t *plugin, ply_trigger_t *idle_trigger)
{
	event_emit (plugin, "idle", NULL);

	ply_trigger_pull (idle_trigger, NULL);
}

static void
hide_splash_screen (ply_boot_splash_plugin_t *plugin, ply_event_loop_t *loop)
{
	ply_list_node_t *node;
	view_t *view;

	plugin->animating = false;

	if (plugin->loop)
		ply_event_loop_stop_watching_for_timeout (plugin->loop,
							  on_frame, plugin);

	node = ply_list_get_first_node (plugin->views);

	while (node != NULL) {
		view = ply_list_node_get_data (node);
		ply_pixel_display_set_draw_handler (view->display, NULL, NULL);
		node = ply_list_get_next_node (plugin->views, node);
	}

	event_emit (plugin, "hide", NULL);
	ubus_teardown (plugin);
	vm_teardown (plugin);

	plugin->loop = NULL;
}

ply_boot_splash_plugin_interface_t *
ply_boot_splash_plugin_get_interface (void)
{
	static ply_boot_splash_plugin_interface_t plugin_interface = {
		.create_plugin = create_plugin,
		.destroy_plugin = destroy_plugin,
		.add_pixel_display = add_pixel_display,
		.remove_pixel_display = remove_pixel_display,
		.add_text_display = add_text_display,
		.remove_text_display = remove_text_display,
		.show_splash_screen = show_splash_screen,
		.system_update = system_update,
		.update_status = update_status,
		.on_boot_output = on_boot_output,
		.on_boot_progress = on_boot_progress,
		.on_root_mounted = on_root_mounted,
		.hide_splash_screen = hide_splash_screen,
		.display_message = display_message,
		.hide_message = hide_message,
		.display_normal = display_normal,
		.display_password = display_password,
		.display_question = display_question,
		.become_idle = become_idle
	};

	return &plugin_interface;
}
