/*
 *  yummy/plugins/lib/png.c
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#include <png.h>
#include <stdint.h>
#include <debug.h>
#include <image.h>
#include <stdlib.h>
#include <string.h>

struct png_context_s {
	void *data;
	int len;
	int pos;
};

static void read_callback(png_structp png, png_bytep data, png_size_t len)
{
	struct png_context_s *context;
	
	context = png_get_io_ptr(png);
	if (len > context->pos + context->len)
		len = context->len - context->pos;
	memcpy(data, context->data + context->pos, len);
	context->pos += len;
}

uint32_t * load_png(struct load_image_s *args)
{
	png_structp png;
	png_infop info;
	int color_type;
	void *rgb_data;
	png_bytep *pointers;
	int i;
	png_uint_32 w,h;
	int depth;
	struct png_context_s context;
	png_uint_32 stride;
	
	if(png_sig_cmp(args->data, 0, 8)) {
		DEBUG_PUTS("not png");
		return 0;
	}
	png = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
	info = png_create_info_struct(png);
	context.data = args->data;
	context.len = args->length;
	context.pos = 8;
	png_set_read_fn(png, &context, &read_callback);
	png_set_sig_bytes(png, 8);
	png_read_info(png, info);
	png_get_IHDR(png, info, &w, &h, &depth, &color_type, 0, 0, 0);
	DEBUG_PRINTF("w: %d\n", w);
	DEBUG_PRINTF("h: %d\n", h);
	switch (color_type) {
		case PNG_COLOR_TYPE_PALETTE:
		DEBUG_PUTS("pallete");
		png_set_palette_to_rgb(png);
		break;
		case PNG_COLOR_TYPE_RGB_ALPHA:
		DEBUG_PUTS("rgba");
		break;
		default:
		rgb_data = 0;
		goto out;
	};
	png_set_filler(png, 0xff, PNG_FILLER_AFTER);
	pointers = (png_bytep *) malloc(h * sizeof(*pointers));
	if (!pointers) {
		rgb_data = 0;
		goto out;
	}
	stride = png_get_rowbytes(png, info);
	rgb_data = malloc(stride * 4 * h);
	if (!rgb_data) {
		rgb_data = 0;
		goto out;
	}
	for (i = 0; i < h; i++)
		pointers[i] = rgb_data + w * 4 * i;
	png_read_image(png, pointers);
	*args->w = w;
	*args->h = h;
out:
	png_destroy_read_struct(&png, &info, 0);
	
	return rgb_data;
}
