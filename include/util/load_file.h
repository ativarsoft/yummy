
#ifndef LOAD_FILE_H
#define LOAD_FILE_H

#include <windows.h>

DWORD load_whole_file(const char *path, void **buffer);

static inline void unload_whole_file(void **buffer)
{
	free(*buffer);
}

#endif
