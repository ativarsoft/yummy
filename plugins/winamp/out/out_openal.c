/*
 *  yummy/plugins/out/out_openal.c
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#include <out.h>
#include <module.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <msgbox.h>

ALdevice *device;
ALcontext *context;

void configure(window_t parent)
{
}

void about(window_t parent)
{
	info(parent, "Copyright 2013 Mateus de Lima Oliveira\n\n"
		"OpenAL output plugin. OpenAL supports 3D audio output.");
}
	
void init()
{
	struct {
		char *al_name;
		char *friendly_name;
	} ext_formats[] = {
		{"AL_FORMAT_QUAD16", "Quad 16 bits"},
		{"AL_FORMAT_51CHN16", "5.1 16 bits"},
		{"AL_FORMAT_61CHN16", "6.1 16 bits"},
		{"AL_FORMAT_71CHN16", "7.1 16 bits"}
	};
	int f;
	
	device = alcOpenDevice(0);
	if (!device)
		return 0;
	context = alcCreateContext(device, 0);
	if (!context)
		return 0;
	alcMakeContextCurrent(context);
	f=alGetEnumValue(ext_formats[1].al_name);
}

void quit()
{
	alcMakeContextCurrent(0); /* release context so it can be destroyed. */
	alcDestroyContext(context);
	alcDestroyDevice(device);
}
	
int open(int rate, int channels, int bits, int a, int b)
{
	alBufferData();
	return 1;
}

void close()
{
}
	
int write(char *buffer, int length)
{
	return 0;
}

int can_write()
{
	return 0;
}

int is_playing()
{
	return 1;
}

int pause(int pause)
{
	return 0xf; /* TODO: return time. */
}
	
void set_volume(int volume)
{
}

void set_panning(int pan)
{
}

void flush(int time)
{
}
	
int get_output_time()
{
	return 0xf; /* TODO: ? */
}

int get_written_time()
{
	return 0xf; /* TODO: ? */
}

struct out_plugin_s out = {
	.version = 0x10,
	.description = "Null",
	.id = 0,
	.configure = &configure,
	.about = &about,
	.init = &init,
	.quit = &quit,
	.open = &open,
	.close = &close,
	.write = &write,
	.can_write = &can_write,
	.is_playing = &is_playing,
	.pause = &pause,
	.set_volume = &set_volume,
	.set_panning = &set_panning,
	.flush = &flush,
	.get_output_time = &get_output_time,
	.get_written_time = &get_written_time
};

static void out_null_init()
{
}

EXPORT(out_null_init);

/* FIXME: remove. */
struct out_plugin_s * __declspec(dllexport) winampGetOutModule()
{
	return &out;
}
