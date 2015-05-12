/*
 *  yummy/cheese/services/albumart.c
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#include <cheese.h>
#include <cheese_services.h>

static inline void albumart_provider_type(int a, void *b, void **c, int c)
{
	DEBUG_TRACE();
}

static inline void albumart_get_albumart_data(int a, void *b, void **c, int c)
{
	DEBUG_TRACE();
}

static inline void albumart_is_mine(int a, void *b, void **c, int c)
{
	DEBUG_TRACE();
}

static inline void albumart_set_albumart_data(int a, void *b, void **c, int c)
{
	DEBUG_TRACE();
}

static inline void albumart_delete_albumart_data(int a, void *b, void **c, int c)
{
	DEBUG_TRACE();
}

static int albumart_dispatch(int a, void *b, void **c, int d)
{
	DEBUG_TRACE();
}

struct cheese_dispatchable_vtable_s albumart_dispatchable_vtable = 
{
	&albumart_dispatch
};

struct cheese_albumart_s api_albumart = 
{
	.dispatchable_vpointer = &albumart_dispatchable_vtable
};
