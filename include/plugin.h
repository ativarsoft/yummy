/*
 *  yummy/include/plugin.h
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#ifndef PLUGIN_H
#define PLUGIN_H

#include <dsp.h>
#include <gen.h>
#include <in.h>
#include <out.h>
#include <vis.h>

struct in_plugin * load_in_plugin(library_t lib);
struct out_plugin * load_out_plugin(library_t lib);
struct gen_plugin * load_gen_plugin(library_t lib);
struct dsp_plugin_module * load_dsp_plugin(library_t lib);
struct vis_plugin_module * load_vis_plugin(library_t lib);

#endif
