/*
 *  yummy/plugins/out/out_null.c
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#include <out.h>
#include <module.h>
#include <msgbox.h>

void configure(window parent)
{
	info(parent, "No configuration.");
}

void about(window parent)
{
	info(parent, "Copyright 2013 Mateus de Lima Oliveira\n\n"
		"Null plugin. This is a test plugin that outputs no sound.");
}
	
void init()
{
}

void quit()
{
}
	
int open(int rate, int channels, int bits, int a, int b)
{
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

void set_panning(int pannig)
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

struct out_plugin out = {
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
#ifdef _WIN32
struct out_plugin * __declspec(dllexport) winampGetOutModule()
{
	return &out;
}
#endif
