/*
 *  yummy/cheese/script/interpreter.c
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <errno.h>

#include <cheese/script.h>

struct machine_state {
	int stack[32];
	int stack_pos;

	int *code;
	int ip;
	int code_size;

	/* global variables */
	int *mem;
	int mem_size;
};

struct instruction {
	uint32_t opcode;
	void (*handler)(struct script_instance *p, struct machine_state *state);
};

static uint32_t get_operand(struct machine_state *state)
{
}

static void opcode_push(struct script_instance *p, struct machine_state *state)
{
	uint32_t a;

	a = get_operand(state);
	if (state->stack_pos < 31) {
		state->stack_pos++;
		state->stack[state->stack_pos] = a;
	} else {
		fprintf(stderr, "ERROR: stack overflow.\n");
		fflush(stderr);
	}
}

static void opcode_pop(struct script_instance *p, struct machine_state *state)
{
	if (state->stack_pos > 0) {
		state->stack_pos--;
	} else {
		fprintf(stderr, "ERROR: stack underflow.\n");
		fflush(stderr);
	}
}

static void opcode_popto(struct script_instance *p, struct machine_state *state)
{
	uint32_t a;

	a = get_operand(state);
}

static void opcode_eq(struct script_instance *p, struct machine_state *state)
{
}

static void opcode_heq(struct script_instance *p, struct machine_state *state)
{
}

static void opcode_le(struct script_instance *p, struct machine_state *state)
{
}

static void opcode_leq(struct script_instance *p, struct machine_state *state)
{
}

static void opcode_gt(struct script_instance *p, struct machine_state *state)
{
}

static void opcode_gtg(struct script_instance *p, struct machine_state *state)
{
}

static void opcode_jumpif(struct script_instance *p, struct machine_state *state)
{
	uint32_t a;

	a = get_operand(state);
}

static void opcode_jumpifnot(struct script_instance *p, struct machine_state *state)
{
	uint32_t a;

	a = get_operand(state);
}

static void opcode_jump(struct script_instance *p, struct machine_state *state)
{
	uint32_t a;

	a = get_operand(state);
}

static void opcode_call(struct script_instance *p, struct machine_state *state)
{
	uint32_t a;

	a = get_operand(state);
}

static void opcode_callglobal(struct script_instance *p, struct machine_state *state)
{
	uint32_t a;

	a = get_operand(state);
}

static void opcode_ret(struct script_instance *p, struct machine_state *state)
{
}

static void opcode_complete(struct script_instance *p, struct machine_state *state)
{
}

static void opcode_mov(struct script_instance *p, struct machine_state *state)
{
}

static void opcode_postinc(struct script_instance *p, struct machine_state *state)
{
}

static void opcode_postdec(struct script_instance *p, struct machine_state *state)
{
}

static void opcode_preinc(struct script_instance *p, struct machine_state *state)
{
}

static void opcode_predec(struct script_instance *p, struct machine_state *state)
{
}

static void opcode_add(struct script_instance *p, struct machine_state *state)
{
}

static void opcode_sub(struct script_instance *p, struct machine_state *state)
{
}

static void opcode_mul(struct script_instance *p, struct machine_state *state)
{
}

static void opcode_div(struct script_instance *p, struct machine_state *state)
{
}

static void opcode_mod(struct script_instance *p, struct machine_state *state)
{
}

static void opcode_and(struct script_instance *p, struct machine_state *state)
{
}

static void opcode_or(struct script_instance *p, struct machine_state *state)
{
}

static void opcode_not(struct script_instance *p, struct machine_state *state)
{
}

static void opcode_negative(struct script_instance *p, struct machine_state *state)
{
}

static void opcode_logand(struct script_instance *p, struct machine_state *state)
{
}

static void opcode_logor(struct script_instance *p, struct machine_state *state)
{
}

static void opcode_lshift(struct script_instance *p, struct machine_state *state)
{
}

static void opcode_rshift(struct script_instance *p, struct machine_state *state)
{
}

static void opcode_new(struct script_instance *p, struct machine_state *state)
{
}

static void opcode_delete(struct script_instance *p, struct machine_state *state)
{
}

static void opcode_strangecall(struct script_instance *p, struct machine_state *state)
{
	uint32_t a;

	a = get_operand(state);
}

/* name opcode num_operands */
static const struct instruction instructions[] = {
	{0x01, &opcode_push},
	{0x02, &opcode_pop},
	{0x03, &opcode_popto},

	{0x08, &opcode_eq},
	{0x09, &opcode_heq},
	{0x0A, &opcode_le},
	{0x0B, &opcode_leq},
	{0x0C, &opcode_gt},
	{0x0D, &opcode_gtg},

	{0x10, &opcode_jumpif},
	{0x11, &opcode_jumpifnot},
	{0x12, &opcode_jump},

	{0x18, &opcode_call},
	{0x19, &opcode_callglobal},

	{0x21, &opcode_ret},

	{0x28, &opcode_complete},

	{0x30, &opcode_mov},

	{0x38, &opcode_postinc},
	{0x39, &opcode_postdec},
	{0x40, &opcode_preinc},
	{0x41, &opcode_predec},

	{0x40, &opcode_add},
	{0x41, &opcode_sub},
	{0x42, &opcode_mul},
	{0x43, &opcode_div},
	{0x44, &opcode_mod},

	{0x48, &opcode_and},
	{0x49, &opcode_or},
	{0x4A, &opcode_not},
	{0x4C, &opcode_negative},

	{0x50, &opcode_logand},
	{0x51, &opcode_logor},
	{0x5A, &opcode_lshift},
	{0x5B, &opcode_rshift},

	{0x60, &opcode_new},
	{0x61, &opcode_delete},

	{0x70, &opcode_strangecall}
};

static inline int stack_check(int in, int out)
{
	int r;
	
	/*if (sp + in * 4 > SCRIPT_STACK_SIZE)
		return 1;
	if (sp - )*/
}

int interpreter_exec(struct script *script, char *function)
{
#if 0
	int addr;
	void *stack;
	const struct instruction *p;
	struct script_instance inst;
	struct machine_state state;
	
	addr = script_get_function(script, function);
	for (;;) {
		if (addr == 0)
			break;

		/* decode instruction */
		for (p = instructions; p != NULL; p++)
			if (p->opcode == cur_ins) {
				/* execute instruction */
				p->handler(&inst, &state);
				break;
		}

		if (p == NULL)
			return -EFAULT;
	}

	return 0;
#endif
}
