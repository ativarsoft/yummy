/*
 *  yummy/player/w32/tray.c
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#include <windows.h>
#include <config.h>

/* FIXME: remove me. */
extern HWND main_window;

void tray_init()
{
	NOTIFYICONDATA icondata;
	HINSTANCE instance;
	
	instance = GetModuleHandle(0);
	
	icondata.cbSize = sizeof(NOTIFYICONDATA);
	icondata.hWnd = main_window;
	icondata.uID = 0;
	icondata.uFlags = NIF_ICON | NIF_TIP;
	
	/* optional members */
	icondata.hIcon = LoadIcon(instance, MAKEINTRESOURCE(1));
	if (strlen(PACKAGE_NAME) > 128)
		return; /* FIXME: handle error. */
	strcpy(icondata.szTip, PACKAGE_NAME);
	
	Shell_NotifyIcon(NIM_ADD, &icondata);
}
