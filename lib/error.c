/*
 *  yummy/plugins/lib/error.c
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#ifndef ERROR_H
#define ERROR_H

#ifdef _WIN32
#include <windows.h>
#ifdef DEBUG
#include <stdio.h>
#include <stdarg.h>
#endif

void error(int a, int b, const char *format, ...)
{
	char *buffer;
#ifdef DEBUG
	va_list args;
#endif
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS, 
		0, b, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &buffer, 0, 0);
#ifdef DEBUG
	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
	printf(": %s\n", buffer);
#endif
	MessageBox(NULL, buffer, 0, MB_ICONERROR | MB_OK);
}
#endif

#endif
