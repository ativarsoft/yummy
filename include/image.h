/*
 *  yummy/include/image.h
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>

struct load_image_s {
	void *data;
	int length;
	int *w;
	int *h;
};

uint32_t * load_image(struct load_image_s *args);

#endif
