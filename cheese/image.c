/*
 *  yummy/cheese/image.c
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#include <cheese_services.h>
#include <stdint.h>
#include <image.h>

/* FIXME: remove me. */
uint32_t * load_png(struct load_image_s *args);

uint32_t * load_image(struct load_image_s *args)
{
	return load_png(args);
}
