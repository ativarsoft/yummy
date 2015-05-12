/*
 *  yummy/cheese/services/dummy.c
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#include <cheese.h>
#include <config.h>
#include <debug.h>
#include <stdint.h>
#include <wchar.h>

static int STDCALL dummy_dispatch(int a, void *b, void **c, int d)
{	
	switch(a) {
		default:
		DEBUG_UNKNOWN_MESSAGE();
		return 0;
	}
	
	return 1;
}

static struct cheese_dispatchable_vtable_s dummy_dispatchable_vtable = {
	._dispatch = &dummy_dispatch
};

struct cheese_service_s service_dummy = {
	.dispatchable_vpointer = &dummy_dispatchable_vtable
};

static struct guid_s dummy_guid = {};

static inline void service_header_get_service_type(int a, void *b, void **c, int d)
{
	struct args_s {
	};
	
	uint32_t *r = (uint32_t *) b;
	struct args_s *args = (struct args_s *) c;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 0);
	
	*r = FOURCC(' ', ':', '-', ')');
}

static inline void service_header_get_service_name(int a, void *b, void **c, int d)
{
	struct args_s {
	};
	
	char **r = b;
	struct args_s *args = (struct args_s *) c;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 0);
	
	*r = "dummy";
}

static inline void service_header_get_guid(int a, void *b, void **c, int d)
{
	struct args_s {
	};
	
	struct guid_s *r = (struct guid_s *) b;
	struct args_s *args = (struct args_s *) c;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 0);
	
	*r = dummy_guid;
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
	
	*r = &service_dummy;
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
	*r = L"dummy test string";
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

static int STDCALL dummy_header_dispatch(int a, void *b, void **c, int d)
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

struct cheese_dispatchable_vtable_s dummy_header_dispatchable_vtable = {
	._dispatch = &dummy_header_dispatch
};

struct cheese_service_header_s service_header_dummy = {
	.dispatchable_vpointer = &dummy_header_dispatchable_vtable
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

static int STDCALL dummy_component_dispatch(int a, void *b, void **c, int d)
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

struct cheese_dispatchable_vtable_s dummy_component_dispatchable_vtable = {
	._dispatch = &dummy_component_dispatch
};

struct cheese_component_s service_component_dummy = {
	.dispatchable_vpointer = &dummy_component_dispatchable_vtable
};
