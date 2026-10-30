/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Compile a WebKit content blocker JSON file into a content filter store,
 * or check that a compiled list in the store loads with this WebKit, which
 * recompiles a list of an older format from the source it embeds.
 *
 * usage: webkit-contentfilter-compile STORE IDENTIFIER [JSON]
 */

#include <signal.h>
#include <stdio.h>
#include <sys/prctl.h>
#include <sys/stat.h>

#include <wpe/webkit.h>

struct compile_ctx {
	GMainLoop *loop;
	int status;
};

static gboolean identifier_valid(const char *id)
{
	const char *p;

	if (!*id)
		return FALSE;

	for (p = id; *p; p++)
		if (!g_ascii_isalnum(*p) && *p != '_')
			return FALSE;

	return TRUE;
}

static void compile_done(GObject *source, GAsyncResult *res, gpointer data)
{
	WebKitUserContentFilterStore *store = WEBKIT_USER_CONTENT_FILTER_STORE(source);
	WebKitUserContentFilter *filter;
	struct compile_ctx *ctx = data;
	GError *error = NULL;

	filter = webkit_user_content_filter_store_save_from_file_finish(store, res,
									 &error);
	if (!filter) {
		fprintf(stderr, "compiling failed: %s\n", error->message);
		g_error_free(error);
		ctx->status = 1;
	} else {
		webkit_user_content_filter_unref(filter);
		ctx->status = 0;
	}

	g_main_loop_quit(ctx->loop);
}

static void load_done(GObject *source, GAsyncResult *res, gpointer data)
{
	WebKitUserContentFilterStore *store = WEBKIT_USER_CONTENT_FILTER_STORE(source);
	WebKitUserContentFilter *filter;
	struct compile_ctx *ctx = data;
	GError *error = NULL;

	filter = webkit_user_content_filter_store_load_finish(store, res, &error);
	if (!filter) {
		g_error_free(error);
		ctx->status = 1;
	} else {
		webkit_user_content_filter_unref(filter);
		ctx->status = 0;
	}

	g_main_loop_quit(ctx->loop);
}

int main(int argc, char **argv)
{
	struct compile_ctx ctx = { NULL, 1 };
	WebKitUserContentFilterStore *store;
	GFile *source;
	char *path;

	if (argc < 3 || argc > 4 || !identifier_valid(argv[2])) {
		fprintf(stderr, "usage: %s STORE IDENTIFIER [JSON]\n", argv[0]);
		return 2;
	}

	/* a restarted service compiles the list again */
	if (prctl(PR_SET_PDEATHSIG, SIGTERM))
		perror("prctl");

	ctx.loop = g_main_loop_new(NULL, FALSE);
	store = webkit_user_content_filter_store_new(argv[1]);

	if (argc == 3) {
		webkit_user_content_filter_store_load(store, argv[2], NULL,
						      load_done, &ctx);
	} else {
		source = g_file_new_for_path(argv[3]);
		webkit_user_content_filter_store_save_from_file(store, argv[2],
								source, NULL,
								compile_done,
								&ctx);
		g_object_unref(source);
	}
	g_main_loop_run(ctx.loop);

	if (ctx.status)
		goto out;

	/* WebKit creates the list 0600; web views run as other users */
	path = g_strdup_printf("%s/ContentRuleList-%s", argv[1], argv[2]);
	if (chmod(path, 0644)) {
		perror(path);
		ctx.status = 1;
	}
	g_free(path);

out:
	g_object_unref(store);
	g_main_loop_unref(ctx.loop);

	return ctx.status;
}
