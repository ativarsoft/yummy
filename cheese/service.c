/*
 *  yummy/cheese/service.c
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#include <cheese.h>
#include <cheese_services.h>
#include <stdint.h>
#include "debug.h"

#ifndef NUM_BUILTIN_SERVICES
#define NUM_BUILTIN_SERVICES 0
#endif

static struct cheese_factory_list_s factory_list[] = 
{
	/*{factory_list+1, service_header_core}, */
	/*{factory_list+2, service_header_application}, */
	/*{0, service_header_language}*/
};

struct cheese_service_header_list_head_s
{
	struct cheese_service_header_list_s *first;
	struct cheese_service_header_list_s *last;
};

int num_services=NUM_BUILTIN_SERVICES;

static inline void service_register(int a, void *b, void **c, int d)
{
	struct args_s {
		struct cheese_service_header_s *header;
	};
	
	int *r = b;
	struct args_s *args = (struct args_s *) c;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 1);
	
	num_services++;
}

static inline void service_unregister(int a, void *b, void **c, int d)
{
	struct args_s {
		struct cheese_service_header_s *header;
	};
	
	int *r = b;
	struct args_s *args = (struct args_s *) c;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 1);
	
	num_services--;
}

static inline void service_get_num_services(int a, void *b, void **c, int d)
{
	struct args_s {
		uint32_t *fourcc;
	};
	
	int *r = b;
	struct args_s *args = (struct args_s *) c;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 1);
}

static inline void service_enum_service(int a, void *b, void **c, int d)
{
	struct args_s {
		uint32_t *fourcc;
		size_t *n;
	};
	
	struct cheese_service_header_s **r = (struct cheese_service_header_s **) b;
	struct args_s *args = (struct args_s *) c;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 2);
}

static inline void service_get_service_by_guid(int a, void *b, void **c, int d)
{
	struct args_s {
		struct guid_s *guid;
	};
	
	struct cheese_service_header_s **r = (struct cheese_service_header_s **) b;
	struct args_s *args = (struct args_s *) c;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 1);
	
	printf("%d arguments.\n", d);
	debug_print_guid(args->guid);
	
	*r = &service_header_dummy;
}

static inline void service_lock(int a, void *b, void **c, int d)
{
	struct args_s {
		struct cheese_service_header_s *a;
		void *b;
	};
	
	void *r = b;
	struct args_s *args = (struct args_s *) c;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 2);
}

static inline void service_client_lock(int a, void *b, void **c, int d)
{
	struct args_s {
		void *a;
	};
	
	void *r = b;
	struct args_s *args = (struct args_s *) c;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 1);
}

static inline void service_release(int a, void *b, void **c, int d)
{
	struct args_s {
		void *a;
	};
	
	void *r = b;
	struct args_s *args = (struct args_s *) c;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 1);
}

static inline void service_get_type_name(int a, void *b, void **c, int d)
{
	struct args_s {
		uint32_t *fourcc;
	};
	
	void *r = b;
	struct args_s *args = (struct args_s *) c;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 1);
}

static inline void service_get_owning_component(int a, void *b, void **c, int d)
{
	struct args_s {
		void *a;
	};
	
	void *r = b;
	struct args_s *args = (struct args_s *) c;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 1);
}

static inline void service_get_locking_component(int a, void *b, void **c, int d)
{
	struct args_s {
		void *a;
	};
	
	void *r = b;
	struct args_s *args = (struct args_s *) c;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 1);
}

static inline void service_unlock(int a, void *b, void **c, int d)
{
	struct args_s {
		void *a;
	};
	
	void *r = b;
	struct args_s *args = (struct args_s *) c;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 1);
}

static inline void service_is_valid(int a, void *b, void **c, int d)
{
	struct args_s {
		uint32_t fourcc;
		struct cheese_service_header_s *header;
	};
	
	void *r = b;
	struct args_s *args = (struct args_s *) c;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 2);
}

static inline void service_compact_duplicates(int a, void *b, void **c, int d)
{
	struct args_s {
		struct cheese_service_header_s *header;
	};
	
	void *r = b;
	struct args_s *args = (struct args_s *) c;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 1);
}

static int __attribute__((stdcall)) service_dispatch(int a, void *b, void **c, int d)
{
	DEBUG_PRINTF("service msg %d\n", a);
	
	switch(a) {
		case SERVICE_REGISTER:
		service_register(a, b, c, d);
		break;
		case SERVICE_UNREGISTER:
		service_unregister(a, b, c, d);
		break;
		case SERVICE_GET_NUM_SERVICES:
		service_get_num_services(a, b, c, d);
		break;
		case SERVICE_ENUM_SERVICE:
		service_enum_service(a, b, c, d);
		break;
		case SERVICE_GET_SERVICE_BY_GUID:
		service_get_service_by_guid(a, b, c, d);
		break;
		case SERVICE_LOCK:
		service_lock(a, b, c, d);
		break;
		case SERVICE_CLIENT_LOCK:
		service_client_lock(a, b, c, d);
		break;
		case SERVICE_RELEASE:
		service_release(a, b, c, d);
		break;
		case SERVICE_GET_TYPE_NAME:
		service_get_type_name(a, b, c, d);
		break;
		case SERVICE_GET_OWNING_COMPONENT:
		service_get_owning_component(a, b, c, d);
		break;
		case SERVICE_GET_LOCKING_COMPONENT:
		service_get_locking_component(a, b, c, d);
		break;
		case SERVICE_UNLOCK:
		service_unlock(a, b, c, d);
		break;
		case SERVICE_IS_VALID:
		service_is_valid(a, b, c, d);
		break;
		case SERVICE_COMPACT_DUPLICATES:
		service_compact_duplicates(a, b, c, d);
		break;
		default:
		DEBUG_UNKNOWN_MESSAGE();
		return 0;
	}
	
	return 1;
}

static struct cheese_dispatchable_vtable_s service_dispatchable_vtable = {
	&service_dispatch
};

struct cheese_service_s api_service = {
	.dispatchable_vpointer = &service_dispatchable_vtable
};
