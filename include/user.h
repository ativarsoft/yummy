/*
 *  yummy/include/user.h
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#ifndef USER_H
#define USER_H

#include <platform.h> /* library_t */
#include <stdint.h>

#define USER_GET_VERSION 0
#define USER_GET_VERSION_STRING 1
#define USER_DONATION 770

#define USER_PLAY_FILE 100
#define USER_ENQUEUE_FILE 100
#define USER_PLAYFILE_UTF16 1100
#define USER_ENQUEUE_FILE_UTF16 1100
#define USER_DELETE 101
#define USER_DELETE_INTERNAL 1101
#define USER_START_PLAY 102
#define USER_START_PLAY_INTERNAL 1102
#define USER_CHDIR 103
#define USER_IS_PLAYING 104
#define USER_GET_OUTPUT_TIME 105
#define USER_JUMP_TO_TIME 106
#define USER_GET_MODULE_NAME 109
#define USER_EX_IS_RIGHT_EXE 666
#define USER_WRITE_PLAYLIST 120
#define USER_SET_PLAYLIST_POS 121
#define USER_SET_VOLUME 122
/*#define USER_GET_VOLUME*/
#define USER_SET_PANNING 123
#define USER_GET_LIST_LENGTH 124
#define USER_GET_LIST_POS 125
#define USER_GET_INFO 126
#define USER_GET_EQ_DATA 127
#define USER_SET_EQ_DATA 128
#define USER_ADD_BOOKMARK 129
#define USER_ADD_BOOKMARK_UTF16 131
#define USER_INSTALL_PLUGIN 130
#define USER_RESTART_WINAMP 135
#define USER_IS_FULL_STOP 400
#define USER_INTERNET_AVAILABLE 242
#define USER_UPDATE_TITLE 243
#define USER_REFRESH_PLAYLIST_CACHE 247
#define USER_GET_SHUFFLE 250
#define USER_GET_REPEAT 251
#define USER_SET_SHUFFLE 252
#define USER_SET_REPEAT 253

/* FIXME: fix name. */
#define USER_ADD_PREFS_DLG 332

#define USER_GET_API_SERVICE 3025

struct tree_node_s {
	library_t lib;
	int dialog_id;
	int __attribute__((stdcall)) (*callback)(window_t window, unsigned int msg, int a, int b);
	char *name;
	intptr_t node;
	intptr_t id;
	struct tree_node_s *next;
};

#endif
