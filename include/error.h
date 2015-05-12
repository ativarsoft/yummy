/*
 *  yummy/include/error.h
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#ifndef ERROR_H
#define ERROR_H

void error(int a, int b, const char *c, ...);

/* FIXME: transitional. */
#define error_ok error

#endif
