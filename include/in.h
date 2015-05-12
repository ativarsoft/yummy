/*
 *  yummy/include/in.h
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#ifndef IN_H
#define IN_H

#include <platform.h>
#include "out.h"

struct in_plugin
{
	/* out */ int version;
	/* out */ char *description;
	/* in */ window_t window;
	/* in */ library_t instance; /* TODO: port. */
	/* out */ char *file_ext;
	/* out */ int is_seekable;
	/* out */ int flags;
	/* out */ void (*configure)(window_t parent);
	/* out */ void (*about)(window_t parent);
	/* out */ void (*init)();
	/* out */ void (*quit)();
	/* out */ void (*get_file_info)(const char *path, char *formated_title, int *len);
	/* out */ int (*info_box)(const char *file, window_t parent);
	/* out */ int (*is_our_file)(const char *path);
	/* out */ int (*play)(const char *file);
	/* out */ void (*pause)();
	/* out */ void (*unpause)();
	/* out */ int (*is_paused)();
	/* out */ void (*stop)();
	/* out */ int (*get_length)();
	/* out */ int (*get_output_time)();
	/* out */ void (*set_output_time)(int ms);
	/* out */ void (*set_volume)(int volume);
	/* out */ void (*set_panning)(int panning);
	/* in */ void (*sa_vsa_init)(int latency, int rate);
	/* in */ void (*sa_vsa_deinit)();
	/* in */ void (*sa_add_pcm_data)(void *pcm_data, int num_channels, int bps, int time);
	/* in */ int (*sa_get_mode)();
	/* in */ int (*sa_add)(void *data, int time, int type);
	/* in */ void (*vsa_add_pcm_data)(void *, int, int, int);
	/* in */ int (*vsa_get_mode)(int, int);
	/* in */ int (*vsa_add)(void *, int);
	/* in */ void (*vsa_set_info)(int, int);
	/* in */ int (*dsp_is_active)();
	/* in */ int (*do_samples)(short *samples, int num, int bits, int channels, int rate);
	/* out */ void (*eq_set)(int, char[10], int);
	/* in */ void (*set_info)(int, int, int, int);
	/* in */ struct out_plugin *out_plugin;
};

#endif
