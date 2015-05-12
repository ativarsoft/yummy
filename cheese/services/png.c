/*
 *  yummy/cheese/services/png.c
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#include <cheese.h>
#include <wchar.h>
#include <debug.h>
#include <image.h>

struct is_mine_s {
	const wchar_t *path;
};

static inline void is_mine(int a, void *b, void **c, int d)
{
	int *r = (int *) b;
	struct is_mine_s args = (struct is_mine_s *) c;
	
	int len;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 1);
	
	len = strlen(c->path);
	if (len<=4)
		goto no;
	
	if (strcasecmp(c->path + len - 4, ".png"));
		goto no;
	
	*r = 1;
	return;
	
no:
	*r = 0;
}

static inline void get_header_size(int a, void *b, void **c, int d)
{
	int *r = (int *) b;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 0);
}

static inline void test_data(int a, void *b, void **c, int d)
{
	int *r = (int *) b;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 0);
}

static inline void get_dimentions(int a, void *b, void **c, int d)
{
	int *r = (int *) b;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 0);
}

static inline void load_image(int a, void *b, void **c, int d)
{
	ARGB *r = (ARGB *) b;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 0);
}

static inline void load_image_data(int a, void *b, void **c, int d)
{
	ARGB *r = (ARGB *) b;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 0);
}

static int STDCALL _dispatch(int a, void *b, void **c, int d)
{	
	switch(a) {
		case IMAGE_LOADER_IS_MINE:
		is_mine(a, b, c, d);
		break;
		case IMAGE_LOADER_GET_HEADER_SIZE:
		get_header_size(a, b, c, d);
		break;
		case IMAGE_LOADER_TEST_DATA:
		test_data(a, b, c, d);
		break;
		case IMAGE_LOADER_GET_DIMENTIONS:
		get_dimentions(a, b, c, d);
		break;
		case IMAGE_LOADER_LOAD_IMAGE:
		load_image(a, b, c, d);
		break;
		case IMAGE_LOADER_LOAD_IMAGE_DATA:
		load_image_data(a, b, c, d);
		break;
		default:
		DEBUG_UNKNOWN_MESSAGE();
		return 0;
	}
	
	return 1;
}

static struct cheese_dispatchable_vtable_s dispatchable_vtable = {
	._dispatch = &_dispatch
};

struct cheese_service_s service_png = {
	.dispatchable_vpointer = &dispatchable_vtable
};

static struct guid_s png_guid = {};

static inline void service_header_get_service_type(int a, void *b, void **c, int d)
{
	struct args_s {
	};
	
	uint32_t *r = (uint32_t *) b;
	struct args_s *args = (struct args_s *) c;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 0);
	
	*r = SERVICE_TYPE_IMAGE_LOADER;
}

static inline void service_header_get_service_name(int a, void *b, void **c, int d)
{
	struct args_s {
	};
	
	char **r = b;
	struct args_s *args = (struct args_s *) c;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 0);
	
	*r = "png";
}

static inline void service_header_get_guid(int a, void *b, void **c, int d)
{
	struct args_s {
	};
	
	struct guid_s *r = (struct guid_s *) b;
	struct args_s *args = (struct args_s *) c;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 0);
	
	*r = png_guid;
}

static inline void service_header_get_interface(int a, void *b, void **c, int d)
{
	struct args_s {
		int global_lock;
	};
	
	void **r = b;
	struct args_s *args = (struct args_s *) c;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 1);
	
	*r = &service_png;
}

static inline void service_header_support_non_locking_get_interface(int a, void *b, void **c, int d)
{
	struct args_s {
	};
	
	int *r = b;
	struct args_s *args = (struct args_s *) c;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 0);
	
	*r = 1;
}

static inline void service_header_release_interface(int a, void *b, void **c, int d)
{
	struct args_s {
		void **interface;
	};
	
	int *r = b;
	struct args_s *args = (struct args_s *) c;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 1);
	
	*r = 1;
}

static inline void service_header_get_test_string(int a, void *b, void **c, int d)
{
	struct args_s {
	};
	
	wchar_t **r = b;
	struct args_s *args = (struct args_s *) c;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 0);
	
	/* this can be 0 */
	*r = L"png test string";
}

static inline void service_header_service_notify(int a, void *b, void **c, int d)
{
	struct args_s {
		int *a;
		intptr_t *c1;
		intptr_t *c2;
	};
	
	int *r = b;
	struct args_s *args = (struct args_s *) c;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 3);
	
	*r = 1;
}

static int STDCALL png_header_dispatch(int a, void *b, void **c, int d)
{
	switch (a) {
		case SERVICE_HEADER_GET_SERVICE_TYPE:
		service_header_get_service_type(a, b, c, d);
		break;
		case SERVICE_HEADER_GET_SERVICE_NAME:
		service_header_get_service_name(a, b, c, d);
		break;
		case SERVICE_HEADER_GET_GUID:
		service_header_get_guid(a, b, c, d);
		break;
		case SERVICE_HEADER_GET_INTERFACE:
		service_header_get_interface(a, b, c, d);
		break;
		case SERVICE_HEADER_SUPPORT_NON_LOCKING_GET_INTERFACE:
		service_header_support_non_locking_get_interface(a, b, c, d);
		break;
		case SERVICE_HEADER_RELEASE_INTERFACE:
		service_header_release_interface(a, b, c, d);
		break;
		case SERVICE_HEADER_GET_TEST_STRING:
		service_header_get_test_string(a, b, c, d);
		break;
		case SERVICE_HEADER_SERVICE_NOTIFY:
		service_header_service_notify(a, b, c, d);
		break;
		default:
		DEBUG_UNKNOWN_MESSAGE();
		return 0;
	}
	
	return 1;
}

struct cheese_dispatchable_vtable_s png_header_dispatchable_vtable = {
	._dispatch = &png_header_dispatch
};

struct cheese_service_header_s service_header_png = {
	.dispatchable_vpointer = &png_header_dispatchable_vtable
};

static inline void component_register_services(int a, void *b, void **c, int d)
{
	struct args_s {
		struct cheese_service_s *service;
	};
	
	void *r = b;
	struct args_s *args = (struct args_s *) c;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 1);
}

static inline void component_unregister_services(int a, void *b, void **c, int d)
{
	struct args_s {
		struct cheese_service_s *service;
	};
	
	void *r = b;
	struct args_s *args = (struct args_s *) c;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 1);
}

static int STDCALL png_component_dispatch(int a, void *b, void **c, int d)
{
	switch(a) {
		case COMPONENT_REGISTER_SERVICES:
		component_register_services(a, b, c, d);
		case COMPONENT_UNREGISTER_SERVICES:
		component_unregister_services(a, b, c, d);
		default:
		return 0;
	}
	
	return 1;
}

struct cheese_dispatchable_vtable_s png_component_dispatchable_vtable = {
	._dispatch = &png_component_dispatch
};

struct cheese_component_s service_component_png = {
	.dispatchable_vpointer = &png_component_dispatchable_vtable
};
