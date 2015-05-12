/*
 *  yummy/player/zip.c
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#include <zlib.h>
#include <debug.h>
#include "util/simple_mmap.h"

#define ZIP_FILE_HEADER_MAGIC 0x04034b50

struct zip_file_header_s
{
	unsigned int signature;
	unsigned short version_required;
	unsigned short flags;
	unsigned short method;
	unsigned short modification_time;
	unsigned short modification_date;
	unsigned int crc32;
	unsigned int compressed_size;
	unsigned int uncompressed_size;
	unsigned short filename_length;
	unsigned short extra_field_length;
} __attribute__((packed));

void _unzip(struct zip_file_header_s *zip)
{
	z_stream z;
	if(zip->signature!=ZIP_FILE_HEADER_MAGIC)
		return;
	DEBUG_PRINTF("%.*s\n", zip->filename_length, (char *)(zip)+sizeof(struct zip_file_header_s));
	if(zip->method!=0&&zip->method!=Z_DEFLATED
#ifdef HAVE_BZIP2
	&&zip->method!=Z_BZIP2ED
#endif
	)
		return;
	ZeroMemory(&z, sizeof(z_stream));
	inflateInit2(&z, -MAX_WBITS);
	z.avail_in=zip->compressed_size;
	z.next_in=(void *)(zip)+sizeof(struct zip_file_header_s)+zip->filename_length+zip->extra_field_length;
	z.avail_out=zip->uncompressed_size;
	z.next_out=malloc(zip->uncompressed_size);
	if(!z.next_out)
		return;
	if(inflate(&z, Z_NO_FLUSH)!=Z_STREAM_END)
		return;
	inflateEnd(&z);
}

int unzip()
{
	struct mmaped_file_s mmaped_zip;
	simple_mmap(&mmaped_zip, "test.zip");
	unzip(mmaped_zip.p);
	simple_munmap(&mmaped_zip);
	return 0;
}
