/*
 *  yummy/plugins/in/in_dummy.c
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#include <in.h>
#include <stdbool.h>
#include <windows.h>

bool _is_paused = 0;

void configure(HWND parent)
{
}

void about(HWND parent)
{
}

void init()
{
}

void quit()
{
}

void get_file_info(const char *path, HWND parent)
{
}

int info_box(const char *file, HWND parent)
{
}

int is_our_file(const char *path)
{
	return false;
}

int play(const char *file)
{
	_is_paused = false;
	return 0; /* 0 = ok, -1 = ENOENT */
}

void pause()
{
	_is_paused = !_is_paused;
}

void unpause()
{
}

int is_paused()
{
	return _is_paused;
}

void stop()
{
	_is_paused = false;
}

int get_length()
{
	return 0;
}

int get_output_time()
{
	return 0;
}

void set_output_time(int ms)
{
}

void set_volume(int volume)
{
}

void set_panning(int panning)
{
}

void eq_set(int a, char b[10], int c)
{
}


struct in_plugin_s in = {
	.version = 0x100,
	.description = "libav",
	.file_ext = ".mateus\0test stuff\0", /* TODO: generate at runtime. */
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
	.pause = &pause,
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
