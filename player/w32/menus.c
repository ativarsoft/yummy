/*
 *  yummy/player/w32/menus.c
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#include <windows.h>
#include "menus.h"
#include "gui.h"

menu_t menu_root;

menu_t menu_main;
menu_t menu_presets;
menu_t menu_playlist;
menu_t menu_system;

void menus_show(menu_t m, int x, int y)
{
	TrackPopupMenu(m, 0, x, y, 0, main_window, 0);
}

void menus_init(void)
{
	/* The menus are the same in classic and modern skins. */
	menu_root = LoadMenu(NULL, MAKEINTRESOURCE(1));
	menu_main = GetSubMenu(menu_root, 0);
	menu_presets = GetSubMenu(menu_root, 1);
	/*menu_default_menu = CreatePopupMenu();
	menu_presets = CreatePopupMenu();
	menu_presets_load = CreatePopupMenu();
	menu_presets_save = CreatePopupMenu();
	menu_presets_delete = CreatePopupMenu();
	menu_playlist = CreatePopupMenu();
	create_menu(menu_default_menu, menus_default_data);
	create_menu(menu_presets, menus_presets_data);
	create_menu(menu_playlist, menus_presets_data);
	create_menu(menu_playlist, menus_presets_load_data);
	create_menu(menu_playlist, menus_presets_save_data);
	create_menu(menu_playlist, menus_presets_delete_data);
	create_menu(menu_playlist, menus_playlist_data);
	menus.system = GetSystemMenu(main_window, 0);
	InsertMenu(menu_system, SC_RESTORE, MF_POPUP, (UINT_PTR) menus.default_menu, "Modular player");
	InsertMenu(menu_system, SC_RESTORE, MF_SEPARATOR, 0, 0);*/
}

void menus_quit(void)
{
	/*DestroyMenu(menu_default_menu);
	DestroyMenu(menu_presets);
	DestroyMenu(menu_presets_load);
	DestroyMenu(menu_presets_save);
	DestroyMenu(menu_presets_delete);
	DestroyMenu(menu_playlist);*/
}
