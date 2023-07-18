/*
 *  yummy/player/linux/gui.c
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h> /* for XA_STRING */
#include <string.h>

#include <platform.h>
#include <gui.h>
#include <image.h>
#include <vfs.h>

struct list_container_s *list_container;

Display *display;
int screen;
Window root;
int black;
int white;
Atom wm_delete_window;
Atom _net_wm_state;
Atom _net_wm_state_hidden;

Window cur_win;
GC gc;

void init(void)
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

void quit(void)
{
	XCloseDisplay(display);
}

static window_t create_window(int x, int y, int w, int h, char *title, const struct skin_callbacks *callbacks)
{
	window_t window;
	Visual *visual;
	XVisualInfo visual_info;
	unsigned long attributes_mask;
	XSetWindowAttributes attributes;
	Atom skin_callbacks_atom;
	const int bpp = 32;

	XMatchVisualInfo(display, screen, bpp, TrueColor, &visual_info);
	visual = visual_info.visual;

	attributes_mask = CWBackPixel | CWColormap | CWBorderPixel;
	attributes.colormap = XCreateColormap(display, root, visual, AllocNone);
	attributes.background_pixel = 0xff000000;
	attributes.border_pixel = 0xffffffff;
	/*window = XCreateSimpleWindow(display, root, x, y, w, h, 1, black, white);*/
	window = XCreateWindow(display, root, x, y, w, h, 0, bpp, InputOutput, visual, attributes_mask, &attributes);
	if (!window)
		;

	XSetStandardProperties(display, window, title, title, 0, 0, 0, 0);
	XSelectInput(display, window, ButtonPressMask | ExposureMask | StructureNotifyMask);
	XSetWMProtocols(display, window, &wm_delete_window, 1);

	/* Associate the skin information with the window. */
	skin_callbacks_atom = XInternAtom(display, "skin_callbacks", False);
	/* We want to store a pointer. So pass a pointer to the poiter. */
	XChangeProperty(display, window, skin_callbacks_atom, XA_STRING, 8, PropModeReplace, (const unsigned char *) &callbacks, sizeof(struct skin_callbacks *));
	XSync(display, False);

	return window;
}

static void destroy_window(window_t w)
{
	XDestroyWindow(display, w);
}

static void window_expose(XEvent *event)
{
	struct skin_callbacks **callbacks;
	Atom skin_callbacks_atom;
	Atom actual_type;
	int actual_format;
	unsigned long nitems, bytes_after;
	int r;

	puts(__FUNCTION__);
	skin_callbacks_atom = XInternAtom(display, "skin_callbacks", True);
	r = XGetWindowProperty(display, event->xclient.window, skin_callbacks_atom, 0, sizeof(struct skin_callbacks *), False, AnyPropertyType, &actual_type, &actual_format, &nitems, &bytes_after, (unsigned char **) &callbacks);
	if (r != Success) {
		fprintf(stderr, "ERROR: error getting window property.\n");
		return;
	}
	(*callbacks)->draw();
	XFree(callbacks);
	XFlush(display);
#if 0
	struct list_container *container;
	struct list_layer *layer;
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
}

static void event_handler(void)
{
	XEvent event;

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
			/*case GraphicsExpose:*/ /* This will crash the system. */
			case Expose:
				window_expose(&event);
			break;
		}
	}
}

static void show_window(window_t w)
{
	XMapWindow(display, w);
}

static void hide_window(window_t w)
{
	XUnmapWindow(display, w);
}

static void move_window(window_t w, int x, int y)
{
	XMoveWindow(display, w, x, y);
}

static void redraw_window(window_t w)
{
}

static void set_topmost(window_t window)
{
}

static void set_not_topmost(window_t window)
{
}

static void resize_window(window_t window, int w, int h)
{
	XResizeWindow(display, window, w, h);
}

static void get_window_rect(int *rect)
{
}

static void minimize_window(window_t w)
{
}

static pixmap_t load_image(const char *path)
{
	struct image image;
	Pixmap pixmap;
	XImage *ximage;

	/* NOTE: XReadBitmapFile only works for XBM files. */
	load_bmp(&stdops, path, &image);
	pixmap = XCreatePixmap(display, root, image.width, image.height, image.bpp);
	gc = XCreateGC(display, pixmap, 0, NULL);
	ximage = XCreateImage(display, CopyFromParent, image.bpp, ZPixmap, 0, image.pixels, image.width, image.height, 32, 0);
	XPutImage(display, pixmap, gc, ximage, 0, 0, 0, 0, image.width, image.height);
	XFreeGC(display, gc);
	return pixmap;
}

static void unload_image(pixmap_t image)
{
	XFreePixmap(display, image);
}

static void begin_drawing(window_t w)
{
	gc = XCreateGC(display, w, 0, NULL);
	if (gc < 0)
		fprintf(stderr, "ERROR: could not create GC.\n");
	cur_win = w;
}

static void draw_image(pixmap_t pixmap, int dst_x, int dst_y, int w, int h, int src_x, int src_y)
{
	/*XSetBackground(display, gc, 0xffffffff);
	XSetForeground(display, gc, 0xffffffff);
	XSetFillStyle(display, gc, FillSolid);*/
	/*XFillRectangle(display, cur_win, gc, dst_x, dst_y, w, h);*/
	XCopyArea(display, pixmap, cur_win, gc, 0, 0, w, h, dst_x, dst_y);
}

static void draw_image_double(pixmap_t pixmap, int dst_x, int dst_y, int w, int h, int src_x, int src_y)
{
}

static void draw_filled_rectangle(int x, int y, int w, int h, char *color)
{
}

static void end_drawing()
{
	XFreeGC(display, gc);
}


static void capture_mouse(window_t w)
{
}

static void release_mouse(void)
{
}

static cursor_t load_cursor()
{
}

static void unload_cursor(cursor_t cursor)
{
}

static void set_cursor(cursor_t cursor)
{
}


static bool check_glue(window_t a, window_t b, int x, int y)
{
}


static void open_file_dialog()
{
}

static void open_dir_dialog()
{
}


struct gui gui_x11 = {
	.init = &init,
	.quit = &quit,

	.create_window = &create_window,
	.destroy_window = &destroy_window,
	.event_handler = &event_handler,
	.show_window = &show_window,
	.hide_window = &hide_window,
	.move_window = &move_window,
	.redraw_window = &redraw_window,
	.set_topmost = &set_topmost,
	.set_not_topmost = &set_not_topmost,
	.resize_window = &resize_window,
	.get_window_rect = &get_window_rect,
	.minimize_window = &minimize_window,

	.load_image = &load_image,
	.unload_image = &unload_image,
	.begin_drawing = &begin_drawing,
	.draw_image = &draw_image,
	.draw_image_double = &draw_image_double,
	.draw_filled_rectangle = &draw_filled_rectangle,
	.end_drawing = &end_drawing,
	.capture_mouse = &capture_mouse,
	.release_mouse = &release_mouse,
	.load_cursor = &load_cursor,
	.unload_cursor = &unload_cursor,
	.set_cursor = &set_cursor,

	.check_glue = &check_glue,
	
	.open_file_dialog = &open_file_dialog,
	.open_dir_dialog = &open_dir_dialog
};
