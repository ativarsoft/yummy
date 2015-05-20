/*
 *  yummy/include/image.h
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>
#include <vfs.h>

/*
 * TODO: this structure holds a buffered file.
 * Update it to use ops instead.
 */
struct load_image {
	void *data;
	int length;
	int *w;
	int *h;
};

/*
 * Representation of an image in memory to be read by the gui engine.
 * The gui engine creates a pixmap.
 */
struct image {
	int width, height, bpp;
	char *pixels;
};

int load_bmp(struct vfs_ops *ops, const char *path, struct image *out);


#endif
