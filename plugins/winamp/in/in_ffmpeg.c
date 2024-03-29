/*
 *  yummy/plugins/in/in_libav.c
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#include <in.h>
#include <stdbool.h>
#include <stdio.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <module.h>
#include <debug.h>
#include <msgbox.h>

#ifndef _WIN32
#include <unistd.h>
#endif

/* TODO: move to documentation. */
/* The functions refer to this struct,
 * but this struct also refers to the functions
 * by holding pointers. */
extern struct in_plugin in;

AVFormatContext *format_context = 0;
AVCodecContext *codec_context;
const AVCodec *codec;

bool _is_paused = 0;

/* TODO: skip for now. */
void configure(window_t parent)
{
	DEBUG_TRACE();
}

/* TODO: skip for now. */
void about(window_t parent)
{
	DEBUG_TRACE();

	info(parent, "Copyright 2013 Mateus de Lima Oliveira\n\n"
		"libav input plugin.");
}

void init()
{
	const AVInputFormat *av_in = NULL;
	void *av_ctx = NULL;
	int len_all=1;
	int len_long_name;
	int len_ext;
	const char *ext;
	int cur_pos;
	char *p;

	DEBUG_TRACE();

	av_in=0;
	cur_pos=0;
	while((av_in = av_demuxer_iterate(&av_ctx)) != NULL) {
		DEBUG_PRINTF("name: %s\n", av_in->name);
		DEBUG_PRINTF("long_name: %s\n", av_in->long_name);
		DEBUG_PRINTF("extensions: %s\n", av_in->extensions);
		len_long_name=strlen(av_in->long_name)+1;
		if(av_in->extensions) {
			len_ext=strlen(av_in->extensions)+1;
			ext = av_in->extensions;
		} else {
			len_ext=2;
			ext = "a"; /* FIXME: */
		}
		len_all+=len_long_name+len_ext;
		in.file_ext=realloc(in.file_ext, len_all);
		if (!in.file_ext)
			; /* TODO: error */
		memcpy(in.file_ext+cur_pos, ext, len_ext);
		cur_pos+=len_ext;
		memcpy(in.file_ext+cur_pos, av_in->long_name, len_long_name);
		cur_pos+=len_long_name;
	}
	in.file_ext[cur_pos]='\0';
	avformat_network_init();
}

void quit()
{
	avformat_network_deinit();
	/* TODO: delete queue. */
}

void get_file_info(const char *path, char *formated_title, int *len)
{
	AVDictionary *meta;
	AVDictionaryEntry *entry;
	char *artist;
	char *title;

	DEBUG_TRACE();

	if(path)
		if (avformat_open_input(&format_context, path, 0, 0) < 0)
			return;
	meta = format_context->metadata;
	entry = av_dict_get(meta, "artist", 0, 0);
	artist = entry->value;
	entry = av_dict_get(meta, "title", 0, 0);
	title = entry->value;

	sprintf(formated_title, "%s - %s", artist, title);
	*len = 0;

	DEBUG_PRINTF("artist: %s\n", artist);
	DEBUG_PRINTF("title: %s\n", title);
}

/* TODO: skip for now. */
int info_box(const char *file, window_t parent)
{
	DEBUG_TRACE();
}

int is_our_file(const char *path)
{
	AVFormatContext *format_context = 0;

	DEBUG_TRACE();
	DEBUG_PRINTF("path: %s\n", path);

	/* TODO: this will only check the container type and protocol.
	 * it will not check the codecs needed. */
	 /* NOTE: the format pointer must be initialized to zero. */
	if (avformat_open_input(&format_context, path, 0, 0) < 0)
		return false;
	avformat_close_input(&format_context);

	return true;
}

#ifdef _WIN32
DWORD __attribute__((stdcall)) playing_thread(void *a)
#else
void playing_thread(void *a)
#endif
{
	AVFrame *frame;
	AVPacket *packet;
	int got_frame;
	int bits;

	frame = av_frame_alloc();
	packet = av_packet_alloc();

	if (av_read_frame(format_context, packet) < 0)
		return;
	switch(codec_context->sample_fmt) {
		case AV_SAMPLE_FMT_U8:
		bits = 8;
		break;
		case AV_SAMPLE_FMT_S16:
		bits = 16;
		break;
		/* NOTE: no 24. */
		case AV_SAMPLE_FMT_S32:
		bits = 32;
		break;
		default:
		return;
	};
	/*in.out_plugin->open(codec_context->sample_rate, codec_context->channels, bits, 0, 0);*/
	in.out_plugin->open(44100, 2, 16, 0, 0);
	for (;;) {
		if (in.out_plugin->can_write() >= frame->nb_samples*2*2) {
			/* TODO: error: argument 2. */
			if (avcodec_receive_frame(codec_context, frame) != 0)
				break;

			/* Obsoleted ffmpeg function */
			/*if (avcodec_decode_audio4(codec_context, frame, &got_frame, &packet) < 1)
				break;*/

#ifdef FF_API_OLD_CHANNEL_LAYOUT
			in.sa_add_pcm_data(frame->data[0], codec_context->ch_layout.nb_channels, bits, 0);
			in.vsa_add_pcm_data(frame->data[0], codec_context->ch_layout.nb_channels, bits, 0);
#else
			in.sa_add_pcm_data(frame->data[0], codec_context->channel_layout, bits, 0);
			in.vsa_add_pcm_data(frame->data[0], codec_context->channel_layout, bits, 0);
#endif

			/* Obsoleted ffmpeg fields */
			/*in.sa_add_pcm_data(frame->data[0], codec_context->channels, bits, 0);
			in.vsa_add_pcm_data(frame->data[0], codec_context->channels, bits, 0);*/

			/*?=in.do_samples(buffer, ?, ?, ?, ?);*/
			if (got_frame)
				in.out_plugin->write(frame->data[0], frame->nb_samples*2*2);
			av_packet_free(&packet);
			if (av_read_frame(format_context, packet) < 0)
				break;
		} else {
#ifdef _WIN32
			Sleep(100); /* TODO: port. */ /* TODO: calculate time from rate? */
#else
			usleep(100);
#endif
		}
	}
}

int play(const char *path)
{
	int audio_stream_index;

	DEBUG_TRACE();

	_is_paused = false;

	/* format stuff */
	if (avformat_open_input(&format_context, path, 0, 0) < 0)
		return -1;
	if (avformat_find_stream_info(format_context, 0) < 0)
		return 1;
	audio_stream_index = av_find_best_stream(format_context, AVMEDIA_TYPE_AUDIO, -1, -1, 0, 0);
	switch (audio_stream_index) {
		case AVERROR_STREAM_NOT_FOUND:
		return 2;
		case AVERROR_DECODER_NOT_FOUND:
		return 3;
	}

	/* codec stuff */

	codec_context = avcodec_alloc_context3(codec);

	//codec_context = format_context->streams[audio_stream_index]->codec;
	if (avcodec_parameters_to_context(codec_context, format_context->streams[audio_stream_index]->codecpar) < 0)
		return 6;

	codec = avcodec_find_decoder(codec_context->codec_id);
	if (!codec)
		return 4;
	/* TODO: handle truncated? */
	if (avcodec_open2(codec_context, codec, 0) < 0)
		return 5;

#ifdef _WIN32
	CreateThread(0, 0, &playing_thread, 0, 0, 0);
#else
	playing_thread(0);
#endif

	return 0; /* 0 = ok, -1 = ENOENT, other = unsupported or corrupt */
}

void pause_playback(void)
{
	DEBUG_TRACE();

	_is_paused = true;
	in.out_plugin->pause(1);
}

void unpause()
{
	DEBUG_TRACE();

	_is_paused = false;
	in.out_plugin->pause(0);
}

int is_paused()
{
	DEBUG_TRACE();

	return _is_paused;
}

void stop()
{
	DEBUG_TRACE();

	_is_paused = false;

	in.out_plugin->close();
	/*in.sa_vsa_deinit();*/

	/*avcodec_close(codec_context);
	avformat_close_input(format_context);*/
}

int get_length()
{
	DEBUG_TRACE();
	DEBUG_PRINTF("length: %lld\n", format_context->duration);

	return (int) format_context->duration;
}

int get_output_time()
{
	DEBUG_TRACE();

	return in.out_plugin->get_output_time();
}

void set_output_time(int ms)
{
	DEBUG_TRACE();
}

void set_volume(int volume)
{
	DEBUG_TRACE();

	in.out_plugin->set_volume(volume);
}

void set_panning(int panning)
{
	DEBUG_TRACE();

	in.out_plugin->set_panning(panning);
}

void eq_set(int a, char b[10], int c)
{
	DEBUG_TRACE();
}


struct in_plugin in = {
	.version = 0x100,
	.description = "libav",
	.file_ext = 0, /* NOTE: generated at runtime. */
	.is_seekable = 1,
	.flags = 1, /* uses_output */
	.configure = &configure,
	.about = &about,
	.init = &init,
	.quit = &quit,
	.get_file_info = &get_file_info,
	.info_box = &info_box,
	.is_our_file = &is_our_file,
	.play = &play,
	.pause = &pause_playback,
	.unpause = &unpause,
	.is_paused = &is_paused,
	.stop = &stop,
	.get_length = &get_length,
	.get_output_time = &get_output_time,
	.set_output_time = &set_output_time,
	.set_volume = &set_volume,
	.set_panning = &set_panning,
	.eq_set = &eq_set
};

void in_libav()
{
	/*register_in_plugin(&in);*/
}

struct in_plugin * DLLEXPORT winampGetInModule2()
{
	return &in;
}

EXPORT(in_libav);

