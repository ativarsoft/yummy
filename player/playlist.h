/*
 *  yummy/player/playlist.h
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <stdbool.h>

struct formated_time {
	int hours;
	int minutes;
	int seconds;
};

struct playlist_entry {
	struct playlist_entry *next;
	struct playlist_entry *previous;
	char *path;
	char *formated_title;
	struct formated_time formated_len;
};

struct playlist {
	struct playlist_entry *first;
	struct playlist_entry *last;
};

#endif
