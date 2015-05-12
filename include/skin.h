/*
 *  yummy/include/skin.h
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#ifndef SKIN_H
#define SKIN_H

struct skin {
	void (*init)(void);
	void (*quit)(void);
	void (*set_title)(char *title);
	void (*set_time)(int minutes, int seconds);
	void (*set_bitrate)(int value);
	void (*set_freq)(int value);
};

#endif
