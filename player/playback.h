/*
 *  yummy/player/playback.h
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#ifndef PLAYBACK_H
#define PLAYBACK_H

void sa_vsa_init(int latency, int rate);
void sa_vsa_deinit();
void sa_add_pcm_data(void *pcm_data, int num_channels, int bps, int time);
int sa_get_mode();
int sa_add(void *data, int time, int type);
void vsa_add_pcm_data(void *pcm_data, int channels, int bits, int decode_pos_ms);
int vsa_get_mode(int spectrum_num_channels, int waveform_num_chennels);
int vsa_add(void *data, int timestamp);
void vsa_set_info(int rate, int channels);
int dsp_is_active();
int do_samples(short *samples, int num, int bits, int channels, int rate);
void set_info(int a, int b, int c, int d);

void playback_previous(void);
void playback_play(void);
void playback_pause(void);
void playback_stop(void);
void playback_next(void);

#endif
