/*
 *  yummy/plugins/out/out_wmm.c
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

#include <out.h>
#include <module.h>
#include <mmsystem.h>
#include <debug.h>
#include <stdbool.h>
#include <msgbox.h>

#ifndef WMM_DEF_BUFFER_SIZE
#define WMM_DEF_BUFFER_SIZE 4096
#endif

#ifndef WMM_DEF_NUM_BLOCKS
#define WMM_DEF_NUM_BLOCKS 16
#endif

struct block_s {
	WAVEHDR wave_header;
	int pos;
};

static HWAVEOUT wave_out;

static int _panning;

static int in_block=0;
static int out_block=0;

static struct block_s *blocks;
static int buffer_size = WMM_DEF_BUFFER_SIZE; /* all blocks have the same size. */
static int num_blocks = WMM_DEF_NUM_BLOCKS;
static int free_blocks;

static void wmm_error(MMRESULT r)
{
	char a[MAXERRORLENGTH];
	
	waveOutGetErrorText(r, a, sizeof(a));
	DEBUG_PUTS(a);
}

static void configure(window_t parent)
{
	DEBUG_TRACE();
}

static void about(window_t parent)
{
	DEBUG_TRACE();
	
	info(parent, "Copyright 2013 Mateus de Lima Oliveira\n\n"
		"WinMM out plugin.");
}
	
static void init()
{
	DEBUG_TRACE();
	
	blocks = malloc(num_blocks * sizeof(struct block_s));
	
	/*waveOutReset();*/
	/*waveOutGetNumDevs();
	waveOutGetDevCaps();*/
}

static void quit()
{
	DEBUG_TRACE();
	
	free(blocks);
}

void CALLBACK callback(HWAVEOUT wave_out, UINT msg, DWORD_PTR instance, DWORD_PTR a, DWORD_PTR b)
{
	if (msg == WOM_DONE) {
		out_block++;
		if (out_block>=num_blocks)
			out_block=0;
		free_blocks++;
	}
}
	
static int open(int rate, int channels, int bits, int a, int b)
{
	MMRESULT r;
	WAVEFORMATEX format;
	int i;
	
	DEBUG_TRACE();
	DEBUG_PRINTF("rate: %d\n", rate);
	DEBUG_PRINTF("channels: %d\n", channels);
	DEBUG_PRINTF("bits: %d\n", bits);
	
	format.wFormatTag = WAVE_FORMAT_PCM;
	format.nChannels = channels;
	format.wBitsPerSample = bits;
	format.nBlockAlign = format.nChannels*format.wBitsPerSample/8;
	format.nSamplesPerSec = rate;
	format.nAvgBytesPerSec = format.nSamplesPerSec * format.nBlockAlign;
	format.cbSize = 0;
	
	r = waveOutOpen(&wave_out, WAVE_MAPPER, &format, (DWORD_PTR) &callback, 0, CALLBACK_FUNCTION);
	if (r != MMSYSERR_NOERROR) {
		wmm_error(r);
		return 0;
	}
	
	/*waveOutGetID(wave_out, 0);*/
	
	for(i=0; i<num_blocks; i++)
	{
		/* NOTE: the location of the buffer cannot be changed. */
		blocks[i].wave_header.lpData = malloc(buffer_size);
		blocks[i].wave_header.dwBufferLength = buffer_size;
		blocks[i].wave_header.dwFlags = 0;
		
		r = waveOutPrepareHeader(wave_out, &blocks[i].wave_header, sizeof(WAVEHDR));
		if (r != MMSYSERR_NOERROR) {
			wmm_error(r);
			return 0;
		}
		
		blocks[i].pos = 0;
	}
	
	free_blocks = num_blocks;
	
	return 1;
}

static void close()
{
	DEBUG_TRACE();
	
	/*waveOutUnprepareHeader();*/
	waveOutClose(wave_out);
}

static int write(char *buffer, int length)
{
	MMRESULT r;
	int avail;
	
	/*DEBUG_TRACE();
	DEBUG_PRINTF("in_block: %d\n", in_block);
	DEBUG_PRINTF("out_block: %d\n", out_block);
	DEBUG_PRINTF("out_block flags: %d\n", blocks[out_block].wave_header.dwFlags);
	DEBUG_PRINTF("length: %d\n", length);*/
	
	/* NOTE: the length must be less than the specified buffer size
	 * to waveOutPrepareHeader(). */
	/*blocks[in_block].wave_header.dwBufferLength = length;*/
	
	avail = buffer_size-blocks[in_block].pos;
	
	DEBUG_PRINTF("%d: %d/%d\n", in_block, blocks[in_block].pos, buffer_size);
	/*DEBUG_PRINTF("avail: %d\n", avail);*/
	
	if (length>=avail)
	{
		DEBUG_PUTS("a\n");
		memcpy(((char *)blocks[in_block].wave_header.lpData)+blocks[in_block].pos, buffer, avail);
		blocks[in_block].pos+=avail;
		free_blocks--;
		r = waveOutWrite(wave_out, &blocks[in_block].wave_header, sizeof(WAVEHDR));
		if (r != MMSYSERR_NOERROR)
			wmm_error(r);
		in_block++;
		if(in_block>=num_blocks)
			in_block=0;
		//memcpy(blocks[in_block].wave_header.lpData, buffer, length);
		//blocks[in_block].pos=length;
		memcpy(blocks[in_block].wave_header.lpData, buffer+avail, length-avail);
		blocks[in_block].pos=length-avail;
	}
	else
	{
		DEBUG_PUTS("_a\n");
		memcpy((char *) blocks[in_block].wave_header.lpData+blocks[in_block].pos, buffer, length);
		blocks[in_block].pos+=length;
		DEBUG_PUTS("_b\n");
	}
	return 0;
}

static int can_write()
{
	int n;
	
	if (in_block == out_block && free_blocks != num_blocks)
		return 0;
	n = (buffer_size - blocks[in_block].pos) +
		free_blocks * buffer_size;
	if (n < 8192)
		return n;
	else
		return 8192;
}

static int is_playing()
{
	if (blocks[out_block].wave_header.dwFlags & WHDR_DONE)
		return 0;
	return 1;
}

static int pause(int pause)
{
	MMRESULT r;
	
	/*r = waveOutPause(wave_out);
	if (r != MMSYSERR_NOERROR)
		wmm_error(r);
	r = waveOutRestart(wave_out);
	if (r != MMSYSERR_NOERROR)
		wmm_error(r);*/
	
	return 0xf; /* TODO: return time. */
}
	
static void set_volume(int volume)
{
	/*if (_panning>0)
		volume = (volume*_panning/128)<<8 | volume;
	else
		volume = volume<<8 | (volume*_panning/128);
	waveOutSetVolume(wave_out, volume);*/
	/*waveOutSetVolume(wave_out, 0xffff);*/
}

static void set_panning(int panning)
{
	/*_panning = panning;
	set_volume(255);*/
}

static void flush(int time)
{
}
	
static int get_output_time()
{
	return 0xf; /* TODO: ? */
}

static int get_written_time()
{
	return 0xf; /* TODO: ? */
}

struct out_plugin_s out_wmm = {
	.version = 0x10,
	.description = "WinMM",
	.id = 0,
	.configure = &configure,
	.about = &about,
	.init = &init,
	.quit = &quit,
	.open = &open,
	.close = &close,
	.write = &write,
	.can_write = &can_write,
	.is_playing = &is_playing,
	.pause = &pause,
	.set_volume = &set_volume,
	.set_panning = &set_panning,
	.flush = &flush,
	.get_output_time = &get_output_time,
	.get_written_time = &get_written_time
};

static void out_wmm_init()
{
}

EXPORT(out_wmm_init);

/* FIXME: remove. */
struct out_plugin_s * __declspec(dllexport) winampGetOutModule()
{
	return &out_wmm;
}
