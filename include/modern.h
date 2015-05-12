/*
 *  yummy/include/modern.h
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#ifndef GUI_H
#define GUI_H

#include <platform.h>

/*struct rectangle_t {
	int x, y;
	int w, h;
};*/

struct list_accelerators {
};

/**********************************************/

struct list_bitmap {
	struct list_bitmap *next;
	pixmap_t pixmap;
	int w, h;
};

/*struct list_elements {
	struct list_elements *next;
	struct list_bitmap *bitmap;
};*/

/**********************************************/

struct list_layer {
	struct list_layer *next;
	struct layer *data;
};

struct list_groupdef {
	struct list_groupdef *next;
	struct list_layer *layer;
	struct groupdef *data;
};

/**********************************************/

struct list_group {
	struct list_group *next;
	struct list_groupdef *groupdef;
};

struct list_layout {
	struct list_layout *next;
	struct list_group *group;
};

struct list_container {
	struct list_container *next;
	struct list_layout *layout;
	window_t window;
};

struct skin {
	struct {
		char *author;
	} info;
	struct list_accelerators *list_accelerators;
	struct list_bitmap *bitmap;
	struct list_groupdef *list_groupdefs;
	struct list_container *list_container;
};

extern struct skin cur_skin;

#endif
