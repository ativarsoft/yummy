/*
 *  yummy/player/w32/transverse.c
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#include <windows.h>
#include <stdio.h>

#include <error.h>
#include <transverse.h>

void transverse_dir(char *path, void (*callback)(char *path, char *filename))
{
	WIN32_FIND_DATA find_data;
	HANDLE find_handle;
	int r;
	int path_len;
	char *file_path;
	int name_len;

	path_len = strlen(path);
	if (path_len + 2 > MAX_PATH)
		return;
	file_path = malloc(MAX_PATH);
	if(!file_path)
		error(0, 0, "ENOMEM");
	sprintf(file_path, "%s\\*", path);

	find_handle = FindFirstFile(file_path, &find_data);
	if (find_handle == INVALID_HANDLE_VALUE) {
		r = GetLastError();
		if(r == ERROR_NO_MORE_FILES)
			return; /* folder is empty. */
		else
			goto out;
	}

	do {
		printf("found %s\n", find_data.cFileName);
		name_len = strlen(find_data.cFileName);
		if (name_len < 7)
			continue;
		if (strcasecmp(find_data.cFileName + name_len - 4, ".dll") != 0)
			continue;
		if (path_len + strlen(find_data.cFileName) + 1 > MAX_PATH)
			continue;
		strcpy(file_path + path_len + 1, find_data.cFileName);
		callback(file_path, find_data.cFileName);
	} while (FindNextFile(find_handle, &find_data));

	r = GetLastError();
	if (r != ERROR_NO_MORE_FILES)
		goto out;

	(void) FindClose(find_handle);
	free(file_path);
	return;

out:
	(void) FindClose(find_handle);
	free(file_path);
	printf("Coludn't read contents from folder %s.\n", file_path);
	return;
}

#if 0
void watch_folders_for_changes()
{
	FindFirstChangeNotification(0, 0, 0);
}
#endif
