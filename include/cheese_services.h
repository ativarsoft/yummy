/*
 *  yummy/include/cheese_services.h
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#ifndef SERVICES_H
#define SERVICES_H

#define ALBUMART_PROVIDER_TYPE         0
#define ALBUMART_GET_ALBUMART_DATA    10
#define ALBUMART_IS_MINE              20
#define ALBUMART_SET_ALBUMART_DATA    30
#define ALBUMART_DELETE_ALBUMART_DATA 40
#define ALBUMART_SUCCESS               0
#define ALBUMART_FAILURE               1
#define ALBUMART_RDONLY                2
#define ALBUMART_EMBEDDED              0
#define ALBUMART_DATABASE              1
#define ALBUMART_FOLDER                2

#define APPLICATION_GET_APP_NAME              10
#define APPLICATION_GET_VERSION_STRING        20
#define APPLICATION_GET_VERSION_STRING2       21
#define APPLICATION_GET_BUILD_NUMBER          30
#define APPLICATION_GET_GUID                  40
#define APPLICATION_GET_MAIN_THREAD_HANDLE    50
#define APPLICATION_GET_HINSTANCE             60
#define APPLICATION_GET_COMMAND_LINE          70
#define APPLICATION_SHUTDOWN                  80
#define APPLICATION_CANCEL_SHUTDOWN           90
#define APPLICATION_IS_SHUTTING_DOWN         100
#define APPLICATION_GET_APP_PATH             110
#define APPLICATION_GET_USER_SETTINGS_PATH   120
#define APPLICATION_GET_INIT_COUNT           130
#define APPLICATION_MESSAGE_LOOP_STEP        140
#define APPLICATION_ADD_MESSAGE_PROCESSOR    150
#define APPLICATION_REMOVE_MESSAGE_PROCESSOR 160
#define APPLICATION_ADD_MODELESS_DIALOG      170
#define APPLICATION_REMOVE_MODELESS_DIALOG   180
#define APPLICATION_GET_WORKING_PATH         190
#define APPLICATION_SET_WORKING_PATH         200
#define APPLICATION_GET_MACHINE_ID           210
#define APPLICATION_GET_USER_ID              220
#define APPLICATION_GET_SESSION_ID           230
#define APPLICATION_ADD_ACCELERATORS         240
#define APPLICATION_REMOVE_ACCELERATORS      250
#define APPLICATION_TRANSLATE_ACCELERATORS   260
#define APPLICATION_GET_ACCELERATORS         280
#define APPLICATION_REGISTER_GLOBAL_WINDOW   290
#define APPLICATION_UNREGISTER_GLOBAL_WINDOW 300
#define APPLICATION_ALLOCATE_THREAD_STORAGE  310
#define APPLICATION_GET_THREAD_STORAGE       320
#define APPLICATION_SET_THREAD_STORAGE       330

#define CONFIG_GET_GROUP              10
#define CONFIG_REGISTER_GROUP         20
#define CONFIG_STATUS_SUCCESS          0
#define CONFIG_STATUS_FAILURE          1
#define CONFIG_STATUS_GROUP_NOT_FOUND  2
#define CONFIG_STATUS_ITEM_NOT_FOUND   3

#define CORE_GET_SUPPORTED_EXTENSIONS      0
#define CORE_GET_EXT_SUPPORTED_EXTENSIONS 10
#define CORE_CREATE                       20
#define CORE_FREE                         30
#define CORE_SET_NEXT_FILE                40
#define CORE_GET_STATUS                   50
#define CORE_GET_CURRENT                  60
#define CORE_GET_CUR_PLAYBACK_NUMBER      70
#define CORE_GET_POSITION                 80
#define CORE_GET_WRITE_POSITION           90
#define CORE_SET_POSITION                100
#define CORE_GET_LENGTH                  110
#define CORE_GET_PLUGIN_DATA             120
#define CORE_GET_VOLUME                  130
#define CORE_SET_VOLUME                  140
#define CORE_GET_PAN                     150
#define CORE_SET_PAN                     160
#define CORE_ADD_CALLBACK                170
#define CORE_DEL_CALLBACK                180
#define CORE_GET_VIS_DATA                190
#define CORE_GET_LEFT_VU_METER           200
#define CORE_GET_RIGHT_VU_METER          210
#define CORE_REGISER_SEQUENCER           220
#define CORE_UNREGISTER_SEQUENCER        230
#define CORE_USER_BUTTON                 240
#define CORE_GET_EQ_STATUS               250
#define CORE_SET_EQ_STATUS               260
#define CORE_GET_EQ_PREAMP               270
#define CORE_SET_EQ_PREAMP               280
#define CORE_GET_EQ_BAND                 290
#define CORE_SET_EQ_BAND                 300
#define CORE_GET_EQ_AUTO                 310
#define CORE_SET_EQ_AUTO                 320
#define CORE_CUSTOM_MSG                  330
#define CORE_REGISTER_EXTENSION          340
#define CORE_GET_EXTENSION_FAMILY        350
#define CORE_UNREGISTER_EXTENSION        360
#define CORE_GET_TITLE                   370
#define CORE_SET_TITLE                   380
#define CORE_GET_DECODER_NAME            390
#define CORE_GET_RATING                  400
#define CORE_SET_RATING                  410

#define IMAGE_LOADER_IS_MINE          50
#define IMAGE_LOADER_GET_HEADER_SIZE 100
#define IMAGE_LOADER_TEST_DATA       200
#define IMAGE_LOADER_GET_DIMENTIONS  300
#define IMAGE_LOADER_LOAD_IMAGE      400
#define IMAGE_LOADER_LOAD_IMAGE_DATA 500

#define LANGUAGE_GET_STRING                    10
#define LANGUAGE_GET_STRING_UTF16              11
#define LANGUAGE_GET_STRING_FROM_GUID          12
#define LANGUAGE_GET_STRING_FROM_GUID_UTF16    13
#define LANGUAGE_GET_INSTANCE_BY_GUID          20
#define LANGUAGE_GET_INSTANCE_BY_NAME          21
#define LANGUAGE_GET_INSTANCE_BY_NAME_UTF16    22
#define LANGUAGE_STARTUP                       30
#define LANGUAGE_SHUTDOWN                      31
#define LANGUAGE_GET_LANGUAGE_FOLDER           40
#define LANGUAGE_CREATE_DIALOG_PARAM           50
#define LANGUAGE_DIALOG_LPARAM                 51
#define LANGUAGE_LOAD_MENU                     52
#define LANGUAGE_CREATE_DIALOG_LPARAM          53
#define LANGUAGE_DIALOG_LPARAM_UTF16           54
#define LANGUAGE_LOAD_MENU_UTF16               55
#define LANGUAGE_GET_LANGUAGE_IDENTIFIER       60
#define LANGUAGE_LOAD_RESOURCE_FROM_FILE       70
#define LANGUAGE_LOAD_RESOURCE_FROM_FILE_UTF16 71
#define LANGUAGE_LOAD_ACCELERATORS             80
#define LANGUAGE_LOAD_ACCELERATORS_UTF16       81

#define MEMORY_ALLOC    0
#define MEMORY_FREE    10
#define MEMORY_REALLOC 20
#define MEMORY_CHANGED 30

#define SCRIPT_PUSH_OBJECT           10
#define SCRIPT_PUSH_INT              20
#define SCRIPT_PUSH_BOOLEAN          30
#define SCRIPT_PUSH_FLOAT            40
#define SCRIPT_PUSH_DOUBLE           50
#define SCRIPT_PUSH_STRING           60
#define SCRIPT_PUSH_VOID             70
#define SCRIPT_PUSH_ANY              80
#define SCRIPT_POP_OBJECT            90
#define SCRIPT_POP_INT              100
#define SCRIPT_POP_BOOLEAN          110
#define SCRIPT_POP_FLOAT            120
#define SCRIPT_POP_DOUBLE           130
#define SCRIPT_POP_STRING           140
#define SCRIPT_POP_ANY              150
#define SCRIPT_POP_DISCARD          160
#define SCRIPT_GET_FUNCTION         170
#define SCRIPT_ADD_DLF_REF          180
#define SCRIPT_ADD_DLF_CLASS_REF    190
#define SCRIPT_REM_DLF_REF          200
#define SCRIPT_CALL_FUNCTION        210
#define SCRIPT_TRIGGER_EVENT        220
#define SCRIPT_GET_SCRIPT_INT       230
#define SCRIPT_GET_SCRIPT_BOOLEAN   240
#define SCRIPT_GET_SCRIPT_FLOAT     250
#define SCRIPT_GET_SCRIPT_DOUBLE    260
#define SCRIPT_GET_SCRIPT_STRING    270
#define SCRIPT_GET_SCRIPT_OBJECT    280
#define SCRIPT_UPDATE_DLF           290
#define SCRIPT_INSTANTIATE          300
#define SCRIPT_DESTROY              310
#define SCRIPT_ENCAPSULATE          320
#define SCRIPT_DEENCAPSULATE        330
#define SCRIPT_GET_CONTROLLER       340
#define SCRIPT_CREATE_ORPHAN        350
#define SCRIPT_KILL_ORPHAN          360
#define SCRIPT_SET_OBJECT_ATOM      370
#define SCRIPT_GET_OBJECT_ATOM      380
#define SCRIPT_FIND_OBJECT          390
#define SCRIPT_ADD_SCRIPT_OBJECT    400
#define SCRIPT_REMOVE_SCRIPT_OBJECT 410
#define SCRIPT_GET_CACHE_COUNT      420
#define SCRIPT_IS_VALID_SCRIPT_ID   430
#define SCRIPT_MAP_VARIABLE_ID      440
#define SCRIPT_GET_USER_ANCESTOR_ID 450
#define SCRIPT_GET_NUM_EVENTS       460
#define SCRIPT_GET_EVENT            470
#define SCRIPT_GET_COMPLETE         480
#define SCRIPT_SET_COMPLETE         481
#define SCRIPT_RESET_COMPLETE       482
#define SCRIPT_GET_CLASS_NAME       490

struct cheese_core_vtable_s
{
};

struct cheese_core_s
{
	struct cheese_dispatchable_vtable_s *dispatchable_vpointer;
	/*struct cheese_core_vtable_s *core_vpointer;*/
};

struct cheese_application_vtable_s
{
};

struct cheese_application_s
{
	struct cheese_dispatchable_vtable_s *dispatchable_vpointer;
	/*struct cheese_application_vtable_s *application_vpointer;*/
};

struct cheese_language_vtable_s
{
};

struct cheese_language_s
{
	struct cheese_dispatchable_vtable_s *dispatchable_vpointer;
	/*struct cheese_language_vtable_s *language_vpointer;*/
};

extern struct cheese_service_s service_dummy;
extern struct cheese_service_header_s service_header_dummy;
extern struct cheese_core_s service_core;
extern struct cheese_core_s service_header_core;
extern struct cheese_application_s service_application;
extern struct cheese_application_s service_header_application;
extern struct cheese_language_s service_language;
extern struct cheese_language_s service_header_language;

#endif
