/*
 *  yummy/lib/vfs.c
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#include <stdio.h>
#include <vfs.h>

uint16_t read16lsb(struct vfs_ops *ops, vfs_file f)
{
	uint16_t data;

	ops->read(&data, 2, 1, f);
	return data;
}

uint32_t read32lsb(struct vfs_ops *ops, vfs_file f)
{
	uint32_t data;

	ops->read(&data, 4, 1, f);
	return data;
}
