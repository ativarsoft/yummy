/*
 *  yummy/player/plugin.c
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#include <stdint.h>
#include <stddef.h>

#include <dsp.h>
#include <gen.h>
#include <in.h>
#include <out.h>
#include <vis.h>
#include <error.h>
#include <platform.h>
#include <debug.h>
#include "playback.h"
#include "discovery.h"
#include "gui.h"

struct in_plugin * load_in_plugin(library_t lib)
{
	struct in_plugin *in;
	struct in_plugin * (*get_in_module)();
	
	get_in_module = (struct in_plugin * (*)()) get_symbol(lib, "winampGetInModule2");
	printf("last error get_symbol %x\n", GetLastError());
	if(get_in_module == NULL)
		return NULL;
	in = get_in_module();
	in->window = main_window;
	in->instance = lib;
	in->sa_vsa_init = &sa_vsa_init;
	in->sa_vsa_deinit = &sa_vsa_deinit;
	in->sa_add_pcm_data = &sa_add_pcm_data;
	in->sa_get_mode = &sa_get_mode;
	in->sa_add = &sa_add;
	in->vsa_add_pcm_data = &vsa_add_pcm_data;
	in->vsa_get_mode = &vsa_get_mode;
	in->vsa_add = &vsa_add;
	in->vsa_set_info = &vsa_set_info;
	in->dsp_is_active = &dsp_is_active;
	in->do_samples = &do_samples;
	in->set_info = &set_info;
	in->out_plugin = cur_out;
	in->init();
	DEBUG_PUTS(in->description);
	cur_in = in; /* TODO: remove. */
	
	return in;
}

struct out_plugin * load_out_plugin(library_t lib)
{
	struct out_plugin *out;
	struct out_plugin * (*get_out_module)();
	
	get_out_module = (struct out_plugin * (*)()) get_symbol(lib, "winampGetOutModule");
	if(get_out_module == NULL)
		return NULL;
	out = get_out_module();
	out->window = main_window;
	out->instance = lib;
	out->init();
	out->set_volume(255);
	out->set_panning(0);
	DEBUG_PUTS(out->description);
	/*out->about(0);*/
	/*out->configure(0);*/
	/*out->quit();*/
	cur_out = out; /* TODO: remove. */
	
	return out;
}

struct gen_plugin * load_gen_plugin(library_t lib)
{
	struct gen_plugin *gen;
	struct gen_plugin * (*get_gen_module)();
	
	get_gen_module = (struct gen_plugin * (*)()) get_symbol(lib, "winampGetGeneralPurposePlugin");
	if(get_gen_module == NULL)
		return NULL;
	gen = get_gen_module();
	gen->window = main_window;
	gen->dll_instance = lib;
	gen->init();
#ifdef DEBUG
	puts(gen->description);
#endif
	gen->configure();
	
	return gen;
}

struct dsp_plugin_module * load_dsp_plugin(library_t lib)
{
	struct dsp_plugin_header *dsp_header;
	struct dsp_plugin_module *dsp_module;
	struct dsp_plugin_header * (*get_dsp_header)();
	
	get_dsp_header = (struct dsp_plugin_header * (*)()) get_symbol(lib, "winampDSPGetHeader2");
	if(get_dsp_header == NULL)
		return NULL;
	dsp_header = get_dsp_header();
	DEBUG_PUTS(dsp_header->description);
	dsp_module = dsp_header->get_dsp_module(0);
	dsp_module->main_window = main_window;
	dsp_module->dll_instance = lib;
	dsp_module->init(dsp_module);
	DEBUG_PUTS(dsp_module->description);
	
	return dsp_module;
}

struct vis_plugin_module * load_vis_plugin(library_t lib)
{
	struct vis_plugin_header *vis_header;
	struct vis_plugin_module *vis_module;
	struct vis_plugin_header * (*get_vis_header)();
	
	get_vis_header = (struct vis_plugin_header * (*)()) get_symbol(lib, "winampVisGetHeader");
	if(get_vis_header == NULL)
		return NULL;
	vis_header = get_vis_header();
	DEBUG_PUTS(vis_header->description);
	vis_module = vis_header->get_vis_module(0);
	vis_module->main_window = main_window;
	vis_module->dll_instance = lib;
	vis_module->init(vis_module);
	DEBUG_PUTS(vis_module->description);
	DEBUG_PRINTF("latency: %d\n", vis_module->latency);
	DEBUG_PRINTF("delay: %d\n", vis_module->delay);
	cur_vis = vis_module;
	
	return vis_module;
}
