/*
 *  yummy/player/playback.c
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#include <stdio.h>
#include <stdbool.h>

#include "discovery.h"
#include "playlist.h"
#include "playback.h"

static bool paused;

void sa_vsa_init(int latency, int rate)
{
	cur_vis->latency = latency;
	cur_vis->rate = rate;
}

void sa_vsa_deinit()
{
}

void sa_add_pcm_data(void *pcm_data, int num_channels, int bps, int time)
{
}

int sa_get_mode()
{
	return 0;
}

int sa_add(void *data, int time, int type)
{
	return 0;
}

void vsa_add_pcm_data(void *pcm_data, int channels, int bits, int decode_pos_ms)
{
	int i;
	int div;
	int step;
	
	/* We have 8 bits of precision, so we take the most significant bits.
	 * It's the same as dividing 2^bits by 2^8.
	 * Also, we have to step the number of bytes of the sample. */
	switch(bits) {
		case 8:
		div = 0;
		step = 1;
		break;
		case 16:
		div = 8;
		step = 2;
		break;
		case 32:
		div = 24;
		step = 4;
		break;
	}
	
	i = 576;
	if (channels > 1)
	for (; i >= 0; i--) {
		cur_vis->waveform_data[0][i] = (char)(*((short *)pcm_data)>>div);
		pcm_data += step;
		cur_vis->waveform_data[1][i] = (char)(*((short *)pcm_data)>>div);
		pcm_data += step;
	}
}

int vsa_get_mode(int spectrum_num_channels, int waveform_num_chennels)
{
	return 0;
}

int vsa_add(void *data, int timestamp)
{
	return 0;
}

void vsa_set_info(int rate, int channels)
{
	cur_vis->rate = rate;
	cur_vis->channels = channels;
}

int dsp_is_active()
{
	return 1;
}

int do_samples(short *samples, int num, int bits, int channels, int rate)
{
	return 576;
	return cur_dsp->modify_samples(cur_dsp, samples, num, bits, channels, rate);
}

void set_info(int a, int b, int c, int d)
{
}

void playback_previous(void)
{
}

void playback_play(void)
{
	/* Make sure there is an in plugin that can handle this file. */
	/* An out and plugin is required by the in plugin. */
	if (cur_in == NULL || cur_out == NULL || cur_vis == NULL) {
		fprintf(stderr, "ERROR: plugin chain not complete.\n");
		fflush(stderr);
		return;
	}

	cur_in->out_plugin = cur_out;
	cur_in->play("test.mp3");
	paused = false;
}

void playback_pause(void)
{
	if (cur_in != NULL) {
		if (paused == false)
			cur_in->pause();
		else
			cur_in->unpause();
		paused = !paused;
	}
}

void playback_stop(void)
{
	if (cur_in != NULL)
		cur_in->stop();
}

void playback_next(void)
{
}
