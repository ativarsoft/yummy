/*
 *  yummy/cheese/script/script.c
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#include <debug.h>
#include <cheese.h>
#include <stdlib.h> /* malloc */
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>

#include <cheese/script.h>

/* TODO: unload strings. */

#define SCRIPT_MAGIC ('F' | ('G' << 8))

#ifndef SCRIPT_STACK_SIZE
#define SCRIPT_STACK_SIZE 4096
#endif

#if defined DEBUG || 1
const char *types[] = {
	"void",
	"event",
	"int",
	"float",
	"double",
	"boolean",
	"string",
	"object",
	"any"
};
#endif

static int load_string(struct script_string *string, FILE *file)
{
	fread(&string->len, 2, 1, file);
	if (string->len < 0)
		return 0;
	string->data = malloc(string->len + 1);
	if(!string->data)
		return 0;
	fread(string->data, string->len, 1, file);
	/* security: strings are NOT null terminated. */
	string->data[string->len] = 0;

out:
	return string->len + 1;
}

static inline void unload_string(struct script_string *string)
{
	free(string->data);
}

/* Load the file structure into memory. No script code execution occurs. */
struct script * script_load(char *path)
{
	FILE *file;
	struct stript_header header;
	int i;
	int var_index;
	struct script_string dummy_string;
	struct script *script;

	file = fopen(path, "rb");
	if (!file) {
		fprintf(stderr, "ERROR: cannot open file '%s'.\n", path);
		fflush(stderr);
		return NULL;
	}
	/* objdect types that will be used. much like an import section. */
	fread(&header, 8, 1, file);
	if (header.magic != SCRIPT_MAGIC) {
		fprintf(stderr, "ERROR: wrong magic.\n");
		fflush(stderr);
		return NULL;
	}
	/*if (header.version > 1) {
		DEBUG_PUTS("unsupported version");
		return 0;
	}*/
	DEBUG_PRINTF("version: %#x\n", header.version);

	script = (struct script *) malloc(sizeof(*script));
	if (!script)
		return NULL;

	fread(&script->num_types, 4, 1, file);
	DEBUG_PRINTF("num types: %d\n", script->num_types);
	script->types = (struct guid_s *) malloc(16 * script->num_types);
	if (!script->types)
		return 0;
	fread(script->types, 16, script->num_types, file);
	for (i = 0; i < script->num_types; i++)
		debug_print_guid_c(script->types + i);

	fread(&script->num_function_names, 4, 1, file);
	DEBUG_PRINTF("num function names: %d\n", script->num_function_names);
	script->function_names = (struct script_function_name *) malloc(script->num_function_names * sizeof(*script->function_names));
	for (i=0; i<script->num_function_names; i++) {
		fread(&script->function_names[i].a, 2, 1, file);
		fread(&script->function_names[i].b, 2, 1, file);
		load_string(&script->function_names[i].string, file);
		DEBUG_PUTS(script->function_names[i].string.data);
	}

	fread(&script->num_variables, 4, 1, file);
	DEBUG_PRINTF("num variables: %d\n", script->num_variables);
	script->variables = (struct script_variable *) malloc(script->num_variables * sizeof(*script->variables));
	fread(script->variables, 14, script->num_variables, file);
	for (i = 0; i < script->num_variables; i++) {
		if (script->variables[i].type > NUM_TYPES)
			DEBUG_PUTS("ERROR: unknown type.");
		/* NOTE: this segfults on some scripts. */
		/*if (!script->variables[i].object)
			DEBUG_PUTS(types[script->variables[i].type]);*/
	}

	/* string literals, actually. */
	fread(&script->num_constants, 4, 1, file);
	DEBUG_PRINTF("num constants: %d\n", script->num_constants);
	script->constants = (struct script_constant *) malloc(script->num_constants * sizeof(*script->constants));
	if (!script->constants)
		return 0;
	for (i=0; i<script->num_constants; i++) {
		fread(&script->constants[i].var_index, 4, 1, file);
		/* check if the type of the variable is a string */
		if (script->variables[script->constants[i].var_index].type != SCRIPT_TYPE_STRING) {
			DEBUG_PUTS("ERROR: var is not string.");
			/* FIXME: free memory. */
			for (; i >= 0; i--)
				unload_string(&script->constants[i].value);
			return 0;
		}
		if (!load_string(&script->constants[i].value, file)) {
			DEBUG_PUTS("bad string");
			return 0;
		}
		DEBUG_PUTS(script->constants[i].value.data);
	}

	fread(&script->num_functions, 4, 1, file);
	DEBUG_PRINTF("num functions: %d\n", script->num_functions);
	script->functions = (struct script_function *) malloc(script->num_functions * sizeof(*script->functions));
	fread(script->functions, 12, script->num_functions, file);
	fread(&script->num_opcodes, 4, 1, file);
	DEBUG_PRINTF("num opcodes: %d\n", script->num_opcodes);
	script->opcodes = (char *) malloc(script->num_opcodes);
	fread(script->opcodes, script->num_opcodes, 1, file);
	fclose(file);

	/*script->stack = malloc(SCRIPT_STACK_SIZE);
	if (!script->stack)
		return 0;
	script->sp = 0;*/

	return script;
}

void script_unload(struct script *script)
{
	int i;

	for (i = 0; i < script->num_function_names; i++)
		unload_string(&script->function_names[i].string);
	for (i = 0; i < script->num_constants; i++)
		unload_string(&script->constants[i].value);
	free(script->types);
	free(script->function_names);
	free(script->variables);
	free(script->constants);
	free(script->functions);
	free(script->opcodes);
	free(script);
}

/* when an event occurs (e.g. loading complete),
 * use this function to execute the event callback. */
int script_get_function(struct script *script, char *function)
{
	int i;
	int index;
	
	for (i = 0; i < script->num_functions; i++) {
		index = script->functions[i].func_num;
		if (strcasecmp(script->function_names[index].string, function) == 0)
			return script->functions[index].offset;
	}
	
	return -1;
}
