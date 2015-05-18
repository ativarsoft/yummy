/*
 *  yummy/player/linux/main.c
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <argp.h>
#include <getopt.h>
#include <gui.h>
#include "configurations.h"
#include "error.h"
#include "../init.h"

struct arguments
{
	char *filename;
};

static struct argp_option options[] = 
{
	{0}
};

static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
	struct arguments *arguments;
	
	arguments = state->input;
	
	switch(key) {
		case ARGP_KEY_ARG:
		if(state->arg_num>=1)
			argp_usage(state);
		arguments->filename=arg;
		break;
		case ARGP_KEY_END:
		if(state->arg_num<1)
			argp_usage(state);
		break;
		default:
		return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

static const char doc[] = {"player"};

static struct argp argp = {options, parse_opt, 0, doc};

int main(int argc, char **argv)
{
	struct script *script;
#if 0
	struct arguments_s arguments;
	
	argp_parse(&argp, argc, argv, 0, 0, &arguments);
#endif
	player_init();
	
	return 0;
}
