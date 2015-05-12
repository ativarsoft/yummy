/*
 *  yummy/plugins/out/out_alsa.c
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#include <error.h>
#include <alsa/asoundlib.h>

#include "noise.h"

short *buffer;

void alsa()
{
	snd_pcm_t *handle;
	snd_pcm_hw_params_t *params;
	unsigned int f;
	int dir;
	int frames;
	int r;
	
	buffer=malloc(1024*sizeof(short)*2);
	
	/* this seems not to be set in the tutorial. */
	dir=0;
	
	r=snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0);
	if(r<0)
		error(1, 0, "snd_pcm_open: %s", snd_strerror(r));
	
	/* format */
	/* alloc on the stack */
	r=snd_pcm_hw_params_malloc(&params);
	if(r<0)
		error(0,0,"snd_pcm_hw_params_malloc");
	
	r=snd_pcm_hw_params_any(handle, params);
	if(r<0)
		error(0,0,"snd_pcm_hw_params_any");
	
	r=snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
	if(r<0)
		error(0, 0, "snd_pcm_hw_params_set_access");
	
	r=snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);
	if(r<0)
		error(0, 0, "snd_pcm_hw_params_set_format: %s", snd_strerror(r));
	
	r=snd_pcm_hw_params_set_channels(handle, params, 1);
	if(r<0)
		error(0, 0, "snd_pcm_hw_params_set_channels: %s", snd_strerror(r));
	
	f=44100;
	r=snd_pcm_hw_params_set_rate_near(handle, params, &f, &dir);
	if(r<0)
		error(0, 0, "snd_pcm_hw_params_set_rate_near: %s", snd_strerror(r));
	
	frames=1024;
	r=snd_pcm_hw_params_set_period_size_near(handle, params, &frames, &dir);
	if(r<0)
		error(0,0,"snd_pcm_hw_params_set_period_size_near");
	
	r=snd_pcm_hw_params(handle, params);
	if(r<0)
		error(1, 0, "snd_pcm_hw_params: %s", snd_strerror(r));
	
	snd_pcm_hw_params_free(params);
	
	square_wave(buffer, 1024, 16);
	/*fwrite(buffer, 1024, 2, stdout);*/
	snd_pcm_writei(handle, buffer, frames);
	/* TODO: complete the error strings in some calls. */
	/* free buffer */
	snd_pcm_drain(handle);
	snd_pcm_close(handle);
}

void test_alsa()
{
	alsa();
}
