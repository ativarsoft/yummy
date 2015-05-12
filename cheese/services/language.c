/*
 *  yummy/cheese/services/language.c
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#ifdef DEBUG
#include <stdio.h>
#endif
#include <cheese.h>
#ifdef DEBUG
#include "debug.h"
#endif

static inline void language_get_string(int a, void *b, void **c, int d)
{
	debug_trace();
}

static inline void language_get_string_utf16(int a, void *b, void **c, int d)
{
	debug_trace();
}

static inline void language_get_string_from_guid(int a, void *b, void **c, int d)
{
	debug_trace();
}

static inline void language_get_string_from_guid_utf16(int a, void *b, void **c, int d)
{
	debug_trace();
}

static inline void language_get_instance_by_guid(int a, void *b, void **c, int d)
{
	debug_trace();
}

static inline void language_get_instance_by_name(int a, void *b, void **c, int d)
{
	debug_trace();
}

static inline void language_get_instance_by_name_utf16(int a, void *b, void **c, int d)
{
	debug_trace();
}

static inline void language_startup(int a, void *b, void **c, int d)
{
	debug_trace();
}

static inline void language_shutdown(int a, void *b, void **c, int d)
{
	debug_trace();
}

static inline void language_get_language_folder(int a, void *b, void **c, int d)
{
	debug_trace();
}

static inline void language_create_dialog_param(int a, void *b, void **c, int d)
{
	debug_trace();
}

static inline void language_dialog_lparam(int a, void *b, void **c, int d)
{
	debug_trace();
}

static inline void language_load_menu(int a, void *b, void **c, int d)
{
	debug_trace();
}

static inline void language_create_dialog_lparam(int a, void *b, void **c, int d)
{
	debug_trace();
}

static inline void language_dialog_lparam_utf16(int a, void *b, void **c, int d)
{
	debug_trace();
}

static inline void language_load_menu_utf16(int a, void *b, void **c, int d)
{
	debug_trace();
}

static inline void language_get_language_identifier(int a, void *b, void **c, int d)
{
	debug_trace();
}

static inline void language_load_resource_from_file(int a, void *b, void **c, int d)
{
	debug_trace();
}

static inline void language_load_resource_from_file_utf16(int a, void *b, void **c, int d)
{
	debug_trace();
}

static inline void language_load_accelerators(int a, void *b, void **c, int d)
{
	debug_trace();
}

static inline void language_load_accelerators_utf16(int a, void *b, void **c, int d)
{
	debug_trace();
}


static int language_dispatch(int a, void *b, void **c, int d)
{
	switch(a)
	{
		case LANGUAGE_GET_STRING:
		case LANGUAGE_GET_STRING_UTF16:
		case LANGUAGE_GET_STRING_FROM_GUID:
		case LANGUAGE_GET_STRING_FROM_GUID_UTF16:
		case LANGUAGE_GET_INSTANCE_BY_GUID:
		case LANGUAGE_GET_INSTANCE_BY_NAME:
		case LANGUAGE_GET_INSTANCE_BY_NAME_UTF16:
		case LANGUAGE_STARTUP:
		case LANGUAGE_SHUTDOWN:
		case LANGUAGE_GET_LANGUAGE_FOLDER:
		case LANGUAGE_CREATE_DIALOG_PARAM:
		case LANGUAGE_DIALOG_LPARAM:
		case LANGUAGE_LOAD_MENU:
		case LANGUAGE_CREATE_DIALOG_LPARAM:
		case LANGUAGE_DIALOG_LPARAM_UTF16:
		case LANGUAGE_LOAD_MENU_UTF16:
		case LANGUAGE_GET_LANGUAGE_IDENTIFIER:
		case LANGUAGE_LOAD_RESOURCE_FROM_FILE:
		case LANGUAGE_LOAD_RESOURCE_FROM_FILE_UTF16:
		case LANGUAGE_LOAD_ACCELERATORS:
		case LANGUAGE_LOAD_ACCELERATORS_UTF16:
		default:
		debug_printf("WARNING: unknown language message %d\n", a);
		return 1;
	}
	
	return 0;
}

struct cheese_dispatchable_vtable_s language_dispatchable_vtable = 
{
	&language_dispatch
};

struct cheese_language_s api_language = 
{
	.dispatchable_vpointer = &language_dispatchable_vtable
};
