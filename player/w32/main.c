/*
 *  yummy/player/w32/main.c
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#include <windows.h>
#include <gui.h>
#include <skin.h>
#include "configurations.h"
#include "preferences.h"
#include "error.h"

extern struct gui gui_w32;
extern struct skin classic;
extern struct skin modern;

/* This is used by classic and modern skins. */
HWND main_window;

struct gui *gui;
struct skin *skin;

int CALLBACK WinMain(HINSTANCE instance, HINSTANCE previous, char *cmd_line, int cmd)
{
#if defined DEBUG && 0
	debug_check_config_length();
#endif
	/*load_config("Winamp.ini");*/
	
	gui = &gui_w32;
	skin = &classic;
	/*skin = &modern;*/
	
	gui->init();
	skin->init();
	/* these depend on the main window */
	tray_init(); /* Requires main window. */
	menus_init(); /* The menus are the same in classic and modern skins. */
	discovery_init(); /* Requires main window. */
	gui->event_handler();
	/*skin_classic(gui);*/
	/*preferences();*/
	/*pe_load("plugins/in_libav.dll");*/
	/*modern_skin("skins/Winamp Modern");*/
	/*discovery_init();*/
	
	/* TODO: destroy menus. */
	gui->quit();
	/*Sleep(10000);*/
	puts("bye");
	
	return 0;
}
