/*
 *  yummy/cheese/services/core.c
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
#include "playback.h"

static int core_dispatch(int a, void *b, void **c, int d)
{	
	switch(a)
	{
		case CORE_GET_SUPPORTED_EXTENSIONS:
		case CORE_GET_EXT_SUPPORTED_EXTENSIONS:
		case CORE_CREATE:
		case CORE_FREE:
		case CORE_SET_NEXT_FILE:
		case CORE_GET_STATUS:
		case CORE_GET_CURRENT:
		case CORE_GET_CUR_PLAYBACK_NUMBER:
		case CORE_GET_POSITION:
		case CORE_GET_WRITE_POSITION:
		case CORE_SET_POSITION:
		case CORE_GET_LENGTH:
		case CORE_GET_PLUGIN_DATA:
		case CORE_GET_VOLUME:
		case CORE_SET_VOLUME:
		case CORE_GET_PAN:
		case CORE_SET_PAN:
		case CORE_ADD_CALLBACK:
		case CORE_DEL_CALLBACK:
		case CORE_GET_VIS_DATA:
		case CORE_GET_LEFT_VU_METER:
		case CORE_GET_RIGHT_VU_METER:
		case CORE_REGISER_SEQUENCER:
		case CORE_UNREGISTER_SEQUENCER:
		case CORE_USER_BUTTON:
		case CORE_GET_EQ_STATUS:
		case CORE_SET_EQ_STATUS:
		case CORE_GET_EQ_PREAMP:
		case CORE_SET_EQ_PREAMP:
		case CORE_GET_EQ_BAND:
		case CORE_SET_EQ_BAND:
		case CORE_GET_EQ_AUTO:
		case CORE_SET_EQ_AUTO:
		case CORE_CUSTOM_MSG:
		case CORE_REGISTER_EXTENSION:
		case CORE_GET_EXTENSION_FAMILY:
		case CORE_UNREGISTER_EXTENSION:
		case CORE_GET_TITLE:
		case CORE_SET_TITLE:
		case CORE_GET_DECODER_NAME:
		case CORE_GET_RATING:
		case CORE_SET_RATING:
		default:
		debug_printf("WARNING: unknown core message %d\n", a);
		return 0;
	}
	
	return 1;
}

static struct cheese_dispatchable_vtable_s core_dispatchable_vtable = 
{
	._dispatch = &core_dispatch
};

struct cheese_core_s api_core =
{
	.dispatchable_vpointer = &core_dispatchable_vtable
};
