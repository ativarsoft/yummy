/*
 *  yummy/include/dsp.h
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#ifndef DSP_H
#define DSP_H

#include <platform.h>

struct dsp_plugin_module
{
	char *description;
	window_t main_window;
	library_t dll_instance;
	
	void (*configure)(struct dsp_plugin_module *this);
	int (*init)(struct dsp_plugin_module *this);
	int (*modify_samples)(struct dsp_plugin_module *this, short *samples, int num, int bits, int channels, int rate);
	void (*quit)(struct dsp_plugin_module *this);
	void *user_data;
};

struct dsp_plugin_header
{
	int version;
	char *description;
	
	struct dsp_plugin_module * (*get_dsp_module)(int);
	int (*a)(int);
};

#endif
