/*
 *  yummy/include/menus.h
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#ifndef MENUS_H
#define MENUS_H

#include <platform.h>

extern menu menu_main;
extern menu menu_presets;
extern menu menu_playlist;
extern menu menu_system;

void menus_init(void);
void menus_quit(void);
void menus_show(menu m, int x, int y);

#endif
