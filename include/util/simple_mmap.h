
#ifndef SIMPLE_MMAP_H
#define SIMPLE_MMAP_H

#ifdef _WIN32
#include <windows.h>

struct map_s {
	HANDLE file;
	HANDLE mapping;
	void *p;
};
#else
struct map_s {
	int file;
	int length;
	void *p;
};
#endif

void * simple_mmap(struct map_s *map, char *path);
void simple_munmap(struct map_s *map, void *addr);

#endif
