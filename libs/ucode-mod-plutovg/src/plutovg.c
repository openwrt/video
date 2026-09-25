/*
 * Copyright (C) 2026 Daniel Golle <daniel@makrotopia.org>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <stdlib.h>
#include <string.h>

#include "uc-plutovg.h"

static const char * const uc_pvg_spread_names[] = {
	"pad", "reflect", "repeat", NULL
};

static const char * const uc_pvg_texture_names[] = {
	"plain", "tiled", NULL
};

static const char * const uc_pvg_fill_rule_names[] = {
	"nonzero", "evenodd", NULL
};

static const char * const uc_pvg_operator_names[] = {
	"clear", "src", "dst", "src-over", "dst-over", "src-in", "dst-in",
	"src-out", "dst-out", "src-atop", "dst-atop", "xor", NULL
};

static const char * const uc_pvg_line_cap_names[] = {
	"butt", "round", "square", NULL
};

static const char * const uc_pvg_line_join_names[] = {
	"miter", "round", "bevel", NULL
};

static const char * const uc_pvg_command_names[] = {
	"move_to", "line_to", "cubic_to", "close", NULL
};

static bool
uc_pvg_args_num(uc_vm_t *vm, size_t nargs, size_t first, size_t count,
                float *out)
{
	uc_value_t *arg;
	size_t i;

	for (i = 0; i < count; i++) {
		arg = _uc_fn_arg(vm, nargs, first + i);

		if (ucv_type(arg) != UC_INTEGER && ucv_type(arg) != UC_DOUBLE) {
			uc_vm_raise_exception(vm, EXCEPTION_TYPE,
				"Argument %zu is not a number", first + i + 1);

			return false;
		}

		out[i] = (float)ucv_to_double(arg);
	}

	return true;
}

static bool
uc_pvg_enum_get(uc_vm_t *vm, uc_value_t *uv, const char * const *names,
                const char *what, int *out)
{
	const char *str;
	int i;

	if (ucv_type(uv) != UC_STRING) {
		uc_vm_raise_exception(vm, EXCEPTION_TYPE,
			"Expecting %s name string", what);

		return false;
	}

	str = ucv_string_get(uv);

	for (i = 0; names[i]; i++) {
		if (strcmp(names[i], str))
			continue;

		*out = i;

		return true;
	}

	uc_vm_raise_exception(vm, EXCEPTION_TYPE, "Unknown %s '%s'", what, str);

	return false;
}

static bool
uc_pvg_enum_opt(uc_vm_t *vm, uc_value_t *uv, const char * const *names,
                const char *what, int *out)
{
	if (uv == NULL || ucv_type(uv) == UC_NULL)
		return true;

	return uc_pvg_enum_get(vm, uv, names, what, out);
}

static bool
uc_pvg_matrix_opt(uc_vm_t *vm, uc_value_t *uv, plutovg_matrix_t *storage,
                  const plutovg_matrix_t **out)
{
	*out = NULL;

	if (uv == NULL || ucv_type(uv) == UC_NULL)
		return true;

	if (!uc_pvg_matrix_get(uv, storage)) {
		uc_vm_raise_exception(vm, EXCEPTION_TYPE,
			"Invalid matrix argument");

		return false;
	}

	*out = storage;

	return true;
}

static bool
uc_pvg_matrix_arg(uc_vm_t *vm, uc_value_t *uv, plutovg_matrix_t *matrix)
{
	if (uc_pvg_matrix_get(uv, matrix))
		return true;

	uc_vm_raise_exception(vm, EXCEPTION_TYPE, "Invalid matrix argument");

	return false;
}

static plutovg_gradient_stop_t *
uc_pvg_stops_get(uc_vm_t *vm, uc_value_t *uv, int *nstops)
{
	plutovg_gradient_stop_t *stops;
	uc_value_t *entry;
	size_t count, i;

	*nstops = 0;

	if (ucv_type(uv) != UC_ARRAY) {
		uc_vm_raise_exception(vm, EXCEPTION_TYPE,
			"Expecting array of gradient stops");

		return NULL;
	}

	count = ucv_array_length(uv);

	if (count == 0) {
		uc_vm_raise_exception(vm, EXCEPTION_TYPE,
			"Gradient stop array is empty");

		return NULL;
	}

	stops = calloc(count, sizeof(*stops));

	if (!stops)
		return NULL;

	for (i = 0; i < count; i++) {
		entry = ucv_array_get(uv, i);

		if (!uc_pvg_component(entry, "offset", 0, &stops[i].offset) ||
		    !uc_pvg_color_get(ucv_type(entry) == UC_OBJECT
		                      ? ucv_object_get(entry, "color", NULL)
		                      : ucv_array_get(entry, 1),
		                      &stops[i].color)) {
			uc_vm_raise_exception(vm, EXCEPTION_TYPE,
				"Invalid gradient stop at index %zu", i);
			free(stops);

			return NULL;
		}
	}

	*nstops = (int)count;

	return stops;
}

static float *
uc_pvg_dashes_get(uc_vm_t *vm, uc_value_t *uv, int *ndashes)
{
	uc_value_t *entry;
	size_t count, i;
	float *dashes;

	*ndashes = 0;

	if (uv == NULL || ucv_type(uv) == UC_NULL)
		return NULL;

	if (ucv_type(uv) != UC_ARRAY) {
		uc_vm_raise_exception(vm, EXCEPTION_TYPE,
			"Expecting array of dash lengths");

		return NULL;
	}

	count = ucv_array_length(uv);

	if (count == 0)
		return NULL;

	dashes = calloc(count, sizeof(*dashes));

	if (!dashes)
		return NULL;

	for (i = 0; i < count; i++) {
		entry = ucv_array_get(uv, i);

		if (ucv_type(entry) != UC_INTEGER &&
		    ucv_type(entry) != UC_DOUBLE) {
			uc_vm_raise_exception(vm, EXCEPTION_TYPE,
				"Invalid dash length at index %zu", i);
			free(dashes);

			return NULL;
		}

		dashes[i] = (float)ucv_to_double(entry);
	}

	*ndashes = (int)count;

	return dashes;
}

static void *
uc_pvg_arg_res(uc_vm_t *vm, uc_value_t *uv, const char *type, const char *what)
{
	void *ptr = ucv_resource_data(uv, type);

	if (!ptr)
		uc_vm_raise_exception(vm, EXCEPTION_TYPE,
			"Expecting %s object", what);

	return ptr;
}

static void
uc_pvg_surface_free(void *ptr)
{
	plutovg_surface_destroy(ptr);
}

static void
uc_pvg_canvas_free(void *ptr)
{
	plutovg_canvas_destroy(ptr);
}

static void
uc_pvg_path_free(void *ptr)
{
	plutovg_path_destroy(ptr);
}

static void
uc_pvg_paint_free(void *ptr)
{
	plutovg_paint_destroy(ptr);
}

static void
uc_pvg_font_face_free(void *ptr)
{
	plutovg_font_face_destroy(ptr);
}

static uc_value_t *
uc_pvg_surface_wrap(uc_vm_t *vm, plutovg_surface_t *surface)
{
	uc_value_t *rv;

	if (!surface)
		return NULL;

	rv = ucv_resource_create(vm, UC_PVG_SURFACE, surface);

	if (!rv)
		plutovg_surface_destroy(surface);

	return rv;
}

static uc_value_t *
uc_pvg_path_wrap(uc_vm_t *vm, plutovg_path_t *path)
{
	uc_value_t *rv;

	if (!path)
		return NULL;

	rv = ucv_resource_create(vm, UC_PVG_PATH, path);

	if (!rv)
		plutovg_path_destroy(path);

	return rv;
}

static uc_value_t *
uc_pvg_paint_wrap(uc_vm_t *vm, plutovg_paint_t *paint)
{
	uc_value_t *rv;

	if (!paint)
		return NULL;

	rv = ucv_resource_create(vm, UC_PVG_PAINT, paint);

	if (!rv)
		plutovg_paint_destroy(paint);

	return rv;
}

static uc_value_t *
uc_pvg_font_face_wrap(uc_vm_t *vm, plutovg_font_face_t *face)
{
	uc_value_t *rv;

	if (!face)
		return NULL;

	rv = ucv_resource_create(vm, UC_PVG_FONT_FACE, face);

	if (!rv)
		plutovg_font_face_destroy(face);

	return rv;
}

static uc_value_t *
uc_pvg_version(uc_vm_t *vm, size_t nargs)
{
	return ucv_int64_new(plutovg_version());
}

static uc_value_t *
uc_pvg_version_string(uc_vm_t *vm, size_t nargs)
{
	return ucv_string_new(plutovg_version_string());
}

static uc_value_t *
uc_pvg_deg2rad(uc_vm_t *vm, size_t nargs)
{
	float v[1];

	if (!uc_pvg_args_num(vm, nargs, 0, 1, v))
		return NULL;

	return ucv_double_new(PLUTOVG_DEG2RAD(v[0]));
}

static uc_value_t *
uc_pvg_rad2deg(uc_vm_t *vm, size_t nargs)
{
	float v[1];

	if (!uc_pvg_args_num(vm, nargs, 0, 1, v))
		return NULL;

	return ucv_double_new(PLUTOVG_RAD2DEG(v[0]));
}

static uc_value_t *
uc_pvg_color_parse(uc_vm_t *vm, size_t nargs)
{
	uc_value_t *arg = uc_fn_arg(0);
	plutovg_color_t color;

	if (ucv_type(arg) != UC_STRING) {
		uc_vm_raise_exception(vm, EXCEPTION_TYPE,
			"Expecting color string");

		return NULL;
	}

	if (plutovg_color_parse(&color, ucv_string_get(arg),
	                        ucv_string_length(arg)) == 0)
		return NULL;

	return uc_pvg_color_new(vm, &color);
}

static uc_value_t *
uc_pvg_color_hsl(uc_vm_t *vm, size_t nargs)
{
	plutovg_color_t color;
	float v[3];

	if (!uc_pvg_args_num(vm, nargs, 0, 3, v))
		return NULL;

	plutovg_color_init_hsla(&color, v[0], v[1], v[2],
		uc_pvg_number(uc_fn_arg(3), 1.0f));

	return uc_pvg_color_new(vm, &color);
}

static uc_value_t *
uc_pvg_color_rgba32(uc_vm_t *vm, size_t nargs)
{
	plutovg_color_t color;

	if (!uc_pvg_color_get(uc_fn_arg(0), &color)) {
		uc_vm_raise_exception(vm, EXCEPTION_TYPE, "Invalid color value");

		return NULL;
	}

	return ucv_uint64_new(plutovg_color_to_rgba32(&color));
}

static uc_value_t *
uc_pvg_color_argb32(uc_vm_t *vm, size_t nargs)
{
	plutovg_color_t color;

	if (!uc_pvg_color_get(uc_fn_arg(0), &color)) {
		uc_vm_raise_exception(vm, EXCEPTION_TYPE, "Invalid color value");

		return NULL;
	}

	return ucv_uint64_new(plutovg_color_to_argb32(&color));
}

static uc_value_t *
uc_pvg_matrix_identity(uc_vm_t *vm, size_t nargs)
{
	plutovg_matrix_t matrix;

	plutovg_matrix_init_identity(&matrix);

	return uc_pvg_matrix_new(vm, &matrix);
}

static uc_value_t *
uc_pvg_matrix_init_translate(uc_vm_t *vm, size_t nargs)
{
	plutovg_matrix_t matrix;
	float v[2];

	if (!uc_pvg_args_num(vm, nargs, 0, 2, v))
		return NULL;

	plutovg_matrix_init_translate(&matrix, v[0], v[1]);

	return uc_pvg_matrix_new(vm, &matrix);
}

static uc_value_t *
uc_pvg_matrix_init_scale(uc_vm_t *vm, size_t nargs)
{
	plutovg_matrix_t matrix;
	float v[2];

	if (!uc_pvg_args_num(vm, nargs, 0, 2, v))
		return NULL;

	plutovg_matrix_init_scale(&matrix, v[0], v[1]);

	return uc_pvg_matrix_new(vm, &matrix);
}

static uc_value_t *
uc_pvg_matrix_init_rotate(uc_vm_t *vm, size_t nargs)
{
	plutovg_matrix_t matrix;
	float v[1];

	if (!uc_pvg_args_num(vm, nargs, 0, 1, v))
		return NULL;

	plutovg_matrix_init_rotate(&matrix, v[0]);

	return uc_pvg_matrix_new(vm, &matrix);
}

static uc_value_t *
uc_pvg_matrix_init_shear(uc_vm_t *vm, size_t nargs)
{
	plutovg_matrix_t matrix;
	float v[2];

	if (!uc_pvg_args_num(vm, nargs, 0, 2, v))
		return NULL;

	plutovg_matrix_init_shear(&matrix, v[0], v[1]);

	return uc_pvg_matrix_new(vm, &matrix);
}

static uc_value_t *
uc_pvg_matrix_translate(uc_vm_t *vm, size_t nargs)
{
	plutovg_matrix_t matrix;
	float v[2];

	if (!uc_pvg_matrix_arg(vm, uc_fn_arg(0), &matrix))
		return NULL;

	if (!uc_pvg_args_num(vm, nargs, 1, 2, v))
		return NULL;

	plutovg_matrix_translate(&matrix, v[0], v[1]);

	return uc_pvg_matrix_new(vm, &matrix);
}

static uc_value_t *
uc_pvg_matrix_scale(uc_vm_t *vm, size_t nargs)
{
	plutovg_matrix_t matrix;
	float v[2];

	if (!uc_pvg_matrix_arg(vm, uc_fn_arg(0), &matrix))
		return NULL;

	if (!uc_pvg_args_num(vm, nargs, 1, 2, v))
		return NULL;

	plutovg_matrix_scale(&matrix, v[0], v[1]);

	return uc_pvg_matrix_new(vm, &matrix);
}

static uc_value_t *
uc_pvg_matrix_rotate(uc_vm_t *vm, size_t nargs)
{
	plutovg_matrix_t matrix;
	float v[1];

	if (!uc_pvg_matrix_arg(vm, uc_fn_arg(0), &matrix))
		return NULL;

	if (!uc_pvg_args_num(vm, nargs, 1, 1, v))
		return NULL;

	plutovg_matrix_rotate(&matrix, v[0]);

	return uc_pvg_matrix_new(vm, &matrix);
}

static uc_value_t *
uc_pvg_matrix_shear(uc_vm_t *vm, size_t nargs)
{
	plutovg_matrix_t matrix;
	float v[2];

	if (!uc_pvg_matrix_arg(vm, uc_fn_arg(0), &matrix))
		return NULL;

	if (!uc_pvg_args_num(vm, nargs, 1, 2, v))
		return NULL;

	plutovg_matrix_shear(&matrix, v[0], v[1]);

	return uc_pvg_matrix_new(vm, &matrix);
}

static uc_value_t *
uc_pvg_matrix_multiply(uc_vm_t *vm, size_t nargs)
{
	plutovg_matrix_t left, right, result;

	if (!uc_pvg_matrix_arg(vm, uc_fn_arg(0), &left))
		return NULL;

	if (!uc_pvg_matrix_arg(vm, uc_fn_arg(1), &right))
		return NULL;

	plutovg_matrix_multiply(&result, &left, &right);

	return uc_pvg_matrix_new(vm, &result);
}

static uc_value_t *
uc_pvg_matrix_invert(uc_vm_t *vm, size_t nargs)
{
	plutovg_matrix_t matrix, inverse;

	if (!uc_pvg_matrix_arg(vm, uc_fn_arg(0), &matrix))
		return NULL;

	if (!plutovg_matrix_invert(&matrix, &inverse))
		return NULL;

	return uc_pvg_matrix_new(vm, &inverse);
}

static uc_value_t *
uc_pvg_matrix_map(uc_vm_t *vm, size_t nargs)
{
	plutovg_matrix_t matrix;
	plutovg_point_t point;
	float v[2];

	if (!uc_pvg_matrix_arg(vm, uc_fn_arg(0), &matrix))
		return NULL;

	if (!uc_pvg_args_num(vm, nargs, 1, 2, v))
		return NULL;

	plutovg_matrix_map(&matrix, v[0], v[1], &point.x, &point.y);

	return uc_pvg_point_new(vm, &point);
}

static uc_value_t *
uc_pvg_matrix_map_point(uc_vm_t *vm, size_t nargs)
{
	plutovg_matrix_t matrix;
	plutovg_point_t point;

	if (!uc_pvg_matrix_arg(vm, uc_fn_arg(0), &matrix))
		return NULL;

	if (!uc_pvg_point_get(uc_fn_arg(1), &point)) {
		uc_vm_raise_exception(vm, EXCEPTION_TYPE, "Invalid point value");

		return NULL;
	}

	plutovg_matrix_map_point(&matrix, &point, &point);

	return uc_pvg_point_new(vm, &point);
}

static uc_value_t *
uc_pvg_matrix_map_rect(uc_vm_t *vm, size_t nargs)
{
	plutovg_matrix_t matrix;
	plutovg_rect_t rect;

	if (!uc_pvg_matrix_arg(vm, uc_fn_arg(0), &matrix))
		return NULL;

	if (!uc_pvg_rect_get(uc_fn_arg(1), &rect)) {
		uc_vm_raise_exception(vm, EXCEPTION_TYPE,
			"Invalid rectangle value");

		return NULL;
	}

	plutovg_matrix_map_rect(&matrix, &rect, &rect);

	return uc_pvg_rect_new(vm, &rect);
}

static uc_value_t *
uc_pvg_matrix_parse(uc_vm_t *vm, size_t nargs)
{
	uc_value_t *arg = uc_fn_arg(0);
	plutovg_matrix_t matrix;

	if (ucv_type(arg) != UC_STRING) {
		uc_vm_raise_exception(vm, EXCEPTION_TYPE,
			"Expecting transform string");

		return NULL;
	}

	if (!plutovg_matrix_parse(&matrix, ucv_string_get(arg),
	                          ucv_string_length(arg)))
		return NULL;

	return uc_pvg_matrix_new(vm, &matrix);
}

static uc_value_t *
uc_pvg_surface_create(uc_vm_t *vm, size_t nargs)
{
	float v[2];

	if (!uc_pvg_args_num(vm, nargs, 0, 2, v))
		return NULL;

	if (v[0] <= 0 || v[1] <= 0) {
		uc_vm_raise_exception(vm, EXCEPTION_RUNTIME,
			"Surface dimensions must be positive");

		return NULL;
	}

	return uc_pvg_surface_wrap(vm, plutovg_surface_create((int)v[0],
		(int)v[1]));
}

static uc_value_t *
uc_pvg_surface_from_file(uc_vm_t *vm, size_t nargs)
{
	uc_value_t *arg = uc_fn_arg(0);

	if (ucv_type(arg) != UC_STRING) {
		uc_vm_raise_exception(vm, EXCEPTION_TYPE, "Expecting file name");

		return NULL;
	}

	return uc_pvg_surface_wrap(vm,
		plutovg_surface_load_from_image_file(ucv_string_get(arg)));
}

static uc_value_t *
uc_pvg_surface_from_data(uc_vm_t *vm, size_t nargs)
{
	uc_value_t *arg = uc_fn_arg(0);

	if (ucv_type(arg) != UC_STRING) {
		uc_vm_raise_exception(vm, EXCEPTION_TYPE, "Expecting image data");

		return NULL;
	}

	return uc_pvg_surface_wrap(vm,
		plutovg_surface_load_from_image_data(ucv_string_get(arg),
			(int)ucv_string_length(arg)));
}

static uc_value_t *
uc_pvg_surface_from_base64(uc_vm_t *vm, size_t nargs)
{
	uc_value_t *arg = uc_fn_arg(0);

	if (ucv_type(arg) != UC_STRING) {
		uc_vm_raise_exception(vm, EXCEPTION_TYPE,
			"Expecting base64 image data");

		return NULL;
	}

	return uc_pvg_surface_wrap(vm,
		plutovg_surface_load_from_image_base64(ucv_string_get(arg),
			(int)ucv_string_length(arg)));
}

static uc_value_t *
uc_pvg_surface_width(uc_vm_t *vm, size_t nargs)
{
	plutovg_surface_t *surface = uc_fn_thisval(UC_PVG_SURFACE);

	if (!surface)
		return NULL;

	return ucv_int64_new(plutovg_surface_get_width(surface));
}

static uc_value_t *
uc_pvg_surface_height(uc_vm_t *vm, size_t nargs)
{
	plutovg_surface_t *surface = uc_fn_thisval(UC_PVG_SURFACE);

	if (!surface)
		return NULL;

	return ucv_int64_new(plutovg_surface_get_height(surface));
}

static uc_value_t *
uc_pvg_surface_stride(uc_vm_t *vm, size_t nargs)
{
	plutovg_surface_t *surface = uc_fn_thisval(UC_PVG_SURFACE);

	if (!surface)
		return NULL;

	return ucv_int64_new(plutovg_surface_get_stride(surface));
}

static uc_value_t *
uc_pvg_surface_clear(uc_vm_t *vm, size_t nargs)
{
	plutovg_surface_t *surface = uc_fn_thisval(UC_PVG_SURFACE);
	plutovg_color_t color = PLUTOVG_MAKE_COLOR(0, 0, 0, 0);
	uc_value_t *arg = uc_fn_arg(0);

	if (!surface)
		return NULL;

	if (arg && ucv_type(arg) != UC_NULL && !uc_pvg_color_get(arg, &color)) {
		uc_vm_raise_exception(vm, EXCEPTION_TYPE, "Invalid color value");

		return NULL;
	}

	plutovg_surface_clear(surface, &color);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_surface_data(uc_vm_t *vm, size_t nargs)
{
	plutovg_surface_t *surface = uc_fn_thisval(UC_PVG_SURFACE);
	size_t len;

	if (!surface)
		return NULL;

	len = (size_t)plutovg_surface_get_stride(surface) *
	      (size_t)plutovg_surface_get_height(surface);

	return ucv_string_new_length((char *)plutovg_surface_get_data(surface),
		len);
}

static uc_value_t *
uc_pvg_surface_rgba(uc_vm_t *vm, size_t nargs)
{
	plutovg_surface_t *surface = uc_fn_thisval(UC_PVG_SURFACE);
	unsigned char *buf;
	uc_value_t *rv;
	size_t len;
	int stride;

	if (!surface)
		return NULL;

	stride = plutovg_surface_get_stride(surface);
	len = (size_t)stride * (size_t)plutovg_surface_get_height(surface);
	buf = malloc(len);

	if (!buf)
		return NULL;

	plutovg_convert_argb_to_rgba(buf, plutovg_surface_get_data(surface),
		plutovg_surface_get_width(surface),
		plutovg_surface_get_height(surface), stride);

	rv = ucv_string_new_length((char *)buf, len);
	free(buf);

	return rv;
}

static uc_value_t *
uc_pvg_surface_write_png(uc_vm_t *vm, size_t nargs)
{
	plutovg_surface_t *surface = uc_fn_thisval(UC_PVG_SURFACE);
	uc_value_t *arg = uc_fn_arg(0);

	if (!surface)
		return NULL;

	if (ucv_type(arg) != UC_STRING) {
		uc_vm_raise_exception(vm, EXCEPTION_TYPE, "Expecting file name");

		return NULL;
	}

	return ucv_boolean_new(plutovg_surface_write_to_png(surface,
		ucv_string_get(arg)));
}

static uc_value_t *
uc_pvg_surface_write_jpg(uc_vm_t *vm, size_t nargs)
{
	plutovg_surface_t *surface = uc_fn_thisval(UC_PVG_SURFACE);
	uc_value_t *arg = uc_fn_arg(0);

	if (!surface)
		return NULL;

	if (ucv_type(arg) != UC_STRING) {
		uc_vm_raise_exception(vm, EXCEPTION_TYPE, "Expecting file name");

		return NULL;
	}

	return ucv_boolean_new(plutovg_surface_write_to_jpg(surface,
		ucv_string_get(arg),
		(int)uc_pvg_number(uc_fn_arg(1), 90.0f)));
}

static void
uc_pvg_stream_write(void *closure, void *data, int size)
{
	ucv_stringbuf_addstr((uc_stringbuf_t *)closure, (const char *)data,
		(size_t)size);
}

static uc_value_t *
uc_pvg_surface_png(uc_vm_t *vm, size_t nargs)
{
	plutovg_surface_t *surface = uc_fn_thisval(UC_PVG_SURFACE);
	uc_stringbuf_t *buf;

	if (!surface)
		return NULL;

	buf = ucv_stringbuf_new();

	if (!plutovg_surface_write_to_png_stream(surface, uc_pvg_stream_write,
	                                         buf)) {
		printbuf_free(buf);

		return NULL;
	}

	return ucv_stringbuf_finish(buf);
}

static uc_value_t *
uc_pvg_surface_jpg(uc_vm_t *vm, size_t nargs)
{
	plutovg_surface_t *surface = uc_fn_thisval(UC_PVG_SURFACE);
	uc_stringbuf_t *buf;
	int quality;

	if (!surface)
		return NULL;

	quality = (int)uc_pvg_number(uc_fn_arg(0), 90.0f);
	buf = ucv_stringbuf_new();

	if (!plutovg_surface_write_to_jpg_stream(surface, uc_pvg_stream_write,
	                                         buf, quality)) {
		printbuf_free(buf);

		return NULL;
	}

	return ucv_stringbuf_finish(buf);
}

static uc_value_t *
uc_pvg_path_create(uc_vm_t *vm, size_t nargs)
{
	uc_value_t *arg = uc_fn_arg(0);
	plutovg_path_t *path;

	path = plutovg_path_create();

	if (!path)
		return NULL;

	if (ucv_type(arg) == UC_STRING &&
	    !plutovg_path_parse(path, ucv_string_get(arg),
	                        ucv_string_length(arg))) {
		plutovg_path_destroy(path);
		uc_vm_raise_exception(vm, EXCEPTION_RUNTIME,
			"Unable to parse path data");

		return NULL;
	}

	return uc_pvg_path_wrap(vm, path);
}

static uc_value_t *
uc_pvg_path_move_to(uc_vm_t *vm, size_t nargs)
{
	plutovg_path_t *path = uc_fn_thisval(UC_PVG_PATH);
	float v[2];

	if (!path || !uc_pvg_args_num(vm, nargs, 0, 2, v))
		return NULL;

	plutovg_path_move_to(path, v[0], v[1]);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_path_line_to(uc_vm_t *vm, size_t nargs)
{
	plutovg_path_t *path = uc_fn_thisval(UC_PVG_PATH);
	float v[2];

	if (!path || !uc_pvg_args_num(vm, nargs, 0, 2, v))
		return NULL;

	plutovg_path_line_to(path, v[0], v[1]);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_path_quad_to(uc_vm_t *vm, size_t nargs)
{
	plutovg_path_t *path = uc_fn_thisval(UC_PVG_PATH);
	float v[4];

	if (!path || !uc_pvg_args_num(vm, nargs, 0, 4, v))
		return NULL;

	plutovg_path_quad_to(path, v[0], v[1], v[2], v[3]);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_path_cubic_to(uc_vm_t *vm, size_t nargs)
{
	plutovg_path_t *path = uc_fn_thisval(UC_PVG_PATH);
	float v[6];

	if (!path || !uc_pvg_args_num(vm, nargs, 0, 6, v))
		return NULL;

	plutovg_path_cubic_to(path, v[0], v[1], v[2], v[3], v[4], v[5]);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_path_arc_to(uc_vm_t *vm, size_t nargs)
{
	plutovg_path_t *path = uc_fn_thisval(UC_PVG_PATH);
	float v[3], e[2];

	if (!path || !uc_pvg_args_num(vm, nargs, 0, 3, v))
		return NULL;

	if (!uc_pvg_args_num(vm, nargs, 5, 2, e))
		return NULL;

	plutovg_path_arc_to(path, v[0], v[1], v[2],
		ucv_is_truish(uc_fn_arg(3)), ucv_is_truish(uc_fn_arg(4)),
		e[0], e[1]);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_path_close(uc_vm_t *vm, size_t nargs)
{
	plutovg_path_t *path = uc_fn_thisval(UC_PVG_PATH);

	if (!path)
		return NULL;

	plutovg_path_close(path);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_path_reset(uc_vm_t *vm, size_t nargs)
{
	plutovg_path_t *path = uc_fn_thisval(UC_PVG_PATH);

	if (!path)
		return NULL;

	plutovg_path_reset(path);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_path_reserve(uc_vm_t *vm, size_t nargs)
{
	plutovg_path_t *path = uc_fn_thisval(UC_PVG_PATH);
	float v[1];

	if (!path || !uc_pvg_args_num(vm, nargs, 0, 1, v))
		return NULL;

	plutovg_path_reserve(path, (int)v[0]);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_path_current_point(uc_vm_t *vm, size_t nargs)
{
	plutovg_path_t *path = uc_fn_thisval(UC_PVG_PATH);
	plutovg_point_t point;

	if (!path)
		return NULL;

	plutovg_path_get_current_point(path, &point.x, &point.y);

	return uc_pvg_point_new(vm, &point);
}

static uc_value_t *
uc_pvg_path_add_rect(uc_vm_t *vm, size_t nargs)
{
	plutovg_path_t *path = uc_fn_thisval(UC_PVG_PATH);
	float v[4];

	if (!path || !uc_pvg_args_num(vm, nargs, 0, 4, v))
		return NULL;

	plutovg_path_add_rect(path, v[0], v[1], v[2], v[3]);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_path_add_round_rect(uc_vm_t *vm, size_t nargs)
{
	plutovg_path_t *path = uc_fn_thisval(UC_PVG_PATH);
	float v[6];

	if (!path || !uc_pvg_args_num(vm, nargs, 0, 6, v))
		return NULL;

	plutovg_path_add_round_rect(path, v[0], v[1], v[2], v[3], v[4], v[5]);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_path_add_ellipse(uc_vm_t *vm, size_t nargs)
{
	plutovg_path_t *path = uc_fn_thisval(UC_PVG_PATH);
	float v[4];

	if (!path || !uc_pvg_args_num(vm, nargs, 0, 4, v))
		return NULL;

	plutovg_path_add_ellipse(path, v[0], v[1], v[2], v[3]);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_path_add_circle(uc_vm_t *vm, size_t nargs)
{
	plutovg_path_t *path = uc_fn_thisval(UC_PVG_PATH);
	float v[3];

	if (!path || !uc_pvg_args_num(vm, nargs, 0, 3, v))
		return NULL;

	plutovg_path_add_circle(path, v[0], v[1], v[2]);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_path_add_arc(uc_vm_t *vm, size_t nargs)
{
	plutovg_path_t *path = uc_fn_thisval(UC_PVG_PATH);
	float v[5];

	if (!path || !uc_pvg_args_num(vm, nargs, 0, 5, v))
		return NULL;

	plutovg_path_add_arc(path, v[0], v[1], v[2], v[3], v[4],
		ucv_is_truish(uc_fn_arg(5)));

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_path_add_path(uc_vm_t *vm, size_t nargs)
{
	plutovg_path_t *path = uc_fn_thisval(UC_PVG_PATH);
	const plutovg_matrix_t *mptr;
	plutovg_matrix_t matrix;
	plutovg_path_t *source;

	if (!path)
		return NULL;

	source = uc_pvg_arg_res(vm, uc_fn_arg(0), UC_PVG_PATH, "path");

	if (!source)
		return NULL;

	if (!uc_pvg_matrix_opt(vm, uc_fn_arg(1), &matrix, &mptr))
		return NULL;

	plutovg_path_add_path(path, source, mptr);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_path_transform(uc_vm_t *vm, size_t nargs)
{
	plutovg_path_t *path = uc_fn_thisval(UC_PVG_PATH);
	plutovg_matrix_t matrix;

	if (!path || !uc_pvg_matrix_arg(vm, uc_fn_arg(0), &matrix))
		return NULL;

	plutovg_path_transform(path, &matrix);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_path_clone(uc_vm_t *vm, size_t nargs)
{
	plutovg_path_t *path = uc_fn_thisval(UC_PVG_PATH);

	if (!path)
		return NULL;

	return uc_pvg_path_wrap(vm, plutovg_path_clone(path));
}

static uc_value_t *
uc_pvg_path_clone_flatten(uc_vm_t *vm, size_t nargs)
{
	plutovg_path_t *path = uc_fn_thisval(UC_PVG_PATH);

	if (!path)
		return NULL;

	return uc_pvg_path_wrap(vm, plutovg_path_clone_flatten(path));
}

static uc_value_t *
uc_pvg_path_clone_dashed(uc_vm_t *vm, size_t nargs)
{
	plutovg_path_t *path = uc_fn_thisval(UC_PVG_PATH);
	uc_value_t *rv;
	float *dashes;
	int ndashes;

	if (!path)
		return NULL;

	dashes = uc_pvg_dashes_get(vm, uc_fn_arg(1), &ndashes);

	if (vm->exception.type != EXCEPTION_NONE)
		return NULL;

	rv = uc_pvg_path_wrap(vm, plutovg_path_clone_dashed(path,
		uc_pvg_number(uc_fn_arg(0), 0.0f), dashes, ndashes));
	free(dashes);

	return rv;
}

static uc_value_t *
uc_pvg_path_extents(uc_vm_t *vm, size_t nargs)
{
	plutovg_path_t *path = uc_fn_thisval(UC_PVG_PATH);
	plutovg_rect_t rect;

	if (!path)
		return NULL;

	plutovg_path_extents(path, &rect, ucv_is_truish(uc_fn_arg(0)));

	return uc_pvg_rect_new(vm, &rect);
}

static uc_value_t *
uc_pvg_path_length(uc_vm_t *vm, size_t nargs)
{
	plutovg_path_t *path = uc_fn_thisval(UC_PVG_PATH);

	if (!path)
		return NULL;

	return ucv_double_new(plutovg_path_length(path));
}

static uc_value_t *
uc_pvg_path_parse(uc_vm_t *vm, size_t nargs)
{
	plutovg_path_t *path = uc_fn_thisval(UC_PVG_PATH);
	uc_value_t *arg = uc_fn_arg(0);

	if (!path)
		return NULL;

	if (ucv_type(arg) != UC_STRING) {
		uc_vm_raise_exception(vm, EXCEPTION_TYPE, "Expecting path data");

		return NULL;
	}

	return ucv_boolean_new(plutovg_path_parse(path, ucv_string_get(arg),
		ucv_string_length(arg)));
}

static uc_value_t *
uc_pvg_path_elements(uc_vm_t *vm, size_t nargs)
{
	plutovg_path_t *path = uc_fn_thisval(UC_PVG_PATH);
	plutovg_path_iterator_t it;
	plutovg_point_t points[3];
	uc_value_t *rv, *entry;
	int command, npoints;
	uc_value_t *coords;
	int i;

	if (!path)
		return NULL;

	plutovg_path_iterator_init(&it, path);

	rv = ucv_array_new(vm);

	while (plutovg_path_iterator_has_next(&it)) {
		command = plutovg_path_iterator_next(&it, points);
		npoints = (command == PLUTOVG_PATH_COMMAND_CUBIC_TO) ? 3 : 1;
		coords = ucv_array_new(vm);

		for (i = 0; i < npoints; i++)
			ucv_array_push(coords, uc_pvg_point_new(vm, &points[i]));

		entry = ucv_object_new(vm);
		ucv_object_add(entry, "command",
			ucv_string_new(uc_pvg_command_names[command]));
		ucv_object_add(entry, "points", coords);
		ucv_array_push(rv, entry);
	}

	return rv;
}

static uc_value_t *
uc_pvg_paint_color(uc_vm_t *vm, size_t nargs)
{
	plutovg_color_t color;

	if (!uc_pvg_color_get(uc_fn_arg(0), &color)) {
		uc_vm_raise_exception(vm, EXCEPTION_TYPE, "Invalid color value");

		return NULL;
	}

	return uc_pvg_paint_wrap(vm, plutovg_paint_create_color(&color));
}

static uc_value_t *
uc_pvg_paint_linear_gradient(uc_vm_t *vm, size_t nargs)
{
	plutovg_gradient_stop_t *stops;
	const plutovg_matrix_t *mptr;
	plutovg_matrix_t matrix;
	int spread, nstops;
	uc_value_t *rv;
	float v[4];

	spread = PLUTOVG_SPREAD_METHOD_PAD;

	if (!uc_pvg_args_num(vm, nargs, 0, 4, v))
		return NULL;

	if (!uc_pvg_enum_opt(vm, uc_fn_arg(4), uc_pvg_spread_names,
	                     "spread method", &spread))
		return NULL;

	stops = uc_pvg_stops_get(vm, uc_fn_arg(5), &nstops);

	if (!stops)
		return NULL;

	if (!uc_pvg_matrix_opt(vm, uc_fn_arg(6), &matrix, &mptr)) {
		free(stops);

		return NULL;
	}

	rv = uc_pvg_paint_wrap(vm, plutovg_paint_create_linear_gradient(v[0],
		v[1], v[2], v[3], spread, stops, nstops, mptr));
	free(stops);

	return rv;
}

static uc_value_t *
uc_pvg_paint_radial_gradient(uc_vm_t *vm, size_t nargs)
{
	plutovg_gradient_stop_t *stops;
	const plutovg_matrix_t *mptr;
	plutovg_matrix_t matrix;
	int spread, nstops;
	uc_value_t *rv;
	float v[6];

	spread = PLUTOVG_SPREAD_METHOD_PAD;

	if (!uc_pvg_args_num(vm, nargs, 0, 6, v))
		return NULL;

	if (!uc_pvg_enum_opt(vm, uc_fn_arg(6), uc_pvg_spread_names,
	                     "spread method", &spread))
		return NULL;

	stops = uc_pvg_stops_get(vm, uc_fn_arg(7), &nstops);

	if (!stops)
		return NULL;

	if (!uc_pvg_matrix_opt(vm, uc_fn_arg(8), &matrix, &mptr)) {
		free(stops);

		return NULL;
	}

	rv = uc_pvg_paint_wrap(vm, plutovg_paint_create_radial_gradient(v[0],
		v[1], v[2], v[3], v[4], v[5], spread, stops, nstops, mptr));
	free(stops);

	return rv;
}

static uc_value_t *
uc_pvg_paint_texture(uc_vm_t *vm, size_t nargs)
{
	const plutovg_matrix_t *mptr;
	plutovg_surface_t *surface;
	plutovg_matrix_t matrix;
	int type;

	type = PLUTOVG_TEXTURE_TYPE_TILED;
	surface = uc_pvg_arg_res(vm, uc_fn_arg(0), UC_PVG_SURFACE, "surface");

	if (!surface)
		return NULL;

	if (!uc_pvg_enum_opt(vm, uc_fn_arg(1), uc_pvg_texture_names,
	                     "texture type", &type))
		return NULL;

	if (!uc_pvg_matrix_opt(vm, uc_fn_arg(3), &matrix, &mptr))
		return NULL;

	return uc_pvg_paint_wrap(vm, plutovg_paint_create_texture(surface, type,
		uc_pvg_number(uc_fn_arg(2), 1.0f), mptr));
}

static uc_value_t *
uc_pvg_font_face_from_file(uc_vm_t *vm, size_t nargs)
{
	uc_value_t *arg = uc_fn_arg(0);

	if (ucv_type(arg) != UC_STRING) {
		uc_vm_raise_exception(vm, EXCEPTION_TYPE, "Expecting file name");

		return NULL;
	}

	return uc_pvg_font_face_wrap(vm,
		plutovg_font_face_load_from_file(ucv_string_get(arg),
			(int)uc_pvg_number(uc_fn_arg(1), 0.0f)));
}

static uc_value_t *
uc_pvg_font_face_from_data(uc_vm_t *vm, size_t nargs)
{
	uc_value_t *arg = uc_fn_arg(0);
	plutovg_font_face_t *face;
	size_t len;
	char *copy;

	if (ucv_type(arg) != UC_STRING) {
		uc_vm_raise_exception(vm, EXCEPTION_TYPE, "Expecting font data");

		return NULL;
	}

	len = ucv_string_length(arg);
	copy = malloc(len);

	if (!copy)
		return NULL;

	memcpy(copy, ucv_string_get(arg), len);

	face = plutovg_font_face_load_from_data(copy, (unsigned int)len,
		(int)uc_pvg_number(uc_fn_arg(1), 0.0f), free, copy);

	if (!face) {
		free(copy);

		return NULL;
	}

	return uc_pvg_font_face_wrap(vm, face);
}

static uc_value_t *
uc_pvg_font_face_metrics(uc_vm_t *vm, size_t nargs)
{
	plutovg_font_face_t *face = uc_fn_thisval(UC_PVG_FONT_FACE);
	float ascent, descent, line_gap;
	plutovg_rect_t extents;
	uc_value_t *rv;
	float v[1];

	if (!face || !uc_pvg_args_num(vm, nargs, 0, 1, v))
		return NULL;

	plutovg_font_face_get_metrics(face, v[0], &ascent, &descent, &line_gap,
		&extents);

	rv = ucv_object_new(vm);
	ucv_object_add(rv, "ascent", ucv_double_new(ascent));
	ucv_object_add(rv, "descent", ucv_double_new(descent));
	ucv_object_add(rv, "line_gap", ucv_double_new(line_gap));
	ucv_object_add(rv, "extents", uc_pvg_rect_new(vm, &extents));

	return rv;
}

static uc_value_t *
uc_pvg_font_face_glyph_metrics(uc_vm_t *vm, size_t nargs)
{
	plutovg_font_face_t *face = uc_fn_thisval(UC_PVG_FONT_FACE);
	float advance_width, left_side_bearing;
	plutovg_rect_t extents;
	uc_value_t *rv;
	float v[2];

	if (!face || !uc_pvg_args_num(vm, nargs, 0, 2, v))
		return NULL;

	plutovg_font_face_get_glyph_metrics(face, v[0],
		(plutovg_codepoint_t)v[1], &advance_width, &left_side_bearing,
		&extents);

	rv = ucv_object_new(vm);
	ucv_object_add(rv, "advance_width", ucv_double_new(advance_width));
	ucv_object_add(rv, "left_side_bearing",
		ucv_double_new(left_side_bearing));
	ucv_object_add(rv, "extents", uc_pvg_rect_new(vm, &extents));

	return rv;
}

static uc_value_t *
uc_pvg_font_face_glyph_path(uc_vm_t *vm, size_t nargs)
{
	plutovg_font_face_t *face = uc_fn_thisval(UC_PVG_FONT_FACE);
	plutovg_path_t *path;
	float v[4];

	if (!face || !uc_pvg_args_num(vm, nargs, 0, 4, v))
		return NULL;

	path = uc_pvg_arg_res(vm, uc_fn_arg(4), UC_PVG_PATH, "path");

	if (!path)
		return NULL;

	return ucv_double_new(plutovg_font_face_get_glyph_path(face, v[0], v[1],
		v[2], (plutovg_codepoint_t)v[3], path));
}

static uc_value_t *
uc_pvg_font_face_text_extents(uc_vm_t *vm, size_t nargs)
{
	plutovg_font_face_t *face = uc_fn_thisval(UC_PVG_FONT_FACE);
	uc_value_t *text = uc_fn_arg(1);
	plutovg_rect_t extents;
	uc_value_t *rv;
	float advance;
	float v[1];

	if (!face || !uc_pvg_args_num(vm, nargs, 0, 1, v))
		return NULL;

	if (ucv_type(text) != UC_STRING) {
		uc_vm_raise_exception(vm, EXCEPTION_TYPE,
			"Expecting text string");

		return NULL;
	}

	advance = plutovg_font_face_text_extents(face, v[0],
		ucv_string_get(text), (int)ucv_string_length(text),
		PLUTOVG_TEXT_ENCODING_UTF8, &extents);

	rv = ucv_object_new(vm);
	ucv_object_add(rv, "advance_width", ucv_double_new(advance));
	ucv_object_add(rv, "extents", uc_pvg_rect_new(vm, &extents));

	return rv;
}

static uc_value_t *
uc_pvg_canvas_create(uc_vm_t *vm, size_t nargs)
{
	plutovg_surface_t *surface;
	plutovg_canvas_t *canvas;
	uc_value_t *rv;

	surface = uc_pvg_arg_res(vm, uc_fn_arg(0), UC_PVG_SURFACE, "surface");

	if (!surface)
		return NULL;

	canvas = plutovg_canvas_create(surface);

	if (!canvas)
		return NULL;

	rv = ucv_resource_create(vm, UC_PVG_CANVAS, canvas);

	if (!rv)
		plutovg_canvas_destroy(canvas);

	return rv;
}

static uc_value_t *
uc_pvg_canvas_surface(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);

	if (!canvas)
		return NULL;

	return uc_pvg_surface_wrap(vm, plutovg_surface_reference(
		plutovg_canvas_get_surface(canvas)));
}

static uc_value_t *
uc_pvg_canvas_save(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);

	if (!canvas)
		return NULL;

	plutovg_canvas_save(canvas);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_canvas_restore(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);

	if (!canvas)
		return NULL;

	plutovg_canvas_restore(canvas);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_canvas_set_color(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	plutovg_color_t color;

	if (!canvas)
		return NULL;

	if (!uc_pvg_color_get(uc_fn_arg(0), &color)) {
		uc_vm_raise_exception(vm, EXCEPTION_TYPE, "Invalid color value");

		return NULL;
	}

	plutovg_canvas_set_color(canvas, &color);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_canvas_color(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	plutovg_color_t color;

	if (!canvas)
		return NULL;

	plutovg_canvas_get_paint(canvas, &color);

	return uc_pvg_color_new(vm, &color);
}

static uc_value_t *
uc_pvg_canvas_get_paint(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	plutovg_paint_t *paint;

	if (!canvas)
		return NULL;

	paint = plutovg_canvas_get_paint(canvas, NULL);

	if (!paint)
		return NULL;

	return uc_pvg_paint_wrap(vm, plutovg_paint_reference(paint));
}

static uc_value_t *
uc_pvg_canvas_set_paint(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	plutovg_paint_t *paint;

	if (!canvas)
		return NULL;

	paint = uc_pvg_arg_res(vm, uc_fn_arg(0), UC_PVG_PAINT, "paint");

	if (!paint)
		return NULL;

	plutovg_canvas_set_paint(canvas, paint);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_canvas_set_linear_gradient(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	plutovg_gradient_stop_t *stops;
	const plutovg_matrix_t *mptr;
	plutovg_matrix_t matrix;
	int spread, nstops;
	float v[4];

	spread = PLUTOVG_SPREAD_METHOD_PAD;

	if (!canvas || !uc_pvg_args_num(vm, nargs, 0, 4, v))
		return NULL;

	if (!uc_pvg_enum_opt(vm, uc_fn_arg(4), uc_pvg_spread_names,
	                     "spread method", &spread))
		return NULL;

	stops = uc_pvg_stops_get(vm, uc_fn_arg(5), &nstops);

	if (!stops)
		return NULL;

	if (!uc_pvg_matrix_opt(vm, uc_fn_arg(6), &matrix, &mptr)) {
		free(stops);

		return NULL;
	}

	plutovg_canvas_set_linear_gradient(canvas, v[0], v[1], v[2], v[3],
		spread, stops, nstops, mptr);
	free(stops);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_canvas_set_radial_gradient(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	plutovg_gradient_stop_t *stops;
	const plutovg_matrix_t *mptr;
	plutovg_matrix_t matrix;
	int spread, nstops;
	float v[6];

	spread = PLUTOVG_SPREAD_METHOD_PAD;

	if (!canvas || !uc_pvg_args_num(vm, nargs, 0, 6, v))
		return NULL;

	if (!uc_pvg_enum_opt(vm, uc_fn_arg(6), uc_pvg_spread_names,
	                     "spread method", &spread))
		return NULL;

	stops = uc_pvg_stops_get(vm, uc_fn_arg(7), &nstops);

	if (!stops)
		return NULL;

	if (!uc_pvg_matrix_opt(vm, uc_fn_arg(8), &matrix, &mptr)) {
		free(stops);

		return NULL;
	}

	plutovg_canvas_set_radial_gradient(canvas, v[0], v[1], v[2], v[3], v[4],
		v[5], spread, stops, nstops, mptr);
	free(stops);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_canvas_set_texture(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	const plutovg_matrix_t *mptr;
	plutovg_surface_t *surface;
	plutovg_matrix_t matrix;
	int type;

	type = PLUTOVG_TEXTURE_TYPE_TILED;

	if (!canvas)
		return NULL;

	surface = uc_pvg_arg_res(vm, uc_fn_arg(0), UC_PVG_SURFACE, "surface");

	if (!surface)
		return NULL;

	if (!uc_pvg_enum_opt(vm, uc_fn_arg(1), uc_pvg_texture_names,
	                     "texture type", &type))
		return NULL;

	if (!uc_pvg_matrix_opt(vm, uc_fn_arg(3), &matrix, &mptr))
		return NULL;

	plutovg_canvas_set_texture(canvas, surface, type,
		uc_pvg_number(uc_fn_arg(2), 1.0f), mptr);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_canvas_set_font(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	plutovg_font_face_t *face;
	float v[1];

	if (!canvas)
		return NULL;

	face = uc_pvg_arg_res(vm, uc_fn_arg(0), UC_PVG_FONT_FACE, "font face");

	if (!face || !uc_pvg_args_num(vm, nargs, 1, 1, v))
		return NULL;

	plutovg_canvas_set_font(canvas, face, v[0]);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_canvas_set_font_face(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	plutovg_font_face_t *face;

	if (!canvas)
		return NULL;

	face = uc_pvg_arg_res(vm, uc_fn_arg(0), UC_PVG_FONT_FACE, "font face");

	if (!face)
		return NULL;

	plutovg_canvas_set_font_face(canvas, face);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_canvas_font_face(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	plutovg_font_face_t *face;

	if (!canvas)
		return NULL;

	face = plutovg_canvas_get_font_face(canvas);

	if (!face)
		return NULL;

	return uc_pvg_font_face_wrap(vm, plutovg_font_face_reference(face));
}

static uc_value_t *
uc_pvg_canvas_set_font_size(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	float v[1];

	if (!canvas || !uc_pvg_args_num(vm, nargs, 0, 1, v))
		return NULL;

	plutovg_canvas_set_font_size(canvas, v[0]);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_canvas_font_size(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);

	if (!canvas)
		return NULL;

	return ucv_double_new(plutovg_canvas_get_font_size(canvas));
}

static uc_value_t *
uc_pvg_canvas_add_font_face(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	uc_value_t *family = uc_fn_arg(0);
	plutovg_font_face_t *face;

	if (!canvas)
		return NULL;

	if (ucv_type(family) != UC_STRING) {
		uc_vm_raise_exception(vm, EXCEPTION_TYPE,
			"Expecting font family name");

		return NULL;
	}

	face = uc_pvg_arg_res(vm, uc_fn_arg(3), UC_PVG_FONT_FACE, "font face");

	if (!face)
		return NULL;

	plutovg_canvas_add_font_face(canvas, ucv_string_get(family),
		ucv_is_truish(uc_fn_arg(1)), ucv_is_truish(uc_fn_arg(2)), face);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_canvas_add_font_file(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	uc_value_t *family = uc_fn_arg(0);
	uc_value_t *file = uc_fn_arg(3);

	if (!canvas)
		return NULL;

	if (ucv_type(family) != UC_STRING || ucv_type(file) != UC_STRING) {
		uc_vm_raise_exception(vm, EXCEPTION_TYPE,
			"Expecting font family name and file name");

		return NULL;
	}

	return ucv_boolean_new(plutovg_canvas_add_font_file(canvas,
		ucv_string_get(family), ucv_is_truish(uc_fn_arg(1)),
		ucv_is_truish(uc_fn_arg(2)), ucv_string_get(file),
		(int)uc_pvg_number(uc_fn_arg(4), 0.0f)));
}

static uc_value_t *
uc_pvg_canvas_select_font_face(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	uc_value_t *family = uc_fn_arg(0);

	if (!canvas)
		return NULL;

	if (ucv_type(family) != UC_STRING) {
		uc_vm_raise_exception(vm, EXCEPTION_TYPE,
			"Expecting font family name");

		return NULL;
	}

	return ucv_boolean_new(plutovg_canvas_select_font_face(canvas,
		ucv_string_get(family), ucv_is_truish(uc_fn_arg(1)),
		ucv_is_truish(uc_fn_arg(2))));
}

static uc_value_t *
uc_pvg_canvas_set_fill_rule(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	int rule;

	if (!canvas)
		return NULL;

	if (!uc_pvg_enum_get(vm, uc_fn_arg(0), uc_pvg_fill_rule_names,
	                     "fill rule", &rule))
		return NULL;

	plutovg_canvas_set_fill_rule(canvas, rule);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_canvas_fill_rule(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);

	if (!canvas)
		return NULL;

	return ucv_string_new(
		uc_pvg_fill_rule_names[plutovg_canvas_get_fill_rule(canvas)]);
}

static uc_value_t *
uc_pvg_canvas_set_operator(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	int op;

	if (!canvas)
		return NULL;

	if (!uc_pvg_enum_get(vm, uc_fn_arg(0), uc_pvg_operator_names,
	                     "operator", &op))
		return NULL;

	plutovg_canvas_set_operator(canvas, op);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_canvas_operator(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);

	if (!canvas)
		return NULL;

	return ucv_string_new(
		uc_pvg_operator_names[plutovg_canvas_get_operator(canvas)]);
}

static uc_value_t *
uc_pvg_canvas_set_opacity(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	float v[1];

	if (!canvas || !uc_pvg_args_num(vm, nargs, 0, 1, v))
		return NULL;

	plutovg_canvas_set_opacity(canvas, v[0]);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_canvas_opacity(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);

	if (!canvas)
		return NULL;

	return ucv_double_new(plutovg_canvas_get_opacity(canvas));
}

static uc_value_t *
uc_pvg_canvas_set_line_width(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	float v[1];

	if (!canvas || !uc_pvg_args_num(vm, nargs, 0, 1, v))
		return NULL;

	plutovg_canvas_set_line_width(canvas, v[0]);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_canvas_line_width(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);

	if (!canvas)
		return NULL;

	return ucv_double_new(plutovg_canvas_get_line_width(canvas));
}

static uc_value_t *
uc_pvg_canvas_set_line_cap(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	int cap;

	if (!canvas)
		return NULL;

	if (!uc_pvg_enum_get(vm, uc_fn_arg(0), uc_pvg_line_cap_names,
	                     "line cap", &cap))
		return NULL;

	plutovg_canvas_set_line_cap(canvas, cap);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_canvas_line_cap(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);

	if (!canvas)
		return NULL;

	return ucv_string_new(
		uc_pvg_line_cap_names[plutovg_canvas_get_line_cap(canvas)]);
}

static uc_value_t *
uc_pvg_canvas_set_line_join(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	int join;

	if (!canvas)
		return NULL;

	if (!uc_pvg_enum_get(vm, uc_fn_arg(0), uc_pvg_line_join_names,
	                     "line join", &join))
		return NULL;

	plutovg_canvas_set_line_join(canvas, join);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_canvas_line_join(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);

	if (!canvas)
		return NULL;

	return ucv_string_new(
		uc_pvg_line_join_names[plutovg_canvas_get_line_join(canvas)]);
}

static uc_value_t *
uc_pvg_canvas_set_miter_limit(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	float v[1];

	if (!canvas || !uc_pvg_args_num(vm, nargs, 0, 1, v))
		return NULL;

	plutovg_canvas_set_miter_limit(canvas, v[0]);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_canvas_miter_limit(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);

	if (!canvas)
		return NULL;

	return ucv_double_new(plutovg_canvas_get_miter_limit(canvas));
}

static uc_value_t *
uc_pvg_canvas_set_dash(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	float *dashes;
	int ndashes;
	float v[1];

	if (!canvas || !uc_pvg_args_num(vm, nargs, 0, 1, v))
		return NULL;

	dashes = uc_pvg_dashes_get(vm, uc_fn_arg(1), &ndashes);

	if (vm->exception.type != EXCEPTION_NONE)
		return NULL;

	plutovg_canvas_set_dash(canvas, v[0], dashes, ndashes);
	free(dashes);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_canvas_set_dash_offset(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	float v[1];

	if (!canvas || !uc_pvg_args_num(vm, nargs, 0, 1, v))
		return NULL;

	plutovg_canvas_set_dash_offset(canvas, v[0]);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_canvas_dash_offset(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);

	if (!canvas)
		return NULL;

	return ucv_double_new(plutovg_canvas_get_dash_offset(canvas));
}

static uc_value_t *
uc_pvg_canvas_set_dash_array(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	float *dashes;
	int ndashes;

	if (!canvas)
		return NULL;

	dashes = uc_pvg_dashes_get(vm, uc_fn_arg(0), &ndashes);

	if (vm->exception.type != EXCEPTION_NONE)
		return NULL;

	plutovg_canvas_set_dash_array(canvas, dashes, ndashes);
	free(dashes);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_canvas_dash_array(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	const float *dashes;
	uc_value_t *rv;
	int count, i;

	if (!canvas)
		return NULL;

	count = plutovg_canvas_get_dash_array(canvas, &dashes);
	rv = ucv_array_new(vm);

	for (i = 0; i < count; i++)
		ucv_array_push(rv, ucv_double_new(dashes[i]));

	return rv;
}

static uc_value_t *
uc_pvg_canvas_translate(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	float v[2];

	if (!canvas || !uc_pvg_args_num(vm, nargs, 0, 2, v))
		return NULL;

	plutovg_canvas_translate(canvas, v[0], v[1]);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_canvas_scale(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	float v[2];

	if (!canvas || !uc_pvg_args_num(vm, nargs, 0, 2, v))
		return NULL;

	plutovg_canvas_scale(canvas, v[0], v[1]);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_canvas_shear(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	float v[2];

	if (!canvas || !uc_pvg_args_num(vm, nargs, 0, 2, v))
		return NULL;

	plutovg_canvas_shear(canvas, v[0], v[1]);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_canvas_rotate(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	float v[1];

	if (!canvas || !uc_pvg_args_num(vm, nargs, 0, 1, v))
		return NULL;

	plutovg_canvas_rotate(canvas, v[0]);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_canvas_transform(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	plutovg_matrix_t matrix;

	if (!canvas || !uc_pvg_matrix_arg(vm, uc_fn_arg(0), &matrix))
		return NULL;

	plutovg_canvas_transform(canvas, &matrix);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_canvas_reset_matrix(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);

	if (!canvas)
		return NULL;

	plutovg_canvas_reset_matrix(canvas);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_canvas_set_matrix(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	plutovg_matrix_t matrix;

	if (!canvas || !uc_pvg_matrix_arg(vm, uc_fn_arg(0), &matrix))
		return NULL;

	plutovg_canvas_set_matrix(canvas, &matrix);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_canvas_matrix(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	plutovg_matrix_t matrix;

	if (!canvas)
		return NULL;

	plutovg_canvas_get_matrix(canvas, &matrix);

	return uc_pvg_matrix_new(vm, &matrix);
}

static uc_value_t *
uc_pvg_canvas_map(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	plutovg_point_t point;
	float v[2];

	if (!canvas || !uc_pvg_args_num(vm, nargs, 0, 2, v))
		return NULL;

	plutovg_canvas_map(canvas, v[0], v[1], &point.x, &point.y);

	return uc_pvg_point_new(vm, &point);
}

static uc_value_t *
uc_pvg_canvas_map_point(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	plutovg_point_t point;

	if (!canvas)
		return NULL;

	if (!uc_pvg_point_get(uc_fn_arg(0), &point)) {
		uc_vm_raise_exception(vm, EXCEPTION_TYPE, "Invalid point value");

		return NULL;
	}

	plutovg_canvas_map_point(canvas, &point, &point);

	return uc_pvg_point_new(vm, &point);
}

static uc_value_t *
uc_pvg_canvas_map_rect(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	plutovg_rect_t rect;

	if (!canvas)
		return NULL;

	if (!uc_pvg_rect_get(uc_fn_arg(0), &rect)) {
		uc_vm_raise_exception(vm, EXCEPTION_TYPE,
			"Invalid rectangle value");

		return NULL;
	}

	plutovg_canvas_map_rect(canvas, &rect, &rect);

	return uc_pvg_rect_new(vm, &rect);
}

static uc_value_t *
uc_pvg_canvas_move_to(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	float v[2];

	if (!canvas || !uc_pvg_args_num(vm, nargs, 0, 2, v))
		return NULL;

	plutovg_canvas_move_to(canvas, v[0], v[1]);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_canvas_line_to(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	float v[2];

	if (!canvas || !uc_pvg_args_num(vm, nargs, 0, 2, v))
		return NULL;

	plutovg_canvas_line_to(canvas, v[0], v[1]);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_canvas_quad_to(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	float v[4];

	if (!canvas || !uc_pvg_args_num(vm, nargs, 0, 4, v))
		return NULL;

	plutovg_canvas_quad_to(canvas, v[0], v[1], v[2], v[3]);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_canvas_cubic_to(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	float v[6];

	if (!canvas || !uc_pvg_args_num(vm, nargs, 0, 6, v))
		return NULL;

	plutovg_canvas_cubic_to(canvas, v[0], v[1], v[2], v[3], v[4], v[5]);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_canvas_arc_to(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	float v[3], e[2];

	if (!canvas || !uc_pvg_args_num(vm, nargs, 0, 3, v))
		return NULL;

	if (!uc_pvg_args_num(vm, nargs, 5, 2, e))
		return NULL;

	plutovg_canvas_arc_to(canvas, v[0], v[1], v[2],
		ucv_is_truish(uc_fn_arg(3)), ucv_is_truish(uc_fn_arg(4)),
		e[0], e[1]);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_canvas_rect(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	float v[4];

	if (!canvas || !uc_pvg_args_num(vm, nargs, 0, 4, v))
		return NULL;

	plutovg_canvas_rect(canvas, v[0], v[1], v[2], v[3]);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_canvas_round_rect(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	float v[6];

	if (!canvas || !uc_pvg_args_num(vm, nargs, 0, 6, v))
		return NULL;

	plutovg_canvas_round_rect(canvas, v[0], v[1], v[2], v[3], v[4], v[5]);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_canvas_ellipse(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	float v[4];

	if (!canvas || !uc_pvg_args_num(vm, nargs, 0, 4, v))
		return NULL;

	plutovg_canvas_ellipse(canvas, v[0], v[1], v[2], v[3]);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_canvas_circle(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	float v[3];

	if (!canvas || !uc_pvg_args_num(vm, nargs, 0, 3, v))
		return NULL;

	plutovg_canvas_circle(canvas, v[0], v[1], v[2]);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_canvas_arc(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	float v[5];

	if (!canvas || !uc_pvg_args_num(vm, nargs, 0, 5, v))
		return NULL;

	plutovg_canvas_arc(canvas, v[0], v[1], v[2], v[3], v[4],
		ucv_is_truish(uc_fn_arg(5)));

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_canvas_add_path(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	plutovg_path_t *path;

	if (!canvas)
		return NULL;

	path = uc_pvg_arg_res(vm, uc_fn_arg(0), UC_PVG_PATH, "path");

	if (!path)
		return NULL;

	plutovg_canvas_add_path(canvas, path);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_canvas_new_path(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);

	if (!canvas)
		return NULL;

	plutovg_canvas_new_path(canvas);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_canvas_close_path(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);

	if (!canvas)
		return NULL;

	plutovg_canvas_close_path(canvas);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_canvas_current_point(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	plutovg_point_t point;

	if (!canvas)
		return NULL;

	plutovg_canvas_get_current_point(canvas, &point.x, &point.y);

	return uc_pvg_point_new(vm, &point);
}

static uc_value_t *
uc_pvg_canvas_path(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	plutovg_path_t *path;

	if (!canvas)
		return NULL;

	path = plutovg_canvas_get_path(canvas);

	if (!path)
		return NULL;

	return uc_pvg_path_wrap(vm, plutovg_path_reference(path));
}

static uc_value_t *
uc_pvg_canvas_fill_contains(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	float v[2];

	if (!canvas || !uc_pvg_args_num(vm, nargs, 0, 2, v))
		return NULL;

	return ucv_boolean_new(plutovg_canvas_fill_contains(canvas, v[0], v[1]));
}

static uc_value_t *
uc_pvg_canvas_stroke_contains(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	float v[2];

	if (!canvas || !uc_pvg_args_num(vm, nargs, 0, 2, v))
		return NULL;

	return ucv_boolean_new(plutovg_canvas_stroke_contains(canvas, v[0],
		v[1]));
}

static uc_value_t *
uc_pvg_canvas_clip_contains(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	float v[2];

	if (!canvas || !uc_pvg_args_num(vm, nargs, 0, 2, v))
		return NULL;

	return ucv_boolean_new(plutovg_canvas_clip_contains(canvas, v[0], v[1]));
}

static uc_value_t *
uc_pvg_canvas_fill_extents(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	plutovg_rect_t rect;

	if (!canvas)
		return NULL;

	plutovg_canvas_fill_extents(canvas, &rect);

	return uc_pvg_rect_new(vm, &rect);
}

static uc_value_t *
uc_pvg_canvas_stroke_extents(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	plutovg_rect_t rect;

	if (!canvas)
		return NULL;

	plutovg_canvas_stroke_extents(canvas, &rect);

	return uc_pvg_rect_new(vm, &rect);
}

static uc_value_t *
uc_pvg_canvas_clip_extents(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	plutovg_rect_t rect;

	if (!canvas)
		return NULL;

	plutovg_canvas_clip_extents(canvas, &rect);

	return uc_pvg_rect_new(vm, &rect);
}

static uc_value_t *
uc_pvg_canvas_fill(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);

	if (!canvas)
		return NULL;

	plutovg_canvas_fill(canvas);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_canvas_stroke(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);

	if (!canvas)
		return NULL;

	plutovg_canvas_stroke(canvas);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_canvas_clip(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);

	if (!canvas)
		return NULL;

	plutovg_canvas_clip(canvas);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_canvas_paint(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);

	if (!canvas)
		return NULL;

	plutovg_canvas_paint(canvas);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_canvas_fill_preserve(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);

	if (!canvas)
		return NULL;

	plutovg_canvas_fill_preserve(canvas);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_canvas_stroke_preserve(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);

	if (!canvas)
		return NULL;

	plutovg_canvas_stroke_preserve(canvas);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_canvas_clip_preserve(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);

	if (!canvas)
		return NULL;

	plutovg_canvas_clip_preserve(canvas);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_canvas_fill_rect(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	float v[4];

	if (!canvas || !uc_pvg_args_num(vm, nargs, 0, 4, v))
		return NULL;

	plutovg_canvas_fill_rect(canvas, v[0], v[1], v[2], v[3]);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_canvas_stroke_rect(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	float v[4];

	if (!canvas || !uc_pvg_args_num(vm, nargs, 0, 4, v))
		return NULL;

	plutovg_canvas_stroke_rect(canvas, v[0], v[1], v[2], v[3]);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_canvas_clip_rect(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	float v[4];

	if (!canvas || !uc_pvg_args_num(vm, nargs, 0, 4, v))
		return NULL;

	plutovg_canvas_clip_rect(canvas, v[0], v[1], v[2], v[3]);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_canvas_fill_path(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	plutovg_path_t *path;

	if (!canvas)
		return NULL;

	path = uc_pvg_arg_res(vm, uc_fn_arg(0), UC_PVG_PATH, "path");

	if (!path)
		return NULL;

	plutovg_canvas_fill_path(canvas, path);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_canvas_stroke_path(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	plutovg_path_t *path;

	if (!canvas)
		return NULL;

	path = uc_pvg_arg_res(vm, uc_fn_arg(0), UC_PVG_PATH, "path");

	if (!path)
		return NULL;

	plutovg_canvas_stroke_path(canvas, path);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_canvas_clip_path(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	plutovg_path_t *path;

	if (!canvas)
		return NULL;

	path = uc_pvg_arg_res(vm, uc_fn_arg(0), UC_PVG_PATH, "path");

	if (!path)
		return NULL;

	plutovg_canvas_clip_path(canvas, path);

	return ucv_boolean_new(true);
}

static uc_value_t *
uc_pvg_canvas_add_glyph(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	float v[3];

	if (!canvas || !uc_pvg_args_num(vm, nargs, 0, 3, v))
		return NULL;

	return ucv_double_new(plutovg_canvas_add_glyph(canvas,
		(plutovg_codepoint_t)v[0], v[1], v[2]));
}

static float
uc_pvg_canvas_text_op(uc_vm_t *vm, size_t nargs, plutovg_canvas_t *canvas,
                      int op, bool *ok)
{
	uc_value_t *text = _uc_fn_arg(vm, nargs, 0);
	const char *str;
	float v[2];
	int len;

	*ok = false;

	if (ucv_type(text) != UC_STRING) {
		uc_vm_raise_exception(vm, EXCEPTION_TYPE,
			"Expecting text string");

		return 0;
	}

	if (!uc_pvg_args_num(vm, nargs, 1, 2, v))
		return 0;

	str = ucv_string_get(text);
	len = (int)ucv_string_length(text);
	*ok = true;

	if (op == 0)
		return plutovg_canvas_add_text(canvas, str, len,
			PLUTOVG_TEXT_ENCODING_UTF8, v[0], v[1]);

	if (op == 1)
		return plutovg_canvas_fill_text(canvas, str, len,
			PLUTOVG_TEXT_ENCODING_UTF8, v[0], v[1]);

	if (op == 2)
		return plutovg_canvas_stroke_text(canvas, str, len,
			PLUTOVG_TEXT_ENCODING_UTF8, v[0], v[1]);

	return plutovg_canvas_clip_text(canvas, str, len,
		PLUTOVG_TEXT_ENCODING_UTF8, v[0], v[1]);
}

static uc_value_t *
uc_pvg_canvas_add_text(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	float advance;
	bool ok;

	if (!canvas)
		return NULL;

	advance = uc_pvg_canvas_text_op(vm, nargs, canvas, 0, &ok);

	return ok ? ucv_double_new(advance) : NULL;
}

static uc_value_t *
uc_pvg_canvas_fill_text(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	float advance;
	bool ok;

	if (!canvas)
		return NULL;

	advance = uc_pvg_canvas_text_op(vm, nargs, canvas, 1, &ok);

	return ok ? ucv_double_new(advance) : NULL;
}

static uc_value_t *
uc_pvg_canvas_stroke_text(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	float advance;
	bool ok;

	if (!canvas)
		return NULL;

	advance = uc_pvg_canvas_text_op(vm, nargs, canvas, 2, &ok);

	return ok ? ucv_double_new(advance) : NULL;
}

static uc_value_t *
uc_pvg_canvas_clip_text(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	float advance;
	bool ok;

	if (!canvas)
		return NULL;

	advance = uc_pvg_canvas_text_op(vm, nargs, canvas, 3, &ok);

	return ok ? ucv_double_new(advance) : NULL;
}

static uc_value_t *
uc_pvg_canvas_font_metrics(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	float ascent, descent, line_gap;
	plutovg_rect_t extents;
	uc_value_t *rv;

	if (!canvas)
		return NULL;

	plutovg_canvas_font_metrics(canvas, &ascent, &descent, &line_gap,
		&extents);

	rv = ucv_object_new(vm);
	ucv_object_add(rv, "ascent", ucv_double_new(ascent));
	ucv_object_add(rv, "descent", ucv_double_new(descent));
	ucv_object_add(rv, "line_gap", ucv_double_new(line_gap));
	ucv_object_add(rv, "extents", uc_pvg_rect_new(vm, &extents));

	return rv;
}

static uc_value_t *
uc_pvg_canvas_glyph_metrics(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	float advance_width, left_side_bearing;
	plutovg_rect_t extents;
	uc_value_t *rv;
	float v[1];

	if (!canvas || !uc_pvg_args_num(vm, nargs, 0, 1, v))
		return NULL;

	plutovg_canvas_glyph_metrics(canvas, (plutovg_codepoint_t)v[0],
		&advance_width, &left_side_bearing, &extents);

	rv = ucv_object_new(vm);
	ucv_object_add(rv, "advance_width", ucv_double_new(advance_width));
	ucv_object_add(rv, "left_side_bearing",
		ucv_double_new(left_side_bearing));
	ucv_object_add(rv, "extents", uc_pvg_rect_new(vm, &extents));

	return rv;
}

static uc_value_t *
uc_pvg_canvas_text_extents(uc_vm_t *vm, size_t nargs)
{
	plutovg_canvas_t *canvas = uc_fn_thisval(UC_PVG_CANVAS);
	uc_value_t *text = uc_fn_arg(0);
	plutovg_rect_t extents;
	uc_value_t *rv;
	float advance;

	if (!canvas)
		return NULL;

	if (ucv_type(text) != UC_STRING) {
		uc_vm_raise_exception(vm, EXCEPTION_TYPE,
			"Expecting text string");

		return NULL;
	}

	advance = plutovg_canvas_text_extents(canvas, ucv_string_get(text),
		(int)ucv_string_length(text), PLUTOVG_TEXT_ENCODING_UTF8,
		&extents);

	rv = ucv_object_new(vm);
	ucv_object_add(rv, "advance_width", ucv_double_new(advance));
	ucv_object_add(rv, "extents", uc_pvg_rect_new(vm, &extents));

	return rv;
}

static const uc_function_list_t surface_fns[] = {
	{ "width",		uc_pvg_surface_width },
	{ "height",		uc_pvg_surface_height },
	{ "stride",		uc_pvg_surface_stride },
	{ "clear",		uc_pvg_surface_clear },
	{ "data",		uc_pvg_surface_data },
	{ "rgba",		uc_pvg_surface_rgba },
	{ "write_png",		uc_pvg_surface_write_png },
	{ "write_jpg",		uc_pvg_surface_write_jpg },
	{ "png",		uc_pvg_surface_png },
	{ "jpg",		uc_pvg_surface_jpg },
};

static const uc_function_list_t path_fns[] = {
	{ "move_to",		uc_pvg_path_move_to },
	{ "line_to",		uc_pvg_path_line_to },
	{ "quad_to",		uc_pvg_path_quad_to },
	{ "cubic_to",		uc_pvg_path_cubic_to },
	{ "arc_to",		uc_pvg_path_arc_to },
	{ "close",		uc_pvg_path_close },
	{ "reset",		uc_pvg_path_reset },
	{ "reserve",		uc_pvg_path_reserve },
	{ "current_point",	uc_pvg_path_current_point },
	{ "add_rect",		uc_pvg_path_add_rect },
	{ "add_round_rect",	uc_pvg_path_add_round_rect },
	{ "add_ellipse",	uc_pvg_path_add_ellipse },
	{ "add_circle",		uc_pvg_path_add_circle },
	{ "add_arc",		uc_pvg_path_add_arc },
	{ "add_path",		uc_pvg_path_add_path },
	{ "transform",		uc_pvg_path_transform },
	{ "clone",		uc_pvg_path_clone },
	{ "clone_flatten",	uc_pvg_path_clone_flatten },
	{ "clone_dashed",	uc_pvg_path_clone_dashed },
	{ "extents",		uc_pvg_path_extents },
	{ "length",		uc_pvg_path_length },
	{ "parse",		uc_pvg_path_parse },
	{ "elements",		uc_pvg_path_elements },
};

static const uc_function_list_t font_face_fns[] = {
	{ "metrics",		uc_pvg_font_face_metrics },
	{ "glyph_metrics",	uc_pvg_font_face_glyph_metrics },
	{ "glyph_path",		uc_pvg_font_face_glyph_path },
	{ "text_extents",	uc_pvg_font_face_text_extents },
};

static const uc_function_list_t canvas_fns[] = {
	{ "surface",		uc_pvg_canvas_surface },
	{ "save",		uc_pvg_canvas_save },
	{ "restore",		uc_pvg_canvas_restore },
	{ "set_color",		uc_pvg_canvas_set_color },
	{ "color",		uc_pvg_canvas_color },
	{ "set_paint",		uc_pvg_canvas_set_paint },
	{ "get_paint",		uc_pvg_canvas_get_paint },
	{ "set_linear_gradient", uc_pvg_canvas_set_linear_gradient },
	{ "set_radial_gradient", uc_pvg_canvas_set_radial_gradient },
	{ "set_texture",	uc_pvg_canvas_set_texture },
	{ "set_font",		uc_pvg_canvas_set_font },
	{ "set_font_face",	uc_pvg_canvas_set_font_face },
	{ "font_face",		uc_pvg_canvas_font_face },
	{ "set_font_size",	uc_pvg_canvas_set_font_size },
	{ "font_size",		uc_pvg_canvas_font_size },
	{ "add_font_face",	uc_pvg_canvas_add_font_face },
	{ "add_font_file",	uc_pvg_canvas_add_font_file },
	{ "select_font_face",	uc_pvg_canvas_select_font_face },
	{ "set_fill_rule",	uc_pvg_canvas_set_fill_rule },
	{ "fill_rule",		uc_pvg_canvas_fill_rule },
	{ "set_operator",	uc_pvg_canvas_set_operator },
	{ "operator",		uc_pvg_canvas_operator },
	{ "set_opacity",	uc_pvg_canvas_set_opacity },
	{ "opacity",		uc_pvg_canvas_opacity },
	{ "set_line_width",	uc_pvg_canvas_set_line_width },
	{ "line_width",		uc_pvg_canvas_line_width },
	{ "set_line_cap",	uc_pvg_canvas_set_line_cap },
	{ "line_cap",		uc_pvg_canvas_line_cap },
	{ "set_line_join",	uc_pvg_canvas_set_line_join },
	{ "line_join",		uc_pvg_canvas_line_join },
	{ "set_miter_limit",	uc_pvg_canvas_set_miter_limit },
	{ "miter_limit",	uc_pvg_canvas_miter_limit },
	{ "set_dash",		uc_pvg_canvas_set_dash },
	{ "set_dash_offset",	uc_pvg_canvas_set_dash_offset },
	{ "dash_offset",	uc_pvg_canvas_dash_offset },
	{ "set_dash_array",	uc_pvg_canvas_set_dash_array },
	{ "dash_array",		uc_pvg_canvas_dash_array },
	{ "translate",		uc_pvg_canvas_translate },
	{ "scale",		uc_pvg_canvas_scale },
	{ "shear",		uc_pvg_canvas_shear },
	{ "rotate",		uc_pvg_canvas_rotate },
	{ "transform",		uc_pvg_canvas_transform },
	{ "reset_matrix",	uc_pvg_canvas_reset_matrix },
	{ "set_matrix",		uc_pvg_canvas_set_matrix },
	{ "matrix",		uc_pvg_canvas_matrix },
	{ "map",		uc_pvg_canvas_map },
	{ "map_point",		uc_pvg_canvas_map_point },
	{ "map_rect",		uc_pvg_canvas_map_rect },
	{ "move_to",		uc_pvg_canvas_move_to },
	{ "line_to",		uc_pvg_canvas_line_to },
	{ "quad_to",		uc_pvg_canvas_quad_to },
	{ "cubic_to",		uc_pvg_canvas_cubic_to },
	{ "arc_to",		uc_pvg_canvas_arc_to },
	{ "rect",		uc_pvg_canvas_rect },
	{ "round_rect",		uc_pvg_canvas_round_rect },
	{ "ellipse",		uc_pvg_canvas_ellipse },
	{ "circle",		uc_pvg_canvas_circle },
	{ "arc",		uc_pvg_canvas_arc },
	{ "add_path",		uc_pvg_canvas_add_path },
	{ "new_path",		uc_pvg_canvas_new_path },
	{ "close_path",		uc_pvg_canvas_close_path },
	{ "current_point",	uc_pvg_canvas_current_point },
	{ "path",		uc_pvg_canvas_path },
	{ "fill_contains",	uc_pvg_canvas_fill_contains },
	{ "stroke_contains",	uc_pvg_canvas_stroke_contains },
	{ "clip_contains",	uc_pvg_canvas_clip_contains },
	{ "fill_extents",	uc_pvg_canvas_fill_extents },
	{ "stroke_extents",	uc_pvg_canvas_stroke_extents },
	{ "clip_extents",	uc_pvg_canvas_clip_extents },
	{ "fill",		uc_pvg_canvas_fill },
	{ "stroke",		uc_pvg_canvas_stroke },
	{ "clip",		uc_pvg_canvas_clip },
	{ "paint",		uc_pvg_canvas_paint },
	{ "fill_preserve",	uc_pvg_canvas_fill_preserve },
	{ "stroke_preserve",	uc_pvg_canvas_stroke_preserve },
	{ "clip_preserve",	uc_pvg_canvas_clip_preserve },
	{ "fill_rect",		uc_pvg_canvas_fill_rect },
	{ "stroke_rect",	uc_pvg_canvas_stroke_rect },
	{ "clip_rect",		uc_pvg_canvas_clip_rect },
	{ "fill_path",		uc_pvg_canvas_fill_path },
	{ "stroke_path",	uc_pvg_canvas_stroke_path },
	{ "clip_path",		uc_pvg_canvas_clip_path },
	{ "add_glyph",		uc_pvg_canvas_add_glyph },
	{ "add_text",		uc_pvg_canvas_add_text },
	{ "fill_text",		uc_pvg_canvas_fill_text },
	{ "stroke_text",	uc_pvg_canvas_stroke_text },
	{ "clip_text",		uc_pvg_canvas_clip_text },
	{ "font_metrics",	uc_pvg_canvas_font_metrics },
	{ "glyph_metrics",	uc_pvg_canvas_glyph_metrics },
	{ "text_extents",	uc_pvg_canvas_text_extents },
};

static const uc_function_list_t module_fns[] = {
	{ "version",		uc_pvg_version },
	{ "version_string",	uc_pvg_version_string },
	{ "deg2rad",		uc_pvg_deg2rad },
	{ "rad2deg",		uc_pvg_rad2deg },
	{ "color_parse",	uc_pvg_color_parse },
	{ "color_hsl",		uc_pvg_color_hsl },
	{ "color_rgba32",	uc_pvg_color_rgba32 },
	{ "color_argb32",	uc_pvg_color_argb32 },
	{ "matrix_identity",	uc_pvg_matrix_identity },
	{ "matrix_init_translate", uc_pvg_matrix_init_translate },
	{ "matrix_init_scale",	uc_pvg_matrix_init_scale },
	{ "matrix_init_rotate",	uc_pvg_matrix_init_rotate },
	{ "matrix_init_shear",	uc_pvg_matrix_init_shear },
	{ "matrix_translate",	uc_pvg_matrix_translate },
	{ "matrix_scale",	uc_pvg_matrix_scale },
	{ "matrix_rotate",	uc_pvg_matrix_rotate },
	{ "matrix_shear",	uc_pvg_matrix_shear },
	{ "matrix_multiply",	uc_pvg_matrix_multiply },
	{ "matrix_invert",	uc_pvg_matrix_invert },
	{ "matrix_map",		uc_pvg_matrix_map },
	{ "matrix_map_point",	uc_pvg_matrix_map_point },
	{ "matrix_map_rect",	uc_pvg_matrix_map_rect },
	{ "matrix_parse",	uc_pvg_matrix_parse },
	{ "surface",		uc_pvg_surface_create },
	{ "surface_from_file",	uc_pvg_surface_from_file },
	{ "surface_from_data",	uc_pvg_surface_from_data },
	{ "surface_from_base64", uc_pvg_surface_from_base64 },
	{ "canvas",		uc_pvg_canvas_create },
	{ "path",		uc_pvg_path_create },
	{ "paint_color",	uc_pvg_paint_color },
	{ "paint_linear_gradient", uc_pvg_paint_linear_gradient },
	{ "paint_radial_gradient", uc_pvg_paint_radial_gradient },
	{ "paint_texture",	uc_pvg_paint_texture },
	{ "font_face_from_file", uc_pvg_font_face_from_file },
	{ "font_face_from_data", uc_pvg_font_face_from_data },
};

void
uc_module_init(uc_vm_t *vm, uc_value_t *scope)
{
	uc_function_list_register(scope, module_fns);

	uc_type_declare(vm, UC_PVG_SURFACE, surface_fns, uc_pvg_surface_free);
	uc_type_declare(vm, UC_PVG_CANVAS, canvas_fns, uc_pvg_canvas_free);
	uc_type_declare(vm, UC_PVG_PATH, path_fns, uc_pvg_path_free);
	uc_type_declare(vm, UC_PVG_FONT_FACE, font_face_fns,
		uc_pvg_font_face_free);

	ucv_resource_type_add(vm, UC_PVG_PAINT, ucv_object_new(NULL),
		uc_pvg_paint_free);
}
