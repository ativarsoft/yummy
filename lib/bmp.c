/*
 *  yummy/lib/bmp.c
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <image.h>
#include <string.h>

int load_bmp(struct vfs_ops *ops, const char *path, struct image *out)
{
	vfs_file f;

	uint16_t signature;
	uint32_t file_size;
	uint32_t off_pixels;

	/* DIB header */
	uint32_t header_size;
	uint32_t width;
	uint32_t height;
	uint16_t num_planes;
	uint16_t bpp;
	uint32_t compression;
	uint32_t image_size;

	f = ops->open(path);
	if (f == 0) {
		fprintf(stderr, "ERROR: could not open file '%s'.\n", path);
		return 0;
	}
	ops->read(&signature, 2, 1, f);
	file_size = read32lsb(ops, f);
	(void) read16lsb(ops, f); /* reserved 1 */
	(void) read16lsb(ops, f); /* reserved 2 */
	off_pixels = read32lsb(ops, f);

	header_size = read32lsb(ops, f);
	if (header_size == 12) {
		/* OS/2 BMP */
		width = (uint32_t) read16lsb(ops, f);
		height = (uint32_t) read16lsb(ops, f);
		num_planes =  read16lsb(ops, f);
		bpp = read16lsb(ops, f);
	} else if (header_size > 12) {
		/* Window BMP */
		width = read32lsb(ops, f);
		height = read32lsb(ops, f);
		num_planes = read16lsb(ops, f);
		bpp = read16lsb(ops, f);
		compression = read32lsb(ops, f);
		image_size = read32lsb(ops, f);
	} else {
		fprintf(stderr, "ERROR: invalid BMP.\n");
		return 0;
	}
	out->width = width;
	out->height = height;
	bpp = 32; /* FIXME: properly convert image depth. */
	image_size = width * height * bpp / 8;
	out->bpp = bpp;
	out->pixels = (char *) malloc(image_size);
	if (out->pixels == NULL) {
		fprintf(stderr, "ERROR: not enough memory.\n");
		goto out;
	}
	(void) ops->seek(f, off_pixels, SEEK_SET);
	(void) ops->read(out->pixels, image_size, 1, f);
out:
	ops->close(f);
	return 1;
}
