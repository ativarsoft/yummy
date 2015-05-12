/*
 *  yummy/include/cheese.h
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#ifndef CHEESE_H
#define CHEESE_H

#include <stdint.h>

#ifndef STDCALL
#ifdef _WIN32
#define STDCALL __attribute__((stdcall))
#else
#define STDCALL
#endif
#endif

#define FOURCC(a, b, c, d) (d<<24 | c<<16 | b<<8 | a)

#define SERVICE_REGISTER               10
#define SERVICE_UNREGISTER             20
#define SERVICE_GET_NUM_SERVICES       30
#define SERVICE_ENUM_SERVICE           40
#define SERVICE_GET_SERVICE_BY_GUID    50
#define SERVICE_LOCK                   60
#define SERVICE_CLIENT_LOCK            70
#define SERVICE_RELEASE                80
#define SERVICE_GET_TYPE_NAME          90
#define SERVICE_GET_OWNING_COMPONENT  100
#define SERVICE_GET_LOCKING_COMPONENT 110
#define SERVICE_UNLOCK                120
#define SERVICE_IS_VALID              130
#define SERVICE_COMPACT_DUPLICATES    140

#define SERVICE_TYPE_NONE                         FOURCC('n', 'o', 'n', 'e')
#define SERVICE_TYPE_UNIQUE                       FOURCC('u', 'n', 'i', 'q')
#define SERVICE_TYPE_OBJECT                       FOURCC('o', 'b', 'j', 'f')
#define SERVICE_TYPE_CONTEXT_COMMAND              FOURCC('c', 'c', 'm', 'd')
#define SERVICE_TYPE_DEVICE                       FOURCC('d', 'e', 'v', ' ')
#define SERVICE_TYPE_FILE_READER                  FOURCC('f', 's', 'r', 'd')
#define SERVICE_TYPE_FILE_SELECTOR                FOURCC('f', 's', 'e', 'l')
#define SERVICE_TYPE_STORAGE_VOLUME_ENUMERATOR    FOURCC('f', 's', 'e', 'n')
#define SERVICE_TYPE_IMAGE_GENERATOR              FOURCC('i', 'm', 'g', 'n')
#define SERVICE_TYPE_IMAGE_LOADER                 FOURCC('i', 'm', 'g', 'l')
#define SERVICE_TYPE_IMAGE_WRITER                 FOURCC('i', 'm', 'g', 'w')
#define SERVICE_TYPE_ITEM_MANAGER                 FOURCC('i', 'm', 'g', 'r')
#define SERVICE_TYPE_PLAYLIST_READER              FOURCC('p', 'l', 'r', 'd')
#define SERVICE_TYPE_PLAYLIST_WRITER              FOURCC('p', 'l', 'w', 'r')
#define SERVICE_TYPE_MEDIA_CONVERTER              FOURCC('c', 'o', 'n', 'v')
#define SERVICE_TYPE_MEDIA_CORE                   FOURCC('c', 'o', 'r', 'e')
#define SERVICE_TYPE_MEDIA_RECORDER               FOURCC('m', 'r', 'e', 'c')
#define SERVICE_TYPE_SCRIPT_OBJECT                FOURCC('m', 'a', 'k', 'i')
#define SERVICE_TYPE_TRANSLATOR                   FOURCC('x', 'l', 'a', 't')
#define SERVICE_TYPE_WINDOW_CREATOR               FOURCC('w', 'n', 'd', 'c')
#define SERVICE_TYPE_XML_PROVIDER                 FOURCC('x', 'm', 'l', 'p')
#define SERVICE_TYPE_DATABASE                     FOURCC('d', 'b', ' ', ' ')
#define SERVICE_TYPE_SKIN_FILTER                  FOURCC('f', 'l', 't', 'r')
#define SERVICE_TYPE_METADATA                     FOURCC('m', 't', 'd', 't')
#define SERVICE_TYPE_METATAG                      FOURCC('m', 't', 't', 'g')
#define SERVICE_TYPE_ENUMERATOR                   FOURCC('e', 'v', 'a', 'l')
#define SERVICE_TYPE_MINIBROWSER                  FOURCC('m', 'b', ' ', ' ')
#define SERVICE_TYPE_TOOLTIP_RENDERER             FOURCC('t', 't', 'i', 'p')
#define SERVICE_TYPE_XML_GUI_OBJECT               FOURCC('x', 'u', 'i', 'o')
#define SERVICE_TYPE_CHARACTER_ENCODING_CONVERTER FOURCC('u', 't', 'f', '8')
#define SERVICE_TYPE_ACTION                       FOURCC('a', 'c', 't', ' ')
#define SERVICE_TYPE_CORE_ADMINISTRATOR           FOURCC('c', 'a', 'd', 'm')
#define SERVICE_TYPE_DROP_TARGET                  FOURCC('d', 'r', 'o', 'p')
#define SERVICE_TYPE_OBJECT_DIRECTORY             FOURCC('o', 'b', 'j', 'd')
#define SERVICE_TYPE_TEXT_FEED                    FOURCC('t', 'x', 't', 'f')
#define SERVICE_TYPE_ACCESSIBILITY                FOURCC('a', 'c', 'c', 's')
#define SERVICE_TYPE_ACCESSIBILITY_ROLESERVER     FOURCC('r', 'o', 'l', 'e')
#define SERVICE_TYPE_EXPORTER                     FOURCC('e', 'x', 'p', ' ')
#define SERVICE_TYPE_COLLECTION                   FOURCC('c', 'l', 'c', 't')
#define SERVICE_TYPE_REDIRECTOR                   FOURCC('r', 'e', 'd', 'r')
#define SERVICE_TYPE_FONT_RENDERER                FOURCC('f', 'o', 'n', 't')
#define SERVICE_TYPE_CLASS_FACTORY                FOURCC('c', 'l', 'f', 'a')
#define SERVICE_TYPE_SOURCE_EDITOR                FOURCC('s', 'e', 'd', 't')
#define SERVICE_TYPE_AAC_DECODER                  FOURCC('m', '4', 'a', 'd')
#define SERVICE_TYPE_PLAYLIST_READER2             FOURCC('p', 'l', 'r', '5')
#define SERVICE_TYPE_PLAYLIST_WRITER2             FOURCC('p', 'l', 'w', '5')
#define SERVICE_TYPE_PLAYLIST_HANDLER             FOURCC('p', 'l', 'h', ' ')
#define SERVICE_TYPE_TAG_PROVIDER                 FOURCC('t', 'a', 'g', 'z')
#define SERVICE_TYPE_NSV_FACTORY                  FOURCC('n', 's', 'v', 'f')
#define SERVICE_TYPE_JAVASCRIPT_API_CREATOR       FOURCC('j', 's', 'a', 'c')

#define SERVICE_HEADER_GET_SERVICE_TYPE                  100
#define SERVICE_HEADER_GET_SERVICE_NAME                  200
#define SERVICE_HEADER_GET_GUID                          210
#define SERVICE_HEADER_GET_INTERFACE                     300
#define SERVICE_HEADER_SUPPORT_NON_LOCKING_GET_INTERFACE 301
#define SERVICE_HEADER_RELEASE_INTERFACE                 310
#define SERVICE_HEADER_GET_TEST_STRING                   500
#define SERVICE_HEADER_SERVICE_NOTIFY                    600

#define COMPONENT_REGISTER_SERVICES   10
#define COMPONENT_UNREGISTER_SERVICES 20

typedef int STDCALL (*dispatch_t)(int, void *, void **, int);

struct cheese_dispatchable_vtable {
	dispatch_t _dispatch;
};

struct dispatchable {
	struct cheese_dispatchable_vtable *dispatchable_vpointer;
};

struct cheese_service {
	struct cheese_dispatchable_vtable *dispatchable_vpointer;
	struct service_vtable {
	} *service_vtable;
};

struct cheese_service_header {
	struct cheese_dispatchable_vtable *dispatchable_vpointer;
	struct header_vtable {
	} *service_header_vtable;
};

struct cheese_component {
	struct cheese_dispatchable_vtable *dispatchable_vpointer;
	struct component_vtable {
	} *component_vpointer;
	void *module;
};

/* FIXME: remove. */
struct cheese_factory_list {
	struct cheese_factory_list *next;
	struct cheese_factory *factory;
};

typedef int bool_t; /* FIXME: remove. */
typedef uint32_t ARGB; /* FIXME: remove. */

extern struct cheese_service api_service;
extern struct cheese_factory service_factory;

#endif
