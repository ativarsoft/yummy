/*
 *  yummy/cheese/services/application.c
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#include <cheese.h>
#include <cheese_services.h>
#include <config.h> /* app name and version */
#include <platform.h>
#include <wchar.h>
#include <string.h>
#include "debug.h"

#define _WIDE(a) L##a
#define WIDE(a) _WIDE(a)

struct guid_s application_guid = {
	0x23b96771, 0x09d7, 0x46d3, {0x9a, 0xe2, 0x20, 0xdc, 0xea, 0x6c, 0x86, 0xea}
};

static inline void application_get_app_name(int a, void *b, void **c, int d)
{
	struct args_s {
	};
	
	wchar_t **r = b;
	struct args_s *args = (struct args_s *) c;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 0);
	
	*r = WIDE(PACKAGE_NAME);
}

static inline void application_get_version_string(int a, void *b, void **c, int d)
{
	struct args_s {
	};
	
	wchar_t **r = b;
	struct args_s *args = (struct args_s *) c;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 0);
	
	*r = WIDE(PACKAGE_STRING);
}

static inline void application_get_version_num_string(int a, void *b, void **c, int d)
{
	struct args_s {
	};
	
	wchar_t **r = b;
	struct args_s *args = (struct args_s *) c;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 0);
	
	*r = WIDE(PACKAGE_VERSION);
}

static inline void application_get_build_number(int a, void *b, void **c, int d)
{
	struct args_s {
	};
	
	unsigned int *r = b;
	struct args_s *args = (struct args_s *) c;
	
	int major;
	int minor;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 0);
	
	sscanf(PACKAGE_VERSION, "%d.%d", &major, &minor);
	
	*r = (major<<8) | minor; /* TODO: check versioning. */
}

static inline void application_get_guid(int a, void *b, void **c, int d)
{
	struct args_s {
	};
	
	struct guid_s *r = b;
	struct args_s *args = (struct args_s *) c;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 0);
	
	memcpy(r, &application_guid, sizeof(struct guid_s));
}

#ifdef _WIN32
static inline void application_get_main_thread_handle(int a, void *b, void **c, int d)
{
	struct args_s {
	};
	
	HANDLE *r = b;
	struct args_s *args = (struct args_s *) c;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 0);
	
	*r=GetCurrentThread();
}

static inline void application_get_hinstance(int a, void *b, void **c, int d)
{
	struct args_s {
	};
	
	HINSTANCE *r = b;
	struct args_s *args = (struct args_s *) c;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 0);
	
	*r=GetModuleHandle(0);
}
#endif

static inline void application_get_command_line(int a, void *b, void **c, int d)
{
	struct args_s {
	};
	
	wchar_t **r = b;
	struct args_s *args = (struct args_s *) c;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 0);
	
#ifdef _WIN32
	*r = GetCommandLineW();
#else
	*r = ""; /* TODO: GNU/Linux. */
#endif
}

static inline void application_shutdown(int a, void *b, void **c, int d)
{
	struct args_s {
	};
	
	void *r = b;
	struct args_s *args = (struct args_s *) c;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 1);
}

static inline void application_cancel_shutdown(int a, void *b, void **c, int d)
{
	struct args_s {
	};
	
	void *r = b;
	struct args_s *args = (struct args_s *) c;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 0);
}

static inline void application_is_shutting_down(int a, void *b, void **c, int d)
{
	struct args_s {
	};
	
	int *r = b;
	struct args_s *args = (struct args_s *) c;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 0);
}

static inline void application_get_app_path(int a, void *b, void **c, int d)
{
	struct args_s {
	};
	
	wchar_t *r = b;
	struct args_s *args = (struct args_s *) c;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 0);
}

static inline void application_get_user_settings_path(int a, void *b, void **c, int d)
{
	struct args_s {
	};
	
	wchar_t *r = b;
	struct args_s *args = (struct args_s *) c;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 0);
}

static inline void application_get_init_count(int a, void *b, void **c, int d)
{
	struct args_s {
	};
	
	int *r = b;
	struct args_s *args = (struct args_s *) c;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 0);
}

static inline void application_message_loop_step(int a, void *b, void **c, int d)
{
	struct args_s {
	};
	
	int **r = b;
	struct args_s *args = (struct args_s *) c;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 0);
}

#ifdef _WIN32
static inline void application_add_message_processor(int a, void *b, void **c, int d)
{
	struct args_s {
	};
	
	void *r = b;
	struct args_s *args = (struct args_s *) c;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 1);
}

static inline void application_remove_message_processor(int a, void *b, void **c, int d)
{
	struct args_s {
	};
	
	void *r = b;
	struct args_s *args = (struct args_s *) c;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 1);
}

static inline void application_add_modeless_dialog(int a, void *b, void **c, int d)
{
	struct args_s {
	};
	
	void *r = b;
	struct args_s *args = (struct args_s *) c;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 1);
}

static inline void application_remove_modeless_dialog(int a, void *b, void **c, int d)
{
	struct args_s {
	};
	
	void *r = b;
	struct args_s *args = (struct args_s *) c;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 1);
}
#endif

static inline void application_get_working_path(int a, void *b, void **c, int d)
{
	struct args_s {
	};
	
	wchar_t **r = b;
	struct args_s *args = (struct args_s *) c;
	
	wchar_t *buffer;
	int len;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 0);
	
	*r = "";
}

static inline void application_set_working_path(int a, void *b, void **c, int d)
{
	struct args_s {
		wchar_t **path;
	};
	
	void *r = b;
	struct args_s *args = (struct args_s *) c;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 1);
}

static inline void application_get_machine_id(int a, void *b, void **c, int d)
{
	struct args_s {
		struct guid_s *guid;
	};
	
	int *r = b;
	struct args_s *args = (struct args_s *) c;
	
#ifdef _WIN32
	HKEY key;
	DWORD type;
	char data[32+6+1];
	DWORD len;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 1);
	
	RegOpenKeyEx(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Criptography", 0, KEY_QUERY_VALUE, &key);
	len=sizeof(data);
	RegQueryValueEx(key, "MachineGuid", 0, &type, data, &len);
	if (len!=sizeof(data)) { /* dunno if ms windows will generate leading zeroes */
		*r = 0;
		return;
	}
	RegCloseKey(key);
	
	sscanf(data, "{%x-%hx-%hx-%hhx%hhx-%hhx%hhx%hhx%hhx%hhx%hhx}", 
		&args->guid->a, &args->guid->b, &args->guid->c,
		args->guid->d, args->guid->d+1, args->guid->d+2, args->guid->d+3,
		args->guid->d+4, args->guid->d+5, args->guid->d+6, args->guid->d+7);
#endif
	
	*r = 1;
}

static inline void application_get_user_id(int a, void *b, void **c, int d)
{
	struct args_s {
	};
	
	int *r=b;
	struct args_s *args = (struct args_s *) c;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 1);
}

static inline void application_get_session_id(int a, void *b, void **c, int d)
{
	struct args_s {
	};
	
	int *r=b;
	struct args_s *args = (struct args_s *) c;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 1);
}

#ifdef _WIN32
static inline void application_add_accelerators(int a, void *b, void **c, int d)
{
	struct args_s {
		HWND *window;
		HACCEL **old;
		INT n;
		UINT a;
	};
	
	void *r = b;
	struct args_s *args = (struct args_s *) c;
	
	int n;
	ACCEL *new_table;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 4);
	
	/*n=CopyAcceleratorTable(handle, 0, 0);
	new_table=malloc((n+1)*sizeof(ACCEL));
	CopyAcceleratorTable(handle, new, n);
	DestroyAcceleratorTable(handle);
	CreateAcceleratorTable(new, new_table);*/
}

static inline void application_remove_accelerators(int a, void *b, void **c, int d)
{
	struct args_s {
		HWND *window;
	};
	
	void *r = b;
	struct args_s *args = (struct args_s *) c;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 1);
}

static inline void application_translate_accelerators(int a, void *b, void **c, int d)
{
	struct args_s {
		MSG **msg;
	};
	
	bool_t *r = b;
	struct args_s *args = (struct args_s *) c;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 1);
}

static inline void application_get_accelerators(int a, void *b, void **c, int d)
{
	struct args_s {
	};
	
	int *r = b;
	struct args_s *args = (struct args_s *) c;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 4);
}

static inline void application_register_global_window(int a, void *b, void **c, int d)
{
	struct args_s {
	};
	
	void *r = b;
	struct args_s *args = (struct args_s *) c;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 1);
}

static inline void application_unregister_global_window(int a, void *b, void **c, int d)
{
	struct args_s {
	};
	
	void *r = b;
	struct args_s *args = (struct args_s *) c;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 1);
}
#endif

static inline void application_allocate_thread_storage(int a, void *b, void **c, int d)
{
	struct args_s {
	};
	
	size_t *r = b;
	struct args_s *args = (struct args_s *) c;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 0);
	
#ifdef _WIN32
	*r = (size_t) TlsAlloc();
	if (*r == TLS_OUT_OF_INDEXES) /* NOTE: TLS_OUT_OF_INDEXES is 0xffffffff. */
		*r = -1;
#else
	*r = -1;
#endif
}

static inline void application_get_thread_storage(int a, void *b, void **c, int d)
{
	struct args_s {
		size_t *index;
	};
	
	void **r = b;
	struct args_s *args = (struct args_s *) c;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 1);
	
#ifdef _WIN32
	*r = (void *) TlsGetValue((DWORD) *args->index);
#else
	*r = 0;
#endif
}

static inline void application_set_thread_storage(int a, void *b, void **c, int d)
{
	struct args_s {
		size_t *index;
		void **value;
	};
	
	void *r = b;
	struct args_s *args = (struct args_s *) c;
	
	DEBUG_TRACE();
	DEBUG_CHECK_NUM_ARGS(d, 2);
	
#ifdef _WIN32
	TlsSetValue((DWORD) *args->index, (LPVOID) *args->value);
#endif
}

static int STDCALL application_dispatch(int a, void *b, void **c, int d)
{
	switch(a)
	{
		case APPLICATION_GET_APP_NAME:
		application_get_app_name(a, b, c, d);
		break;
		case APPLICATION_GET_VERSION_STRING:
		application_get_version_string(a, b, c, d);
		break;
		case APPLICATION_GET_VERSION_STRING2:
		application_get_version_num_string(a, b, c, d); /* NOTE: the names don't match. */
		break;
		case APPLICATION_GET_BUILD_NUMBER:
		application_get_build_number(a, b, c, d);
		break;
		case APPLICATION_GET_GUID:
		application_get_guid(a, b, c, d);
		break;
#ifdef _WIN32
		case APPLICATION_GET_MAIN_THREAD_HANDLE:
		application_get_main_thread_handle(a, b, c, d);
		break;
		case APPLICATION_GET_HINSTANCE:
		application_get_hinstance(a, b, c, d);
		break;
#endif
		case APPLICATION_GET_COMMAND_LINE:
		application_get_command_line(a, b, c, d);
		break;
		case APPLICATION_SHUTDOWN:
		application_shutdown(a, b, c, d);
		break;
		case APPLICATION_CANCEL_SHUTDOWN:
		application_cancel_shutdown(a, b, c, d);
		break;
		case APPLICATION_IS_SHUTTING_DOWN:
		application_is_shutting_down(a, b, c, d);
		break;
		case APPLICATION_GET_APP_PATH:
		application_get_app_path(a, b, c, d);
		break;
		case APPLICATION_GET_USER_SETTINGS_PATH:
		application_get_user_settings_path(a, b, c, d);
		break;
		case APPLICATION_GET_INIT_COUNT:
		application_get_init_count(a, b, c, d);
		break;
		case APPLICATION_MESSAGE_LOOP_STEP:
		application_message_loop_step(a, b, c, d);
		break;
#ifdef _WIN32
		case APPLICATION_ADD_MESSAGE_PROCESSOR:
		application_add_message_processor(a, b, c, d);
		break;
		case APPLICATION_REMOVE_MESSAGE_PROCESSOR:
		application_remove_message_processor(a, b, c, d);
		break;
		case APPLICATION_ADD_MODELESS_DIALOG:
		application_add_modeless_dialog(a, b, c, d);
		break;
		case APPLICATION_REMOVE_MODELESS_DIALOG:
		application_remove_modeless_dialog(a, b, c, d);
		break;
#endif
		case APPLICATION_GET_WORKING_PATH:
		application_get_working_path(a, b, c, d);
		break;
		case APPLICATION_SET_WORKING_PATH:
		application_set_working_path(a, b, c, d);
		break;
		case APPLICATION_GET_MACHINE_ID:
		application_get_machine_id(a, b, c, d);
		break;
		case APPLICATION_GET_USER_ID:
		application_get_user_id(a, b, c, d);
		break;
		case APPLICATION_GET_SESSION_ID:
		application_get_session_id(a, b, c, d);
		break;
#ifdef _WIN32
		case APPLICATION_ADD_ACCELERATORS:
		application_add_accelerators(a, b, c, d);
		break;
		case APPLICATION_REMOVE_ACCELERATORS:
		application_remove_accelerators(a, b, c, d);
		break;
		case APPLICATION_TRANSLATE_ACCELERATORS:
		application_translate_accelerators(a, b, c, d);
		break;
		case APPLICATION_GET_ACCELERATORS:
		application_get_accelerators(a, b, c, d);
		break;
		case APPLICATION_REGISTER_GLOBAL_WINDOW:
		application_register_global_window(a, b, c, d);
		break;
		case APPLICATION_UNREGISTER_GLOBAL_WINDOW:
		application_unregister_global_window(a, b, c, d);
#endif
		break;
		case APPLICATION_ALLOCATE_THREAD_STORAGE:
		application_allocate_thread_storage(a, b, c, d);
		break;
		case APPLICATION_GET_THREAD_STORAGE:
		application_get_thread_storage(a, b, c, d);
		break;
		case APPLICATION_SET_THREAD_STORAGE:
		application_set_thread_storage(a, b, c, d);
		break;
		default:
		DEBUG_UNKNOWN_MESSAGE();
		return 1;
	}
	
	return 0;
}

struct cheese_dispatchable_vtable_s application_dispatchable_vtable = 
{
	&application_dispatch
};

struct cheese_application_s api_application = 
{
	.dispatchable_vpointer = &application_dispatchable_vtable
};
