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

typedef Window window_t;
typedef Pixmap pixmap_t;
typedef void * cursor_t;
typedef void * menu_t;
typedef void * library_t;

extern Display *display;
extern int screen;
extern Window root;
extern int black;
extern int white;

/*#define _NET_WM_STATE_ADD 1
#define _NET_WM_STATE_TOGGLE 2*/

extern Atom wm_delete_window;
/*extern Atom _net_wm_state;
extern Atom _net_wm_state_hidden;*/

static inline library_t load_library(const char *path)
{
	return dlopen(path, RTLD_LAZY);
}

static inline void * get_symbol(library_t lib, const char *sym)
{
	return dlsym(lib, sym);
}

#endif
