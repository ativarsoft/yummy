/*
 *  yummy/include/gen.h
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#ifndef GEN_H
#define GEN_H

#include <platform.h>

struct gen_plugin
{
	int version;
	char *description;
	int (*init)();
	void (*configure)();
	void (*quit)();
	window_t window;
	library_t dll_instance;
};

#endif
