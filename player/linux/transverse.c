/*
 *  yummy/player/linux/transverse.c
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#include <sys/types.h>
#include <dirent.h>
#include <string.h>

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
