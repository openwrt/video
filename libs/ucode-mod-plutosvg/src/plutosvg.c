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

#include <plutosvg/plutosvg.h>
#include <uc-plutovg.h>

#define UC_PSVG_DOCUMENT "plutosvg.document"

typedef struct {
	uc_value_t *palette;
	uc_vm_t *vm;
} uc_psvg_palette_t;

static bool
uc_psvg_palette_lookup(uc_psvg_palette_t *ctx, const char *name, int length,
                       plutovg_color_t *color)
{
	uc_value_t *val;
	char *key;
	bool ok;

	key = strndup(name, (size_t)length);

	if (!key)
		return false;

	val = ucv_object_get(ctx->palette, key, NULL);
	ok = uc_pvg_color_get(val, color);
	free(key);

	return ok;
}

static bool
uc_psvg_palette_call(uc_psvg_palette_t *ctx, const char *name, int length,
                     plutovg_color_t *color)
{
	uc_value_t *rv;
	bool ok;

	uc_vm_stack_push(ctx->vm, ucv_get(ctx->palette));
	uc_vm_stack_push(ctx->vm, ucv_string_new_length(name, (size_t)length));

	if (uc_vm_call(ctx->vm, false, 1) != EXCEPTION_NONE)
		return false;

	rv = uc_vm_stack_pop(ctx->vm);
	ok = uc_pvg_color_get(rv, color);
	ucv_put(rv);

	return ok;
}

static bool
uc_psvg_palette_cb(void *closure, const char *name, int length,
                   plutovg_color_t *color)
{
	uc_psvg_palette_t *ctx = closure;

	if (ucv_type(ctx->palette) == UC_OBJECT)
		return uc_psvg_palette_lookup(ctx, name, length, color);

	if (ucv_is_callable(ctx->palette))
		return uc_psvg_palette_call(ctx, name, length, color);

	return false;
}

static void
uc_psvg_document_free(void *ptr)
{
	plutosvg_document_destroy(ptr);
}

/* a short string lives inside the variable holding the value */
static bool
uc_psvg_id_check(uc_vm_t *vm, uc_value_t *uv)
{
	if (uv == NULL || ucv_type(uv) == UC_NULL || ucv_type(uv) == UC_STRING)
		return true;

	uc_vm_raise_exception(vm, EXCEPTION_TYPE, "Expecting element id string");

	return false;
}

static const plutovg_color_t *
uc_psvg_color_opt(uc_vm_t *vm, uc_value_t *uv, plutovg_color_t *storage,
                  bool *ok)
{
	*ok = true;

	if (uv == NULL || ucv_type(uv) == UC_NULL)
		return NULL;

	if (uc_pvg_color_get(uv, storage))
		return storage;

	uc_vm_raise_exception(vm, EXCEPTION_TYPE, "Invalid color value");
	*ok = false;

	return NULL;
}

static plutosvg_palette_func_t
uc_psvg_palette_opt(uc_value_t *uv, uc_psvg_palette_t *ctx, uc_vm_t *vm)
{
	if (uv == NULL || ucv_type(uv) == UC_NULL)
		return NULL;

	if (ucv_type(uv) != UC_OBJECT && !ucv_is_callable(uv))
		return NULL;

	ctx->palette = uv;
	ctx->vm = vm;

	return uc_psvg_palette_cb;
}

static uc_value_t *
uc_psvg_version(uc_vm_t *vm, size_t nargs)
{
	return ucv_int64_new(plutosvg_version());
}

static uc_value_t *
uc_psvg_version_string(uc_vm_t *vm, size_t nargs)
{
	return ucv_string_new(plutosvg_version_string());
}

static uc_value_t *
uc_psvg_document_wrap(uc_vm_t *vm, plutosvg_document_t *doc)
{
	uc_value_t *rv;

	if (!doc)
		return NULL;

	rv = ucv_resource_create(vm, UC_PSVG_DOCUMENT, doc);

	if (!rv)
		plutosvg_document_destroy(doc);

	return rv;
}

static uc_value_t *
uc_psvg_document_from_data(uc_vm_t *vm, size_t nargs)
{
	uc_value_t *arg = uc_fn_arg(0);
	plutosvg_document_t *doc;
	size_t len;
	char *copy;

	if (ucv_type(arg) != UC_STRING) {
		uc_vm_raise_exception(vm, EXCEPTION_TYPE, "Expecting SVG data");

		return NULL;
	}

	len = ucv_string_length(arg);
	copy = malloc(len);

	if (!copy)
		return NULL;

	memcpy(copy, ucv_string_get(arg), len);

	doc = plutosvg_document_load_from_data(copy, (int)len,
		uc_pvg_number(uc_fn_arg(1), -1.0f),
		uc_pvg_number(uc_fn_arg(2), -1.0f), free, copy);

	if (!doc) {
		free(copy);

		return NULL;
	}

	return uc_psvg_document_wrap(vm, doc);
}

static uc_value_t *
uc_psvg_document_from_file(uc_vm_t *vm, size_t nargs)
{
	uc_value_t *arg = uc_fn_arg(0);

	if (ucv_type(arg) != UC_STRING) {
		uc_vm_raise_exception(vm, EXCEPTION_TYPE, "Expecting file name");

		return NULL;
	}

	return uc_psvg_document_wrap(vm,
		plutosvg_document_load_from_file(ucv_string_get(arg),
			uc_pvg_number(uc_fn_arg(1), -1.0f),
			uc_pvg_number(uc_fn_arg(2), -1.0f)));
}

static uc_value_t *
uc_psvg_document_width(uc_vm_t *vm, size_t nargs)
{
	plutosvg_document_t *doc = uc_fn_thisval(UC_PSVG_DOCUMENT);

	if (!doc)
		return NULL;

	return ucv_double_new(plutosvg_document_get_width(doc));
}

static uc_value_t *
uc_psvg_document_height(uc_vm_t *vm, size_t nargs)
{
	plutosvg_document_t *doc = uc_fn_thisval(UC_PSVG_DOCUMENT);

	if (!doc)
		return NULL;

	return ucv_double_new(plutosvg_document_get_height(doc));
}

static uc_value_t *
uc_psvg_document_extents(uc_vm_t *vm, size_t nargs)
{
	plutosvg_document_t *doc = uc_fn_thisval(UC_PSVG_DOCUMENT);
	uc_value_t *idval = uc_fn_arg(0);
	plutovg_rect_t rect;
	const char *id;

	if (!doc || !uc_psvg_id_check(vm, idval))
		return NULL;

	id = (ucv_type(idval) == UC_STRING) ? ucv_string_get(idval) : NULL;

	if (!plutosvg_document_extents(doc, id, &rect))
		return NULL;

	return uc_pvg_rect_new(vm, &rect);
}

static uc_value_t *
uc_psvg_document_render(uc_vm_t *vm, size_t nargs)
{
	plutosvg_document_t *doc = uc_fn_thisval(UC_PSVG_DOCUMENT);
	plutosvg_palette_func_t palette_func;
	uc_value_t *idval = uc_fn_arg(1);
	const plutovg_color_t *current;
	plutovg_canvas_t *canvas;
	uc_psvg_palette_t ctx;
	plutovg_color_t color;
	const char *id;
	bool ok;

	if (!doc || !uc_psvg_id_check(vm, idval))
		return NULL;

	canvas = ucv_resource_data(uc_fn_arg(0), UC_PVG_CANVAS);

	if (!canvas) {
		uc_vm_raise_exception(vm, EXCEPTION_TYPE,
			"Expecting plutovg canvas object");

		return NULL;
	}

	id = (ucv_type(idval) == UC_STRING) ? ucv_string_get(idval) : NULL;
	current = uc_psvg_color_opt(vm, uc_fn_arg(2), &color, &ok);

	if (!ok)
		return NULL;

	palette_func = uc_psvg_palette_opt(uc_fn_arg(3), &ctx, vm);

	return ucv_boolean_new(plutosvg_document_render(doc, id, canvas,
		current, palette_func, &ctx));
}

static uc_value_t *
uc_psvg_document_render_to_surface(uc_vm_t *vm, size_t nargs)
{
	plutosvg_document_t *doc = uc_fn_thisval(UC_PSVG_DOCUMENT);
	plutosvg_palette_func_t palette_func;
	uc_value_t *idval = uc_fn_arg(0);
	const plutovg_color_t *current;
	plutovg_surface_t *surface;
	uc_psvg_palette_t ctx;
	plutovg_color_t color;
	uc_value_t *rv, *name;
	const char *id;
	bool ok;

	if (!doc || !uc_psvg_id_check(vm, idval))
		return NULL;

	if (!ucv_resource_type_lookup(vm, UC_PVG_SURFACE)) {
		name = ucv_string_new("plutovg");
		ucv_put(uc_vm_invoke(vm, "require", 1, name));
		ucv_put(name);
	}

	if (!ucv_resource_type_lookup(vm, UC_PVG_SURFACE)) {
		uc_vm_raise_exception(vm, EXCEPTION_RUNTIME,
			"The plutovg module is required to create surfaces");

		return NULL;
	}

	id = (ucv_type(idval) == UC_STRING) ? ucv_string_get(idval) : NULL;
	current = uc_psvg_color_opt(vm, uc_fn_arg(3), &color, &ok);

	if (!ok)
		return NULL;

	palette_func = uc_psvg_palette_opt(uc_fn_arg(4), &ctx, vm);

	surface = plutosvg_document_render_to_surface(doc, id,
		(int)uc_pvg_number(uc_fn_arg(1), -1.0f),
		(int)uc_pvg_number(uc_fn_arg(2), -1.0f), current,
		palette_func, &ctx);

	if (!surface)
		return NULL;

	rv = ucv_resource_create(vm, UC_PVG_SURFACE, surface);

	if (!rv)
		plutovg_surface_destroy(surface);

	return rv;
}

static const uc_function_list_t document_fns[] = {
	{ "width",		uc_psvg_document_width },
	{ "height",		uc_psvg_document_height },
	{ "extents",		uc_psvg_document_extents },
	{ "render",		uc_psvg_document_render },
	{ "render_to_surface",	uc_psvg_document_render_to_surface },
};

static const uc_function_list_t module_fns[] = {
	{ "version",		uc_psvg_version },
	{ "version_string",	uc_psvg_version_string },
	{ "document",		uc_psvg_document_from_data },
	{ "document_from_file",	uc_psvg_document_from_file },
};

void
uc_module_init(uc_vm_t *vm, uc_value_t *scope)
{
	uc_function_list_register(scope, module_fns);

	uc_type_declare(vm, UC_PSVG_DOCUMENT, document_fns,
		uc_psvg_document_free);
}
