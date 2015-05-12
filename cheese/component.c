/*
 *  yummy/cheese/component.c
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#include <cheese.h>
#include <platform.h>
#include "error.h"

void load_component(library_t lib)
{
	struct register_args_s {
		struct cheese_service_s **api_service;
	};
	
	struct cheese_component_s * (*get_component)();
	struct cheese_component_s *component;
	struct register_args_s register_args;
	struct cheese_service_s *api_service_p;
	
	get_component=(struct cheese_component_s * (*)()) get_symbol(lib, "GetWinamp5SystemComponent");
	if (!get_component)
		error(0, 0, "component :(");
	component=get_component();
	api_service_p = &api_service;
	register_args.api_service = &api_service_p;
	component->dispatchable_vpointer->_dispatch(COMPONENT_REGISTER_SERVICES, 0, (void **) &register_args, 1);
}
