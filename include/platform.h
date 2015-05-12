/*
 *  yummy/include/platform.h
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#ifndef PLATFORM_H
#define PLATFORM_H

#if defined(_WIN32) || defined(__CYGWIN__)
#include <windows.h>
#define ERRNO GetLastError()
typedef HWND window_t;
typedef HMODULE library_t;
typedef HBITMAP pixmap_t;
typedef HCURSOR cursor_t;
typedef HMENU menu;
extern HINSTANCE instance;
static inline void * get_symbol(library_t lib, const char *sym)
{
	return GetProcAddress(lib, sym);
}
#elif defined __posix__
#include <X11/Xlib.h>
#include <dlfcn.h>
#include <errno.h>
#define ERRNO errno
typedef int window_t;
typedef void * library_t;
typedef Pixmap pixmap_t;
static inline void * get_symbol(library_t lib, const char *sym)
{
	dlsym(lib, sym);
}
/* FIXME: remove. */
#define GetLastError() 0
#else
#error Unknown platform
#endif

/* compiler */
#define PACKED __attribute__((packed))

#endif
