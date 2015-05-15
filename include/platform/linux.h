/*
 *  yummy/include/platform/linux.h
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#ifndef PLATFORM_LINUX_H
#define PLATFORM_LINUX_H

#include <X11/Xlib.h>
#include <dlfcn.h>
#include <errno.h>

#define ERRNO errno
#define GetLastError() 0 /* FIXME: remove. */
#define DLLEXPORT /* All symbols are exported by default in Linux. */

typedef int window;
typedef void * library;
typedef Pixmap pixmap;
typedef void * cursor;
typedef void * menu;

static inline library load_library(const char *path)
{
	return dlopen(path, RTLD_LAZY);
}

static inline void * get_symbol(library lib, const char *sym)
{
	return dlsym(lib, sym);
}

#endif
