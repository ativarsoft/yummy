/*
 *  yummy/include/vfs.h
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#ifndef VFS_H
#define VFS_H

#include <stdint.h>

typedef uintptr_t vfs_file;

struct vfs_ops {
	vfs_file (*open)(const char *path);
	size_t (*read)(void *ptr, size_t size, size_t nmemb, vfs_file file);
	int (*close)(vfs_file file);
	int (*seek)(vfs_file stream, long offset, int whence);
};

uint16_t read16lsb(struct vfs_ops *ops, vfs_file f);
uint32_t read32lsb(struct vfs_ops *ops, vfs_file f);

extern struct vfs_ops stdops;

#endif
