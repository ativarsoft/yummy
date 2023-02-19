/*
 *  yummy/include/gui.h
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#ifndef GUI_H
#define GUI_H

#include <stdint.h>
#include <platform.h>
#include <debug.h>
#include <stdbool.h>

struct skin_callbacks {
	void (*mouse_down)(int x, int y);
	void (*mouse_up)(int x, int y);
	void (*mouse_move)(int x, int y);
	void (*draw)();
	void (*resize)(int w, int h);
	void (*focus)(bool focus);
};

struct gui {
	void (*init)(void);
	void (*quit)(void);

	window_t (*create_window)(int x, int y, int w, int h, char *title, const struct skin_callbacks *callbacks);
	void (*destroy_window)(window_t);
	void (*event_handler)();
	void (*show_window)(window_t);
	void (*hide_window)(window_t);
	void (*move_window)(window_t w, int x, int y);
	void (*redraw_window)(window_t w);
	void (*set_topmost)(window_t window);
	void (*set_not_topmost)(window_t window);
	void (*resize_window)(window_t window, int w, int h);
	void (*get_window_rect)(int *rect);
	void (*minimize_window)(window_t w);

	pixmap_t (*load_image)(const char *filename);
	void (*unload_image)(pixmap_t image);
	void (*begin_drawing)(window_t w);
	void (*draw_image)(pixmap_t pixmap, int dst_x, int dst_y, int w, int h, int src_x, int src_y);
	void (*draw_image_double)(pixmap_t pixmap, int dst_x, int dst_y, int w, int h, int src_x, int src_y);
	void (*draw_filled_rectangle)(int x, int y, int w, int h, char *color);
	void (*end_drawing)();

	void (*capture_mouse)(window_t w);
	void (*release_mouse)(void);
	cursor_t (*load_cursor)();
	void (*unload_cursor)(cursor_t cursor);
	void (*set_cursor)(cursor_t cursor);

	bool (*check_glue)(window_t a, window_t b, int x, int y);

	void (*open_file_dialog)();
	void (*open_dir_dialog)();
};

extern struct skin cur_skin;
extern struct gui *gui;
extern window_t main_window;

#endif
