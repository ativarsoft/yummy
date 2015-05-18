/*
 *  yummy/player/linux/discovery.c
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#include <sys/types.h>
#include <dirent.h>
#include <string.h>

#define PREFIX_IN  0x1
#define PREFIX_OUT 0x2
#define PREFIX_ 0x3
#define PREFIX_DSP 0x4
#define PREFIX_VIS 0x5
#define PREFIX_ENC 0x6

#define SUFFIX

void transverse_dir(char *path, void (*callback)(char *path, char *filename))
{
	DIR *dir;
	struct dirent *ent;
	int name_len;
	int mask;
	
	char *prefix[] = {
		"in_",
		"out_",
		"vis_"
	};
	
	char suffix[] = ".so";
	
	dir = opendir("plugins");
	for (;;) {
		ent = readdir(dir);
		if (ent == NULL)
			break;
		puts(ent->d_name);
		name_len = strlen(ent->d_name);
		/*strncmp();*/
	}
	(void) closedir(dir);
}
