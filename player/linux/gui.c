/*
 *  yummy/player/linux/gui.c
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <string.h>

struct list_container_s *list_container;

Display *display;
int screen;
Window root;
int black;
int white;
Atom wm_delete_window;
Atom _net_wm_state;
Atom _net_wm_state_hidden;

void gui_init(void)
{
	display = XOpenDisplay(0);
	if (!display)
		error(0, 0, "display");
	screen = DefaultScreen(display);
	root = RootWindow(display, screen);
	wm_delete_window = XInternAtom(display, "WM_DELETE_WINDOW", 0);
	/*_net_wm_state = XInternAtom(display, "_NET_WM_STATE", 0);
	_net_wm_state_hidden = XInternAtom(display, "_NET_WM_STATE_ADD", 0);*/
	list_container = 0; /* init list. */
}

void gui_quit(void)
{
	XCloseDisplay(display);
}

/*#define _NET_WM_STATE_ADD 1
#define _NET_WM_STATE_TOGGLE 2*/

extern Display *display;
extern int screen;
extern Window root;
extern int black;
extern int white;

extern Atom wm_delete_window;
/*extern Atom _net_wm_state;
extern Atom _net_wm_state_hidden;*/

static inline window_t gui_create_window(int x, int y, int w, int h, char *title)
{
	window_t window;
	Visual *visual;
	XVisualInfo visual_info;
	unsigned long attributes_mask;
	XSetWindowAttributes attributes;
	
	XMatchVisualInfo(display, screen, 32, TrueColor, &visual_info);
	visual = visual_info.visual;
	
	attributes_mask = CWBackPixel | CWColormap | CWBorderPixel;
	attributes.colormap = XCreateColormap(display, root, visual, AllocNone);
	attributes.background_pixel = 0;
	attributes.border_pixel = 0;
	
	black = BlackPixel(display, screen);
	white = WhitePixel(display, screen);
	
	/*window = XCreateSimpleWindow(display, root, x, y, w, h, 1, black, white);*/
	window = XCreateWindow(display, root, x, y, w, h, 0, 32, InputOutput, visual, attributes_mask, &attributes);
	if (!window)
		;
	XSetStandardProperties(display, window, title, title, 0, 0, 0, 0);
	XSelectInput(display, window, ButtonPressMask | ExposureMask | StructureNotifyMask);
	XSetWMProtocols(display, window, &wm_delete_window, 1);
	/*if (visible)*/
		XMapWindow(display, window);
#ifdef DEBUG
	XSync(display, False);
#endif
	
	return window;
}

static inline void gui_enter_loop(void)
{
	XEvent event;
	GC gc;
	XGCValues gc_values;
	struct list_container *container;
	struct list_layer *layer;
	
	for (;;) {
		XNextEvent(display, &event);
		switch (event.type) {
			case ClientMessage:
			if ((Atom) event.xclient.data.l[0] == wm_delete_window) {
				XWithdrawWindow(display, event.xclient.window, screen);
				DEBUG_PUTS("wm close");
			}
			break;
			case MapNotify:
			case Expose:
#if 0
			for (container = list_container; container; container = container->next) {
				if (container->window = event.xany.window)
					for (layer = container->layer; layer; layer = layer->next) {
						gc = XCreateGC(display, event.xany.window, 0, &gc_values);
						XSetBackground(display, gc, white);
						XSetForeground(display, gc, 0xff);
						XCopyPlane(display, layer->pixmap, event.xany.window, gc, 0, 0, layer->w, layer->h, 0, 0, 1);
						XSetForeground(display, gc, 0xff00);
						/*XCopyPlane(display, layer->pixmap, event.xany.window, gc, 0, 0, layer->w, layer->h, 0, 0, 2);*/
						XCopyArea(display, layer->pixmap, event.xany.window, gc, 0, 0, layer->w, layer->h, 0, 0);
					}
			}
#endif
			break;
		}
	}
}
