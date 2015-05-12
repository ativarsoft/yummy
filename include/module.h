/*
 *  yummy/include/module.h
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#ifndef MODULE_H
#define MODULE_H

enum symbol_type_e {
	PLUGIN_IN,
	PLUGIN_OUT,
	PLUGIN_DSP,
	PLUGIN_VIS,
	PLUGIN_GEN,
	PLUGIN_ENC,
	CHEESE_SERVICE,
	CHEESE_COMPONENT
};

struct symbol_s {
	enum symbol_type_e type;
	void *addr;
};

extern const struct symbol_s __start_plugins[];
extern const struct symbol_s __stop_plugins[];
extern const struct symbol_s __start_cheese[];
extern const struct symbol_s __stop_cheese[];

#define EXPORT(symbol) struct symbol_s tab_##symbol = {0, symbol};

#endif
