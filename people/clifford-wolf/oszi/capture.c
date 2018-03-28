
#include "oszi.h"

#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>

#ifdef HAVE_OSS
#  include <sys/soundcard.h>
#  include <sys/ioctl.h>
#endif

#ifdef HAVE_ALSA
#  include <alsa/asoundlib.h>
#endif

#ifdef HAVE_PORTAUDIO
#  include <portaudio.h>
#endif

int use_oss = 0;
int use_alsa = 1;
int use_portaudio = 0;
int use_pipe = 0;
char *sound_device = NULL;
int mono_mode = 0;
int sound_out_mode = 0;

#ifdef HAVE_OSS
static int fd;
#endif

#ifdef HAVE_ALSA
static snd_pcm_t *capture_handle;
static snd_pcm_t *playback_handle;
static snd_pcm_hw_params_t *hw_params;
#endif

#ifdef HAVE_PORTAUDIO
static PaStream *stream;
#endif

#ifdef WITH_SOUNDOUT

#include <pthread.h>
pthread_t write_thread;

void *write_thread_main(void *arg)
{
	signed short buffer[1024*2];
	double t = 0;

	while (1)
	{
		write(1, ".", 1);
		for (int i = 0; i < 1024; i++) {
			buffer[i*2+0] = 10000 * sin(t*1000);
			buffer[i*2+1] = 10000 * cos(t*1000);
			t += 1.0 / 44100.0;
		}

		if (mono_mode) {
			for (int i = 0; i < 1024; i++)
				buffer[i] = buffer[i*2];
		}

		if (use_oss) {
			int i = mono_mode ? 1024*2 : 1024*4;
			void *vp = buffer;
			while (i > 0) {
				int rc = write(fd, vp, i);
				if (rc <= 0)
					return NULL;
				vp += rc;
				i -= rc;
			}
		}

		if (use_alsa) {
			int i = 1024;
			void *vp = buffer;
			while (i > 0) {
				int rc = snd_pcm_writei(playback_handle, vp, i);
				if (rc <= 0)
					return NULL;
				vp += rc * (mono_mode ? 2 : 4);
				i -= rc;
			}
		}

		if (use_portaudio) {
			if (Pa_WriteStream(stream, buffer, 1024) == paNoError)
				return NULL;
		}
	}
}
#endif /* WITH_SOUNDOUT */

void capture_init()
{
#ifndef WITH_SOUNDOUT
	if (sound_out_mode) {
		fprintf(stderr, "Sound output support is not built in!\n");
		exit(1);
	}
#endif

	if (use_oss)
	{
#ifdef HAVE_OSS
#  ifdef WITH_SOUNDOUT
		fd = CHECK(open(sound_device ?: "/dev/dsp", sound_out_mode ? O_RDWR : O_RDONLY), != -1);
#  else
		fd = CHECK(open(sound_device ?: "/dev/dsp", O_RDONLY), != -1);
#  endif

		int audio_format = AFMT_S16_NE;
		int audio_channels = mono_mode ? 1 : 2;
		int sample_rate = 44100;
		CHECK(ioctl(fd, SNDCTL_DSP_SETFMT, &audio_format), != -1);
		CHECK(ioctl(fd, SNDCTL_DSP_CHANNELS, &audio_channels), != -1);
		CHECK(ioctl(fd, SNDCTL_DSP_SPEED, &sample_rate), != -1);
#  ifdef WITH_SOUNDOUT
		if (sound_out_mode)
			pthread_create(&write_thread, NULL, &write_thread_main, NULL);
#  endif
#else
		fprintf(stderr, "OSS support is not built in!\n");
		exit(1);
#endif
	}

	if (use_alsa)
	{
#ifdef HAVE_ALSA
		unsigned int sample_rate = 44100;
		CHECK(snd_pcm_open(&capture_handle, sound_device ?: "default", SND_PCM_STREAM_CAPTURE, 0), != -1);
		CHECK(snd_pcm_hw_params_malloc(&hw_params), != -1);
		CHECK(snd_pcm_hw_params_any(capture_handle, hw_params), != -1);
		CHECK(snd_pcm_hw_params_set_access(capture_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED), != -1);
		CHECK(snd_pcm_hw_params_set_format(capture_handle, hw_params, SND_PCM_FORMAT_S16), != -1);
		CHECK(snd_pcm_hw_params_set_rate_near(capture_handle, hw_params, &sample_rate, 0), != -1);
		CHECK(snd_pcm_hw_params_set_channels(capture_handle, hw_params, mono_mode ? 1 : 2), != -1);
		CHECK(snd_pcm_hw_params(capture_handle, hw_params), != -1);
		snd_pcm_hw_params_free(hw_params);
		snd_pcm_prepare(capture_handle);
#  ifdef WITH_SOUNDOUT
		if (sound_out_mode) {
			CHECK(snd_pcm_open(&playback_handle, sound_device ?: "default", SND_PCM_STREAM_PLAYBACK, 0), != -1);
			CHECK(snd_pcm_hw_params_malloc(&hw_params), != -1);
			CHECK(snd_pcm_hw_params_any(playback_handle, hw_params), != -1);
			CHECK(snd_pcm_hw_params_set_access(playback_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED), != -1);
			CHECK(snd_pcm_hw_params_set_format(playback_handle, hw_params, SND_PCM_FORMAT_S16), != -1);
			CHECK(snd_pcm_hw_params_set_rate_near(playback_handle, hw_params, &sample_rate, 0), != -1);
			CHECK(snd_pcm_hw_params_set_channels(playback_handle, hw_params, mono_mode ? 1 : 2), != -1);
			CHECK(snd_pcm_hw_params(playback_handle, hw_params), != -1);
			snd_pcm_hw_params_free(hw_params);
			snd_pcm_prepare(playback_handle);
			pthread_create(&write_thread, NULL, &write_thread_main, NULL);
		}
#  endif
#else
		fprintf(stderr, "ALSA support is not built in!\n");
		exit(1);
#endif
	}

	if (use_portaudio)
	{
#ifdef HAVE_PORTAUDIO
		CHECK(Pa_Initialize(), == paNoError);
		CHECK(Pa_OpenDefaultStream(&stream,
                		mono_mode ? 1 : 2, /* input channels */
#  ifdef WITH_SOUNDOUT
                		sound_out_mode ? (mono_mode ? 1 : 2) : 0, /* output channels */
#  else
                		0,                 /* no output channels */
#  endif
                		paInt16,           /* 16 bit integer output */
                		44100,             /* sampling rate */
                		0,                 /* frames per callback buffer */
                		NULL,              /* this is your callback function */
                		NULL),             /* user data pointer */
						== paNoError);
		CHECK(Pa_StartStream(stream), == paNoError);
#  ifdef WITH_SOUNDOUT
		if (sound_out_mode)
			pthread_create(&write_thread, NULL, &write_thread_main, NULL);
#  endif
#else
		fprintf(stderr, "PortAudio support is not built in!\n");
		exit(1);
#endif
	}

	if (use_pipe)
	{
		if (!sound_device || (strcmp(sound_device, "ascii") && strcmp(sound_device, "binary"))) {
			fprintf(stderr, "In pipe mode audio device must be set to 'ascii' or 'binary'!\n");
			exit(1);
		}
	}
}


int capture(void *data, int num)
{
	signed short *sd = data;
	memset(data, 0, num*4);

#ifdef HAVE_OSS
	if (use_oss)
	{
		int i = num*(mono_mode ? 2 : 4);
		while (i > 0) {
			int rc = CHECK(read(fd, data, i), > 0);
			data += rc;
			i -= rc;
		}
	}
#endif

#ifdef HAVE_ALSA
	if (use_alsa)
	{
		int i = num;
		while (i > 0) {
			int rc = snd_pcm_readi(capture_handle, data, i);
			if (rc <= 0) {
				fprintf(stderr, "ALSA <%d> I/O Error: %s (%d)\n",
						num, snd_strerror(rc), rc);
				capture_done();
				capture_init();
				continue;
			}
			data += rc*(mono_mode ? 2 : 4);
			i -= rc;
		}
	}
#endif

#ifdef HAVE_PORTAUDIO
	if (use_portaudio)
	{
		CHECK(Pa_ReadStream(stream, data, num), == paNoError);
	}
#endif

	if (use_pipe)
	{
		double usec = (1000000.0 / 44100.0) * num;
		struct timeval tv1, tv2;
		gettimeofday(&tv1, NULL);

		if (!strcmp(sound_device, "ascii"))
			for (int i=0; i < num; i++)
				fscanf(stdin, "%hd %hd\n", &sd[i*2], &sd[i*2+1]);

		if (!strcmp(sound_device, "binary"))
			fread(sd, num*4, 1, stdin);

		gettimeofday(&tv2, NULL);
		if (tv1.tv_sec != tv2.tv_sec)
			tv2.tv_usec += 1000000;
		usec -= tv2.tv_usec - tv1.tv_usec;
		if (usec < 0)
			fprintf(stderr, "Slow pipe i/o (%d usecs late)\n", (int)-usec);
		else
			usleep(usec);
	}

	if (mono_mode) {
		for (int i=num-1; i >= 0; i--)
			sd[i*2+1] = sd[i*2] = sd[i];
	}

	return num;
}

void capture_done()
{
#ifdef HAVE_OSS
	if (use_oss)
		close(fd);
#endif

#ifdef HAVE_ALSA
	if (use_alsa)
		snd_pcm_close(capture_handle);
#endif

#ifdef HAVE_PORTAUDIO
	if (use_portaudio) {
		CHECK(Pa_StopStream(stream), == paNoError);
		CHECK(Pa_CloseStream(stream), == paNoError);
		CHECK(Pa_Terminate(), == paNoError);
	}
#endif
}

