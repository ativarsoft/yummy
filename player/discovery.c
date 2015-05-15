/*
 *  yummy/player/discovery.c
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include <platform.h>
#include <error.h>
#include <transverse.h>
#include "discovery.h"
#include "plugin.h"

struct plugin_list_head in_list = {0, 0};
struct plugin_list_head out_list = {0, 0};
struct plugin_list_head gen_list = {0, 0};
struct plugin_list_head dsp_list = {0, 0};
struct plugin_list_head vis_list = {0, 0};

struct in_plugin *cur_in = NULL;
struct out_plugin *cur_out = NULL;
struct dsp_plugin_module *cur_dsp = NULL;
struct vis_plugin_module *cur_vis = NULL;

static void list_add(struct plugin_list_head *list, void *plugin)
{
	struct plugin_list_entry *entry;

	entry = malloc(sizeof(struct plugin_list_entry));
	if (entry == NULL)
		return;
	entry->previous = list->head;
	list->head = entry;
}

static void list_remove_all(struct plugin_list_head *list)
{
	struct plugin_list_entry *entry;
	struct plugin_list_entry *prev;
	
	entry = list->head;
	while (entry != NULL) {
		prev = entry->previous;
		free(entry);
		entry = prev;
	}
}

void load_plugin(char *path, char *filename)
{
	void * (*loader)(library lib);
	library lib;
	struct in_plugin *in;
	struct out_plugin *out;
	struct gen_plugin *gen;
	struct dsp_plugin_module *dsp;
	struct vis_plugin_module *vis;

	printf("loading '%s'.\n", path);
	if (strncasecmp(filename, "in_", 3) == 0) {
		lib = load_library(path);
		if (lib == NULL)
			return;
		in = load_in_plugin(lib);
		if (in == NULL)
			return;
		list_add(&in_list, in);
	} else if (strncasecmp(filename, "out_", 4) == 0) {
		lib = load_library(path);
		if (lib == NULL)
			return;
		out = load_out_plugin(lib);
		if (out == NULL)
			return;
		list_add(&out_list, out);
	} else if (strncasecmp(filename, "gen_", 4) == 0) {
		lib = load_library(path);
		if (lib == NULL)
			return;
		gen = load_gen_plugin(lib);
		if (gen == NULL)
			return;
		list_add(&gen_list, gen);
	} else if (strncasecmp(filename, "dsp_", 4) == 0) {
		lib = load_library(path);
		if (lib == NULL)
			return;
		dsp = load_dsp_plugin(lib);
		if (dsp == NULL)
			return;
		list_add(&dsp_list, dsp);
	} else if (strncasecmp(filename, "vis_", 4) == 0) {
		lib = load_library(path);
		if (lib == NULL)
			return;
		vis = load_vis_plugin(lib);
		if (vis == NULL)
			return;
		list_add(&vis_list, vis);
	} else if (strncasecmp(path, "enc_", 4) == 0) {
		lib = load_library(path);
		if (lib == NULL)
			return;
		/*enc = load_enc_plugin(lib);*/
	}
}

void discovery_init()
{
	transverse_dir("plugins", &load_plugin);
}
