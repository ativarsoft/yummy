/*
 *  yummy/lib/ops/stdops.c
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#include <stdio.h>

#include <vfs.h>

static vfs_file stdops_open(const char *path)
{
	return (vfs_file) fopen(path, "rb");
}

static size_t stdops_read(void *ptr, size_t size, size_t nmemb, vfs_file file)
{
	return fread(ptr, size, nmemb, (FILE *) file);
}

static int stdops_close(vfs_file file)
{
	return fclose((FILE *) file);
}

static int stdops_seek(vfs_file stream, long offset, int whence)
{
	return fseek((FILE *) stream, offset, whence);
}

struct vfs_ops stdops = {
	.open = &stdops_open,
	.read = &stdops_read,
	.close = &stdops_close,
	.seek = &stdops_seek
};
