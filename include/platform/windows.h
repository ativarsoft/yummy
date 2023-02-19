/*
 *  yummy/include/platform.h
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#ifndef PLATFORM_WINDOWS_H
#define PLATFORM_WINDOWS_H

#include <windows.h>

#define ERRNO GetLastError()
#define DLLEXPORT __declspec(dllexport)

typedef HWND window_t;
typedef HMODULE library_t;
typedef HBITMAP pixmap_t;
typedef HCURSOR cursor_t;
typedef HMENU menu_t;

extern HINSTANCE instance;

static inline void * get_symbol(library_t lib, const char *sym)
{
	return GetProcAddress(lib, sym);
}

#endif
