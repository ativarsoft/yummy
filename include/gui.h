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

	window (*create_window)(int x, int y, int w, int h, char *title, const struct skin_callbacks *callbacks);
	void (*destroy_window)(window);
	void (*event_handler)();
	void (*show_window)(window);
	void (*hide_window)(window);
	void (*move_window)(window w, int x, int y);
	void (*redraw_window)(window w);
	void (*set_topmost)(window window);
	void (*set_not_topmost)(window window);
	void (*resize_window)(window window, int w, int h);
	void (*get_window_rect)(int *rect);
	void (*minimize_window)(window w);

	pixmap (*load_image)(const char *filename);
	void (*unload_image)(pixmap image);
	void (*begin_drawing)(window w);
	void (*draw_image)(pixmap pixmap, int dst_x, int dst_y, int w, int h, int src_x, int src_y);
	void (*draw_image_double)(pixmap pixmap, int dst_x, int dst_y, int w, int h, int src_x, int src_y);
	void (*draw_filled_rectangle)(int x, int y, int w, int h, char *color);
	void (*end_drawing)();

	void (*capture_mouse)(window w);
	void (*release_mouse)(void);
	cursor (*load_cursor)();
	void (*unload_cursor)(cursor cursor);
	void (*set_cursor)(cursor cursor);

	bool (*check_glue)(window a, window b, int x, int y);

	void (*open_file_dialog)();
	void (*open_dir_dialog)();
};

extern struct skin cur_skin;
extern struct gui *gui;
extern window main_window;

#endif
