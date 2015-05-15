/*
 *  yummy/include/vis.h
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#ifndef VIS_H
#define VIS_H

#include <platform.h>

struct vis_plugin_module
{
	char *description;
	window main_window;
	library dll_instance;
	
	int rate;
	int channels;
	int latency;
	int delay;
	
	int spectrum_num_channels;
	int waveform_num_channels;
	unsigned char spectrum_data[2][576];
	unsigned char waveform_data[2][576];
	
	void (*configure)(struct vis_plugin_module *this);
	int (*init)(struct vis_plugin_module *this);
	int (*render)(struct vis_plugin_module *this);
	void (*quit)(struct vis_plugin_module *this);
	
	void *user_data;
};

struct vis_plugin_header
{
	int version;
	char *description;
	struct vis_plugin_module * (*get_vis_module)(int);
};

#endif
