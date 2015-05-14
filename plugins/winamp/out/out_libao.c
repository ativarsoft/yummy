#include <out.h>
#include <ao/ao.h>
#include <module.h>
#include <debug.h>
#include <msgbox.h>
#include <stdlib.h>

static int driver_id;
static ao_device *device;

/* TODO: not used. */
static struct {
	char *name;
	char *matrix;
} m[] = {
	{"Mono", "M"},
	{"Stereo", "L,R"},
	{"Quadriphonic", "L,R,BL,BR"},
	{"5.1", "L,C,R,LFE,BR,BL"},
	{"7.1", "L,C,R,LFE,BR,BL,SL,SR"}
};

static int _volume;
static int _panning;
static int _is_playing;

static char *_buffer;

void configure(window_t parent)
{
	DEBUG_TRACE();
}

void about(window_t parent)
{
	DEBUG_TRACE();
	
	info(parent, "Copyright 2013 Mateus de Lima Oliveira\n\n"
		"Xiph.Org's libao output plugin.");
}
	
void init()
{
	DEBUG_TRACE();
	
	ao_initialize();
}

void quit()
{
	DEBUG_TRACE();
	
	ao_shutdown();
}
	
int open(int rate, int channels, int bits, int a, int b)
{	
	ao_sample_format sample_format = {
		.bits = bits,
		.channels = channels,
		.rate = rate,
		.byte_format = AO_FMT_NATIVE,
		.matrix = 0
	};
	
	DEBUG_TRACE();
	DEBUG_PRINTF("rate: %d\n", rate);
	DEBUG_PRINTF("channels: %d\n", channels);
	DEBUG_PRINTF("bits: %d\n", bits);
	
	_buffer = (char *) malloc(8192);
	device = ao_open_live(driver_id, &sample_format, 0);
	
	return 1;
}

void close()
{
	DEBUG_TRACE();
	
	/*ao_play(device, _buffer, 8192);*/
	/*ao_close(device);
	free(_buffer);*/
}
	
int write(char *buffer, int length)
{
	DEBUG_TRACE();
	DEBUG_PRINTF("length: %d\n", length);
	
	/*for (i=0; i<length; i++)
		buffer[i]=buffer[i]*_volume/256;*/
	_is_playing = 1;
	memcpy(_buffer, buffer, length);
	if(!ao_play(device, _buffer, length))
		return 1;
	if (!length)
		Sleep(2000);
	_is_playing = 0;
	
	return 0;
}

int can_write()
{
	/*DEBUG_TRACE();*/
	
	return 8192;
}

int is_playing()
{
	DEBUG_TRACE();
	
	return _is_playing;
}

int pause(int pause)
{
	DEBUG_TRACE();
	
	return 0;
}
	
void set_volume(int volume)
{
	DEBUG_TRACE();
	
	if (volume==-666)
		return;
	_volume = volume;
}

void set_panning(int panning)
{
	DEBUG_TRACE();
	
	_panning = panning;
}

void flush(int time)
{
	DEBUG_TRACE();
}
	
int get_output_time()
{
	DEBUG_TRACE();
	
	return 1000;
}

int get_written_time()
{
	DEBUG_TRACE();
	
	return 1001;
}

struct out_plugin_s out = {
	.version = 0x10,
	.description = "Xiph.Org's libao",
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

static void out_libao_init()
{
}

EXPORT(out_libao_init);

/* FIXME: remove. */
#ifdef _WIN32
struct out_plugin_s * __declspec(dllexport) winampGetOutModule()
{
	return &out;
}
#endif
