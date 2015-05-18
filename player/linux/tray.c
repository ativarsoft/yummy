/*
 *  yummy/player/linux/tray.c
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#include <X11/Xlib.h>
#include <string.h>

#include <platform.h>
#include <gui.h>

#define SYSTEM_TRAY_REQUEST_DOCK 0

void tray_init()
{
	XEvent event;
	Window systray;
	Window icon;
	Atom tray_atom;
	
	tray_atom = XInternAtom(display, "_NET_SYSTEM_TRAY_S0", False);
	if (!tray_atom)
		return;
	systray = XGetSelectionOwner(display, tray_atom);
	if (!systray)
		return;

	/*icon = gui->create_window();*/
	
	memset(&event, 0, sizeof(event));
	event.xclient.type = ClientMessage;
	event.xclient.window = systray;
	event.xclient.message_type = 0;
	event.xclient.format = 32;
	event.xclient.data.l[0] = 0;
	event.xclient.data.l[1] = SYSTEM_TRAY_REQUEST_DOCK;
	event.xclient.data.l[2] = icon;
	
	/*XSendEvent(display, systray, False, NoEventMask, False);*/
	XSync(display, False);
}
