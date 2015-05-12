/*
 *  yummy/plugins/gen/gen_dump.c
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#include <cheese.h>
#include <gen.h>
#include <stdint.h>
#include <stdio.h>
#include <user.h>
#include <windows.h>

uint32_t all[] = {
	SERVICE_TYPE_NONE,
	SERVICE_TYPE_UNIQUE,
	SERVICE_TYPE_OBJECT,
	SERVICE_TYPE_CONTEXT_COMMAND,
	SERVICE_TYPE_DEVICE,
	SERVICE_TYPE_FILE_READER,
	SERVICE_TYPE_FILE_SELECTOR,
	SERVICE_TYPE_STORAGE_VOLUME_ENUMERATOR,
	SERVICE_TYPE_IMAGE_GENERATOR,
	SERVICE_TYPE_IMAGE_LOADER,
	SERVICE_TYPE_IMAGE_WRITER,
	SERVICE_TYPE_ITEM_MANAGER,
	SERVICE_TYPE_PLAYLIST_READER,
	SERVICE_TYPE_PLAYLIST_WRITER,
	SERVICE_TYPE_MEDIA_CONVERTER,
	SERVICE_TYPE_MEDIA_CORE,
	SERVICE_TYPE_MEDIA_RECORDER,
	SERVICE_TYPE_SCRIPT_OBJECT,
	SERVICE_TYPE_TRANSLATOR,
	SERVICE_TYPE_WINDOW_CREATOR,
	SERVICE_TYPE_XML_PROVIDER,
	SERVICE_TYPE_DATABASE,
	SERVICE_TYPE_SKIN_FILTER,
	SERVICE_TYPE_METADATA,
	SERVICE_TYPE_METATAG,
	SERVICE_TYPE_ENUMERATOR,
	SERVICE_TYPE_MINIBROWSER,
	SERVICE_TYPE_TOOLTIP_RENDERER,
	SERVICE_TYPE_XML_GUI_OBJECT,
	SERVICE_TYPE_CHARACTER_ENCODING_CONVERTER,
	SERVICE_TYPE_ACTION,
	SERVICE_TYPE_CORE_ADMINISTRATOR,
	SERVICE_TYPE_DROP_TARGET,
	SERVICE_TYPE_OBJECT_DIRECTORY,
	SERVICE_TYPE_TEXT_FEED,
	SERVICE_TYPE_ACCESSIBILITY,
	SERVICE_TYPE_ACCESSIBILITY_ROLESERVER,
	SERVICE_TYPE_EXPORTER,
	SERVICE_TYPE_COLLECTION,
	SERVICE_TYPE_REDIRECTOR,
	SERVICE_TYPE_FONT_RENDERER,
	SERVICE_TYPE_CLASS_FACTORY,
	SERVICE_TYPE_SOURCE_EDITOR,
	SERVICE_TYPE_AAC_DECODER,
	SERVICE_TYPE_PLAYLIST_READER2,
	SERVICE_TYPE_PLAYLIST_WRITER2,
	SERVICE_TYPE_PLAYLIST_HANDLER,
	SERVICE_TYPE_TAG_PROVIDER,
	SERVICE_TYPE_NSV_FACTORY,
	SERVICE_TYPE_JAVASCRIPT_API_CREATOR,
	0
};

struct gen_plugin gen_dump;

static inline void printf_fourcc(uint32_t fourcc, char *a)
{
	char *b;

	b = (char *) &fourcc;
	a[0] = b[3];
	a[1] = b[2];
	a[2] = b[1];
	a[3] = b[0];
	b[4] = 0;
}

static int init()
{
	struct args_s {
		struct guid_s *guid;
	};

	struct args2_s {
		uint32_t *fourcc;
		size_t *n;
	};

	struct cheese_service *api_service;
	dispatch_t dispatch;
	struct args_s args;
	int r1;
	int i, j;
	char fourcc[4+1];
	struct cheese_service_header *header;
	struct args2_s args2;
	dispatch_t header_dispatch;
	struct guid_s guid;
	char *service_name;

	api_service = (struct cheese_service *) SendMessage(gen_dump.window, USER_GET_API_SERVICE, 0, 0);
	dispatch = api_service->dispatchable_vpointer->_dispatch;
	for(i=0; all[i]; i++) {
		dispatch(SERVICE_GET_NUM_SERVICES, &r1, (void **) &args, 1);
		get_fourcc(all[i], &fourcc);
		printf("%s: %d\n", fourcc, r1);
		for(j=0; j < r1; j++) {
			args2.fourcc = &all[i];
			args2.n = &j;
			dispatch(SERVICE_ENUM_SERVICE, &header, (void **) &args, 1);
			header_dispatch = header->dispatchable_vpointer->_dispatch;
			header_dispatch(SERVICE_HEADER_GET_GUID, &guid, 0, 0);
			DEBUG_PRINT_GUID(&guid);
			header_dispatch(SERVICE_HEADER_GET_SERVICE_NAME, &service_name, 0, 0);
			puts(service_name);
		}
	}
}

static void configure()
{
}

static void quit()
{
}

struct gen_plugin_s gen_dump = {
	.version = 1,
	.description = "Dumper",
	.init = &init,
	.configure = &configure,
	.quit = &quit,
};
