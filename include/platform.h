/*
 *  yummy/include/platform.h
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#ifndef PLATFORM_H
#define PLATFORM_H

#if defined(_WIN32) || defined(__CYGWIN__)
#include <platform/windows.h>
#elif defined __linux__
#include <platform/linux.h>
#else
#error Unknown platform
#endif

/* compiler */
#define PACKED __attribute__((packed))

#endif
