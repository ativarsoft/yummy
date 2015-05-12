/*
 *  yummy/include/guid.h
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#ifndef GUID_H
#define GUID_H

#include <stdint.h>

struct guid_s
{
	uint32_t a;
	uint16_t b;
	uint16_t c;
	uint8_t d[8];
};

#endif
