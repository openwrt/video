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

#ifndef UC_PLUTOVG_H
#define UC_PLUTOVG_H

#include <plutovg/plutovg.h>
#include <ucode/module.h>

#define UC_PVG_SURFACE   "plutovg.surface"
#define UC_PVG_CANVAS    "plutovg.canvas"
#define UC_PVG_PATH      "plutovg.path"
#define UC_PVG_PAINT     "plutovg.paint"
#define UC_PVG_FONT_FACE "plutovg.font_face"

static inline float
uc_pvg_number(uc_value_t *uv, float fallback)
{
	if (ucv_type(uv) != UC_INTEGER && ucv_type(uv) != UC_DOUBLE)
		return fallback;

	return (float)ucv_to_double(uv);
}

static inline bool
uc_pvg_component(uc_value_t *uv, const char *key, size_t idx, float *out)
{
	uc_value_t *val;

	if (ucv_type(uv) == UC_OBJECT)
		val = ucv_object_get(uv, key, NULL);
	else
		val = ucv_array_get(uv, idx);

	if (ucv_type(val) != UC_INTEGER && ucv_type(val) != UC_DOUBLE)
		return false;

	*out = (float)ucv_to_double(val);

	return true;
}

static inline bool
uc_pvg_color_get(uc_value_t *uv, plutovg_color_t *color)
{
	uc_value_t *alpha;

	if (ucv_type(uv) == UC_STRING)
		return plutovg_color_parse(color, ucv_string_get(uv),
			ucv_string_length(uv)) > 0;

	if (ucv_type(uv) != UC_OBJECT && ucv_type(uv) != UC_ARRAY)
		return false;

	if (!uc_pvg_component(uv, "r", 0, &color->r))
		return false;
	if (!uc_pvg_component(uv, "g", 1, &color->g))
		return false;
	if (!uc_pvg_component(uv, "b", 2, &color->b))
		return false;

	if (ucv_type(uv) == UC_OBJECT)
		alpha = ucv_object_get(uv, "a", NULL);
	else
		alpha = ucv_array_get(uv, 3);

	color->a = uc_pvg_number(alpha, 1.0f);

	return true;
}

static inline uc_value_t *
uc_pvg_color_new(uc_vm_t *vm, const plutovg_color_t *color)
{
	uc_value_t *rv = ucv_object_new(vm);

	ucv_object_add(rv, "r", ucv_double_new(color->r));
	ucv_object_add(rv, "g", ucv_double_new(color->g));
	ucv_object_add(rv, "b", ucv_double_new(color->b));
	ucv_object_add(rv, "a", ucv_double_new(color->a));

	return rv;
}

static inline bool
uc_pvg_rect_get(uc_value_t *uv, plutovg_rect_t *rect)
{
	if (ucv_type(uv) != UC_OBJECT && ucv_type(uv) != UC_ARRAY)
		return false;

	return uc_pvg_component(uv, "x", 0, &rect->x) &&
	       uc_pvg_component(uv, "y", 1, &rect->y) &&
	       uc_pvg_component(uv, "w", 2, &rect->w) &&
	       uc_pvg_component(uv, "h", 3, &rect->h);
}

static inline uc_value_t *
uc_pvg_rect_new(uc_vm_t *vm, const plutovg_rect_t *rect)
{
	uc_value_t *rv = ucv_object_new(vm);

	ucv_object_add(rv, "x", ucv_double_new(rect->x));
	ucv_object_add(rv, "y", ucv_double_new(rect->y));
	ucv_object_add(rv, "w", ucv_double_new(rect->w));
	ucv_object_add(rv, "h", ucv_double_new(rect->h));

	return rv;
}

static inline bool
uc_pvg_point_get(uc_value_t *uv, plutovg_point_t *point)
{
	if (ucv_type(uv) != UC_OBJECT && ucv_type(uv) != UC_ARRAY)
		return false;

	return uc_pvg_component(uv, "x", 0, &point->x) &&
	       uc_pvg_component(uv, "y", 1, &point->y);
}

static inline uc_value_t *
uc_pvg_point_new(uc_vm_t *vm, const plutovg_point_t *point)
{
	uc_value_t *rv = ucv_object_new(vm);

	ucv_object_add(rv, "x", ucv_double_new(point->x));
	ucv_object_add(rv, "y", ucv_double_new(point->y));

	return rv;
}

static inline bool
uc_pvg_matrix_get(uc_value_t *uv, plutovg_matrix_t *matrix)
{
	if (ucv_type(uv) == UC_STRING)
		return plutovg_matrix_parse(matrix, ucv_string_get(uv),
			ucv_string_length(uv));

	if (ucv_type(uv) != UC_OBJECT && ucv_type(uv) != UC_ARRAY)
		return false;

	return uc_pvg_component(uv, "a", 0, &matrix->a) &&
	       uc_pvg_component(uv, "b", 1, &matrix->b) &&
	       uc_pvg_component(uv, "c", 2, &matrix->c) &&
	       uc_pvg_component(uv, "d", 3, &matrix->d) &&
	       uc_pvg_component(uv, "e", 4, &matrix->e) &&
	       uc_pvg_component(uv, "f", 5, &matrix->f);
}

static inline uc_value_t *
uc_pvg_matrix_new(uc_vm_t *vm, const plutovg_matrix_t *matrix)
{
	uc_value_t *rv = ucv_object_new(vm);

	ucv_object_add(rv, "a", ucv_double_new(matrix->a));
	ucv_object_add(rv, "b", ucv_double_new(matrix->b));
	ucv_object_add(rv, "c", ucv_double_new(matrix->c));
	ucv_object_add(rv, "d", ucv_double_new(matrix->d));
	ucv_object_add(rv, "e", ucv_double_new(matrix->e));
	ucv_object_add(rv, "f", ucv_double_new(matrix->f));

	return rv;
}

#endif
