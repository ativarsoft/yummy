/*
 *  yummy/plugins/lib/path.c
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#include <stdlib.h>
#include <string.h>
#include <libgen.h>

#include <path.h>

char * path_append(const char *a, const char *b)
{
	int len_dir, len_filename;
	char *a_copy;
	const char *dir, *filename;
	char *new_path;
	
	a_copy = strdup(a);
	dir = dirname(a_copy);
	filename = b;
	len_dir = strlen(dir);
	len_filename = strlen(filename);
	new_path = (char *) malloc(len_dir + len_filename + 2);
	strcpy(new_path, dir);
	new_path[len_dir] = '/';
	strcpy(new_path + len_dir + 1, filename);
	free(a_copy);
	return new_path;
}
