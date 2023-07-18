/*
 *  yummy/include/msgbox.h
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#ifndef MSGBOX_H
#define MSGBOX_H

#include <platform.h>

#ifdef _WIN32
#include <windows.h>

static inline void info(window_t window, char *msg)
{
	MessageBox(window, msg, 0, MB_ICONINFORMATION);
}

static inline void warning(window_t window, char *msg)
{
	MessageBox(window, msg, 0, MB_ICONWARNING);
}
#else
#include <stdio.h>

static inline void info(window_t window, char *msg)
{
	printf("INFO: %s", msg);
}

static inline void warning(window_t window, char *msg)
{
	printf("WARNING: %s", msg);
}
#endif

#endif
