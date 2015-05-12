#ifndef SCRIPT_H
#define SCRIPT_H

enum script_type
{
	SCRIPT_TYPE_VOID, 
	SCRIPT_TYPE_EVENT, 
	SCRIPT_TYPE_INT, 
	SCRIPT_TYPE_FLOAT, 
	SCRIPT_TYPE_DOUBLE, 
	SCRIPT_TYPE_BOOLEAN, 
	SCRIPT_TYPE_STRING, 
	SCRIPT_TYPE_OBJECT, 
	SCRIPT_TYPE_ANY,
	NUM_TYPES
};

struct script_string
{
	uint16_t len;
	char *data;
};

struct stript_header
{
	short magic;
	short version;
	int a;
};

struct script_function_name
{
	int a;
	int b;
	struct script_string string;
};

struct script_variable
{
	char type;
	char object; /* boolean */
	short subclass;
	short unk1;
	short unk2;
	short unk3;
	short unk4;
	char global;
	char system;
};

struct script_constant
{
	int var_index;
	struct script_string value;
};

struct script_function
{
	int var_num;
	int func_num;
	int offset;
};

struct script {
	int num_types;
	int num_function_names;
	int num_variables;
	int num_constants;
	int num_functions;
	int num_opcodes; /* FIXME: code size. */
	struct guid_s *types;
	struct script_function_name *function_names;
	struct script_variable *variables;
	struct script_constant *constants;
	struct script_function *functions;
	unsigned char *opcodes;
	
	void **pointers;
	void *mutex;
};

struct script_instance {
	struct script *script;
	int *params;
	int *variables;
};

struct script * script_load(char *path);
void script_unload(struct script *script);
int script_get_function(struct script *script, char *function);
int interpreter_exec(struct script *script, char *function);

#endif
