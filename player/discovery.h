/*
 *  yummy/player/discovery.h
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#ifndef DISCOVERY_H
#define DISCOVERY_H

#include "plugin.h"

struct plugin_list_entry {
	struct plugin_list_entry *previous;

	union {
		struct in_plugin_module *in;
		struct out_plugin_module *out;
		struct gen_plugin_module *gen;
		struct dsp_plugin_module *dsp;
		struct vis_plugin_module *vis;
	};
	library lib; /* if it is zero then it is unloaded. */

	/* the name will be shown on the preferences tab. */
	char *name;
	int name_len;
};

struct plugin_list_head {
	int num_plugins;
	struct plugin_list_entry *head;
};

extern struct plugin_list_head in_list;
extern struct plugin_list_head out_list;
extern struct plugin_list_head gen_list;
extern struct plugin_list_head dsp_list;
extern struct plugin_list_head vis_list;

extern struct in_plugin *cur_in;
extern struct out_plugin *cur_out;
extern struct dsp_plugin_module *cur_dsp;
extern struct vis_plugin_module *cur_vis;

#endif
