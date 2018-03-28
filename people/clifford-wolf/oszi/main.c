
#include "oszi.h"

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int ch0_stream = 0;
int ch0_invert = 0;
int ch0_filterdc = 0;
int ch0_modulation = MODULATION_NONE;
int ch0_process = PROCESS_NONE;

int ch1_stream = 1;
int ch1_invert = 0;
int ch1_filterdc = 0;
int ch1_modulation = MODULATION_NONE;
int ch1_process = PROCESS_NONE;

#define BUFFER_SIZE 2200 // = 44*5
signed short buffer[BUFFER_SIZE][2];
unsigned short buffer_am_demod[2][BUFFER_SIZE];

int dft_in_buffer_0[45000];
int dft_in_buffer_1[45000];

int dft_out_buffer_0[45000];
int dft_out_buffer_1[45000];

int dft_frq_buffer_0[640];
int dft_frq_buffer_1[640];
int dft_frq_specbuf_0[640][100];
int dft_frq_specbuf_1[640][100];

int dft_frq_buffer_0_counter;
int dft_frq_buffer_1_counter;

int dft_buffer_0_counter = 0;
int dft_buffer_1_counter = 0;

SDL_Surface *screen;
TTF_Font *font;

double ch0_dcpart = 0;
double ch1_dcpart = 0;

int ch0_lastypos = 0;
int ch1_lastypos = 0;
double current_xpos=0;
int avoid_glitch_hack=0;

int this_max_ch0 = 0;
int this_max_ch1 = 0;

int last_max_ch0 = 3000;
int last_max_ch1 = 3000;

double max_val = 3000;
double capt_size = 640;
double capt_remain = 0;
int pause_mode = 0;

int display_mode = DISPLAY_MODE_CH12;

static int gety(int val, int dim, int this_max_val)
{
	if (val > this_max_val)
		val = this_max_val;
	if (val < -this_max_val)
		val = -this_max_val;
	return dim - (val*dim)/this_max_val;
}

static void create_hline(int pos, unsigned int col)
{
	hlines[hlines_n][0] = pos;
	hlines[hlines_n][1] = col;
	hlines_n++;
}

static void create_vline(int pos, unsigned int col)
{
	vlines[vlines_n][0] = pos;
	vlines[vlines_n][1] = col;
	vlines_n++;
}

static int qsort_hvline_compar(const void *a_vp, const void *b_vp)
{
	const unsigned int *a = a_vp;
	const unsigned int *b = b_vp;
	if (*a < *b)
		return -1;
	if (*a > *b)
		return +1;
	return 0;
}

static void am_demod(int blocksize, int channel, int stream)
{
	static int last_am_sample[2];
	int temp[BUFFER_SIZE/blocksize+1];
	for (int i=0, j=0; i<BUFFER_SIZE/blocksize; i++) {
		int maxval = buffer[j][stream], minval = buffer[j][stream];
		for (int k=0; k<blocksize; k++, j++) {
			if (maxval < buffer[j][stream])
				maxval = buffer[j][stream];
			if (minval > buffer[j][stream])
				minval = buffer[j][stream];
		}
		temp[i+1] = maxval - minval;
	}
	temp[0] = last_am_sample[channel];
	last_am_sample[channel] = temp[BUFFER_SIZE/blocksize];
	for (int i=0; i<BUFFER_SIZE; i++) {
		buffer_am_demod[channel][i] = (temp[i/blocksize]*(blocksize-i%blocksize) +
				temp[i/blocksize+1]*(i%blocksize)) / blocksize;
	}
}

const char *progname;

void help()
{
	fprintf(stderr, "\n");
	fprintf(stderr, "Usage: %s [ -a | -o | -p | -i ] [ -d device ] [ -1 | -2 ]\n", progname);
	fprintf(stderr, "\n");
	fprintf(stderr, "    -o           Use OSS backend\n");
	fprintf(stderr, "    -a           Use ALSA backend (default)\n");
	fprintf(stderr, "    -p           Use PortAudio backend\n");
	fprintf(stderr, "    -i           Use pipe backend (read from stdin)\n");
	fprintf(stderr, "    -d device    Use the given (ALSA or OSS) dsp device\n");
	fprintf(stderr, "    -1           Mono mode (both channels show the same mono signal)\n");
	fprintf(stderr, "    -2           Dual-Channel-Mode (default)\n");
	fprintf(stderr, "    -O           Active sound output\n");
	fprintf(stderr, "\n");
	exit(1);
}

int main(int argc, char **argv)
{
	int opt;
	progname = argv[0];

	while ((opt = getopt(argc, argv, "aopid:12O")) != -1)
		switch (opt)
		{
		case 'a':
			use_alsa = 1;
			use_oss = 0;
			use_portaudio = 0;
			use_pipe = 0;
			break;
		case 'o':
			use_alsa = 0;
			use_oss = 1;
			use_portaudio = 0;
			use_pipe = 0;
			break;
		case 'p':
			use_alsa = 0;
			use_oss = 0;
			use_portaudio = 1;
			use_pipe = 0;
			break;
		case 'i':
			use_alsa = 0;
			use_oss = 0;
			use_portaudio = 0;
			use_pipe = 1;
			break;
		case 'd':
			sound_device = optarg;
			break;
		case '1':
			mono_mode = 1;
			break;
		case '2':
			mono_mode = 0;
			break;
		case 'O':
			sound_out_mode = 1;
			break;
		default:
			help();
		}

	capture_init();
	atexit(capture_done);

	gui_init();
	atexit(gui_done);

	drawbuttons();

	while (1) {
		SDL_Event event;

		while (SDL_PollEvent(&event))
			if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
				osd_click(event.button.x, event.button.y);
			else
			if (event.type == SDL_MOUSEMOTION && osd == OSD_NONE &&
					event.motion.y > 30 && event.motion.y < 470 &&
					(event.motion.state&SDL_BUTTON(1))) {
				if (SDL_GetModState() & (KMOD_LSHIFT|KMOD_RSHIFT))
					if (SDL_GetModState() & (KMOD_ALT))
						capt_size += 0.00001*event.motion.xrel;
					else
						capt_size += 0.001*event.motion.xrel;
				else
					capt_size += 0.1*event.motion.xrel;
				max_val += max_val*event.motion.yrel/1000.0;
			} else
			if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym)
				{
				case SDLK_q:
					exit(0);
					break;
				case SDLK_LEFT:
					if (event.key.keysym.mod & (KMOD_LSHIFT|KMOD_RSHIFT))
						capt_size -= 0.01;
					else
						capt_size -= 1;
					break;
				case SDLK_UP:
					capt_size -= 10;
					break;
				case SDLK_PAGEUP:
					capt_size /= 2;
					break;
				case SDLK_RIGHT:
					if (event.key.keysym.mod & (KMOD_LSHIFT|KMOD_RSHIFT))
						capt_size += 0.01;
					else
						capt_size += 1;
					break;
				case SDLK_DOWN:
					capt_size += 10;
					break;
				case SDLK_PAGEDOWN:
					capt_size *= 2;
					break;
				case SDLK_1:
					capt_size = 640;
					break;
				case SDLK_2:
					capt_size /= 2;
					break;
				case SDLK_3:
					capt_size /= 3;
					break;
				case SDLK_4:
					capt_size /= 4;
					break;
				case SDLK_5:
					capt_size /= 5;
					break;
				case SDLK_6:
					capt_size /= 6;
					break;
				case SDLK_7:
					capt_size /= 7;
					break;
				case SDLK_PLUS:
				case SDLK_KP_PLUS:
					if (backframes < 32)
						backframes++;
					break;
				case SDLK_MINUS:
				case SDLK_KP_MINUS:
					if (backframes > 0)
						backframes--;
					break;
				case SDLK_SPACE:
					pause_mode = !pause_mode;
					break;
				case SDLK_a:
					max_val = last_max_ch0*1.2;
					break;
				case SDLK_s:
					if (last_max_ch0 > last_max_ch1)
						max_val = last_max_ch0*1.2;
					else
						max_val = last_max_ch1*1.2;
					break;
				case SDLK_d:
					max_val = last_max_ch1*1.2;
					break;
				default:
					/* nothing */
					break;
				}
			} else
			if (event.type == SDL_QUIT) {
				exit(0);
			}

		if (capt_size <= 10)
			capt_size = 10;
		if (capt_size >= 44000)
			capt_size = 44000;
		if (max_val <= 100)
			max_val = 100;
		if (max_val >= 500000)
			max_val = 500000;

		int ch0_max_val = max_val;
		int ch1_max_val = max_val;

		capture(buffer, BUFFER_SIZE);

		int ch0_dim = display_mode == DISPLAY_MODE_CH12 ? 100 : 200;
		int ch1_dim = display_mode == DISPLAY_MODE_CH12 ? 100 : 200;

		switch (ch0_modulation)
		{
		case MODULATION_AM1KHZ:
			am_demod(44, 0, ch0_stream);
			break;
		case MODULATION_AM5KHZ:
			am_demod(8, 0, ch0_stream);
			break;
		case MODULATION_AM10KHZ:
			am_demod(4, 0, ch0_stream);
			break;
		}

		switch (ch1_modulation)
		{
		case MODULATION_AM1KHZ:
			am_demod(44, 1, ch1_stream);
			break;
		case MODULATION_AM5KHZ:
			am_demod(8, 1, ch1_stream);
			break;
		case MODULATION_AM10KHZ:
			am_demod(4, 1, ch1_stream);
			break;
		}

		if (tick == 0)
			reset_oszibuffer();

		snprintf(statusmsg, 1024, "T: %.2f ms / %.2f Hz, A: %.2f%% / %.2f%%, D: %d %s",
				capt_size*1000/44100.0, 44100.0/capt_size,
				100*max_val/INT16_MAX, 100*max_val/INT16_MAX,
				backframes, pause_mode ? "<PAUSE>" : "");

		hlines_n = 0;
		vlines_n = 0;

		int ch0_positive = ch0_modulation || ch0_process == PROCESS_SPECTRUM || ch0_process == PROCESS_FREQ || ch0_process == PROCESS_SPEC_T;
		int ch1_positive = ch1_modulation || ch1_process == PROCESS_SPECTRUM || ch1_process == PROCESS_FREQ || ch1_process == PROCESS_SPEC_T;

		switch (display_mode)
		{
		case DISPLAY_MODE_CH1:
			create_hline(gety(INT16_MAX, ch0_dim, ch0_max_val) + 4, ch0_max_val < INT16_MAX ? 0x00400000 : 0x00ff0000);
			create_hline(gety(0, ch0_dim, ch0_max_val) + 5 + (ch0_positive ? ch0_dim : 0), 0x00808080);
			if (!ch0_positive)
				create_hline(gety(INT16_MIN, ch0_dim, ch0_max_val) + 6, ch0_max_val < INT16_MAX ? 0x00400000 : 0x00ff0000);
			break;
		case DISPLAY_MODE_CH2:
			create_hline(gety(INT16_MAX, ch1_dim, ch1_max_val) + 4, ch1_max_val < INT16_MAX ? 0x00400000 : 0x00ff0000);
			create_hline(gety(0, ch1_dim, ch1_max_val) + 5 + (ch1_positive ? ch1_dim : 0), 0x00808080);
			if (!ch1_positive)
				create_hline(gety(INT16_MIN, ch1_dim, ch1_max_val) + 6, ch1_max_val < INT16_MAX ? 0x00400000 : 0x00ff0000);
			break;
		case DISPLAY_MODE_CH12:
			create_hline(gety(INT16_MAX, ch0_dim, ch0_max_val) + 4, ch0_max_val < INT16_MAX ? 0x00400000 : 0x00ff0000);
			create_hline(gety(0, ch0_dim, ch0_max_val) + 5 + (ch0_positive ? ch0_dim : 0), 0x00808080);
			if (!ch0_positive)
				create_hline(gety(INT16_MIN, ch0_dim, ch0_max_val) + 6, ch0_max_val < INT16_MAX ? 0x00400000 : 0x00ff0000);

			create_hline(gety(INT16_MAX, ch1_dim, ch1_max_val) + 214, ch1_max_val < INT16_MAX ? 0x00400000 : 0x00ff0000);
			create_hline(gety(0, ch1_dim, ch1_max_val) + 215 + (ch1_positive ? ch1_dim : 0), 0x00808080);
			if (!ch1_positive)
				create_hline(gety(INT16_MIN, ch1_dim, ch1_max_val) + 216, ch1_max_val < INT16_MAX ? 0x00400000 : 0x00ff0000);
			break;
		case DISPLAY_MODE_XY:
			create_vline(gety(INT16_MAX, ch0_dim, ch0_max_val) + 120, ch0_max_val < INT16_MAX ? 0x00400000 : 0x00ff0000);
			create_vline(gety(INT16_MIN, ch0_dim, ch0_max_val) + 120, ch0_max_val < INT16_MAX ? 0x00400000 : 0x00ff0000);
			create_hline(gety(INT16_MAX, ch1_dim, ch1_max_val) + 4, ch1_max_val < INT16_MAX ? 0x00400000 : 0x00ff0000);
			create_hline(gety(INT16_MIN, ch1_dim, ch1_max_val) + 6, ch1_max_val < INT16_MAX ? 0x00400000 : 0x00ff0000);
			break;
		}

		qsort(hlines, hlines_n, sizeof(int)*2, qsort_hvline_compar);
		qsort(vlines, vlines_n, sizeof(int)*2, qsort_hvline_compar);

		int updscreen = 1;
		for (int i=0; i<BUFFER_SIZE; i++)
		{
			int new_ch0_rawvalue = ch0_modulation ? buffer_am_demod[0][i] :
					ch0_invert ? -buffer[i][ch0_stream] : buffer[i][ch0_stream];
			int new_ch1_rawvalue = ch1_modulation ? buffer_am_demod[1][i] :
					ch1_invert ? -buffer[i][ch1_stream] : buffer[i][ch1_stream];

			ch0_dcpart = (ch0_dcpart*(BUFFER_SIZE-1) + new_ch0_rawvalue) / BUFFER_SIZE;
			ch1_dcpart = (ch1_dcpart*(BUFFER_SIZE-1) + new_ch1_rawvalue) / BUFFER_SIZE;

			if (ch0_filterdc)
				new_ch0_rawvalue -= ch0_dcpart;
			if (ch1_filterdc)
				new_ch1_rawvalue -= ch1_dcpart;

			if (abs(new_ch0_rawvalue) > this_max_ch0)
				this_max_ch0 = abs(new_ch0_rawvalue);
			if (abs(new_ch1_rawvalue) > this_max_ch1)
				this_max_ch1 = abs(new_ch1_rawvalue);

			if (ch0_modulation)
				new_ch0_rawvalue -= ch0_max_val < INT16_MAX ? ch0_max_val : INT16_MAX;

			if (ch1_modulation)
				new_ch1_rawvalue -= ch1_max_val < INT16_MAX ? ch1_max_val : INT16_MAX;

			double new_xpos = current_xpos + 640/capt_size;

			if (ch0_process == PROCESS_SPECTRUM || ch0_process == PROCESS_FREQ || ch0_process == PROCESS_SPEC_T)
			{
				dft_in_buffer_0[dft_buffer_0_counter] = new_ch0_rawvalue;
				dft_buffer_0_counter = (dft_buffer_0_counter+1) % (int)capt_size;
				if (!dft_buffer_0_counter) {
					dft_wrapper(capt_size, dft_in_buffer_0, dft_out_buffer_0);
					dft_frq_buffer_0_counter = (dft_frq_buffer_0_counter+1) % 640;
					if (ch0_process == PROCESS_FREQ) {
						int val = 0;
						for (int j=5; j<capt_size; j++) {
							if (dft_out_buffer_0[j] > dft_out_buffer_0[val])
								val = j;
						}
						val = (double)(ch0_max_val*1.8*val) / capt_size;
						val -= ch0_max_val < INT16_MAX ? ch0_max_val : INT16_MAX;
						dft_frq_buffer_0[dft_frq_buffer_0_counter] = val;
					}
					if (ch0_process == PROCESS_SPEC_T) {
						int maxval = 0;
						for (int j=0; j<100; j++) {
							int pos = ((int)(j*capt_size / 500))*5 + 3;
							if (dft_out_buffer_0[pos] > maxval)
								maxval = dft_out_buffer_0[pos];
							dft_frq_specbuf_0[dft_frq_buffer_0_counter][j] = dft_out_buffer_0[pos];
						}
						for (int j=0; j<100; j++) {
							int v = dft_frq_specbuf_0[dft_frq_buffer_0_counter][j];
							v = (backframes*1.5) - log(v) * (backframes*1.5) / log(maxval);
							dft_frq_specbuf_0[dft_frq_buffer_0_counter][j] = v;
						}
					}
				}
				int x = new_xpos < 640 ? new_xpos : 639;
				if (ch0_process == PROCESS_SPECTRUM) {
					int out_idx = (int)(new_xpos*capt_size/640) % (int)capt_size;
					new_ch0_rawvalue = dft_out_buffer_0[out_idx];
					new_ch0_rawvalue -= ch0_max_val < INT16_MAX ? ch0_max_val : INT16_MAX;
				}
				if (ch0_process == PROCESS_FREQ) {
					new_ch0_rawvalue = dft_frq_buffer_0[x];
				}
				if (ch0_process == PROCESS_SPEC_T && pause_mode < 2) {
					if (display_mode == DISPLAY_MODE_CH1) {
						int yoff = 400;
						for (int y = 0; y < 300; y++) {
							oszibuffer[x][yoff-y] = tick - dft_frq_specbuf_0[x][y/3];
							oszibuffer[x][yoff-y] = tick - dft_frq_specbuf_0[x][y/3];
						}
					}
					if (display_mode == DISPLAY_MODE_CH12) {
						int yoff = 200;
						for (int y = 0; y < 170; y++) {
							oszibuffer[x][yoff-y] = tick - dft_frq_specbuf_0[x][y*10/17];
						}
					}
				}
			}

			if (ch1_process == PROCESS_SPECTRUM || ch1_process == PROCESS_FREQ || ch1_process == PROCESS_SPEC_T)
			{
				dft_in_buffer_1[dft_buffer_1_counter] = new_ch1_rawvalue;
				if (ch0_process == PROCESS_SPECTRUM || ch0_process == PROCESS_FREQ || ch0_process == PROCESS_SPEC_T)
					dft_buffer_1_counter = dft_buffer_0_counter;
				else
					dft_buffer_1_counter = (dft_buffer_1_counter+1) % (int)capt_size;
				if (!dft_buffer_1_counter) {
					dft_wrapper(capt_size, dft_in_buffer_1, dft_out_buffer_1);
					if (ch0_process == PROCESS_SPECTRUM || ch0_process == PROCESS_FREQ || ch0_process == PROCESS_SPEC_T)
						dft_frq_buffer_1_counter = dft_frq_buffer_0_counter;
					else
						dft_frq_buffer_1_counter = (dft_frq_buffer_1_counter+1) % 640;
					if (ch1_process == PROCESS_FREQ) {
						int val = 0;
						for (int j=5; j<capt_size; j++) {
							if (dft_out_buffer_1[j] > dft_out_buffer_1[val])
								val = j;
						}
						val = (double)(ch1_max_val*1.8*val) / capt_size;
						val -= ch1_max_val < INT16_MAX ? ch1_max_val : INT16_MAX;
						dft_frq_buffer_1[dft_frq_buffer_1_counter] = val;
					}
					if (ch1_process == PROCESS_SPEC_T) {
						int maxval = 0;
						for (int j=0; j<100; j++) {
							int pos = ((int)(j*capt_size / 500))*5 + 3;
							if (dft_out_buffer_1[pos] > maxval)
								maxval = dft_out_buffer_1[pos];
							dft_frq_specbuf_1[dft_frq_buffer_1_counter][j] = dft_out_buffer_1[pos];
						}
						for (int j=0; j<100; j++) {
							int v = dft_frq_specbuf_1[dft_frq_buffer_1_counter][j];
							v = (backframes*1.5) - log(v) * (backframes*1.5) / log(maxval);
							dft_frq_specbuf_1[dft_frq_buffer_1_counter][j] = v;
						}
					}
				}
				int x = new_xpos < 640 ? new_xpos : 639;
				if (ch1_process == PROCESS_SPECTRUM) {
					int out_idx = (int)(new_xpos*capt_size/640) % (int)capt_size;
					new_ch1_rawvalue = dft_out_buffer_1[out_idx];
					new_ch1_rawvalue -= ch1_max_val < INT16_MAX ? ch1_max_val : INT16_MAX;
				}
				if (ch1_process == PROCESS_FREQ) {
					new_ch1_rawvalue = dft_frq_buffer_1[x];
				}
				if (ch1_process == PROCESS_SPEC_T && pause_mode < 2) {
					if (display_mode == DISPLAY_MODE_CH2) {
						int yoff = 400;
						for (int y = 0; y < 300; y++) {
							oszibuffer[x][yoff-y] = tick - dft_frq_specbuf_1[x][y/3];
							oszibuffer[x][yoff-y] = tick - dft_frq_specbuf_1[x][y/3];
						}
					}
					if (display_mode == DISPLAY_MODE_CH12) {
						int yoff = 400;
						for (int y = 0; y < 170; y++) {
							oszibuffer[x][yoff-y] = tick - dft_frq_specbuf_1[x][y*10/17];
						}
					}
				}
			}

			int new_ch0_ypos = gety(new_ch0_rawvalue, ch0_dim, ch0_max_val);
			int new_ch1_ypos = gety(new_ch1_rawvalue, ch1_dim, ch1_max_val);

			if (avoid_glitch_hack) {
				ch0_lastypos = new_ch0_ypos;
				ch1_lastypos = new_ch1_ypos;
				avoid_glitch_hack = 0;
			}

			if (pause_mode < 2)
			{
				switch (display_mode)
				{
				case DISPLAY_MODE_CH1:
					if (ch0_process != PROCESS_SPEC_T)
						drawline(current_xpos, ch0_lastypos + 5, new_xpos, new_ch0_ypos + 5, ch0_process == PROCESS_SPECTRUM);
					break;
				case DISPLAY_MODE_CH2:
					if (ch1_process != PROCESS_SPEC_T)
						drawline(current_xpos, ch1_lastypos + 5, new_xpos, new_ch1_ypos + 5, ch1_process == PROCESS_SPECTRUM);
					break;
				case DISPLAY_MODE_CH12:
					if (ch0_process != PROCESS_SPEC_T)
						drawline(current_xpos, ch0_lastypos + 5, new_xpos, new_ch0_ypos + 5, ch0_process == PROCESS_SPECTRUM);
					if (ch1_process != PROCESS_SPEC_T)
						drawline(current_xpos, ch1_lastypos + 215, new_xpos, new_ch1_ypos + 215, ch1_process == PROCESS_SPECTRUM);
					break;
				case DISPLAY_MODE_XY:
					drawline(ch0_lastypos + 120, ch1_lastypos + 5, new_ch0_ypos + 120, new_ch1_ypos + 5, 0);
					break;
				}

				if (new_xpos > 640)
				{
					if (updscreen) {
						drawscreen();
						updscreen = 0;
					}

					if (pause_mode)
						pause_mode = 2;
					else
						tick++;
				}
			}

			while (new_xpos > 640)
				new_xpos -= 640;

			ch0_lastypos = new_ch0_ypos;
			ch1_lastypos = new_ch1_ypos;
			current_xpos = new_xpos;
		}
		if (updscreen)
			drawscreen();

		if (this_total_pathnum)
			last_avg_pathlen = sqrt(this_total_pathlen / this_total_pathnum);
		this_total_pathlen = this_total_pathnum = 0;

		last_max_ch0 = ch0_modulation ? this_max_ch0/2 : this_max_ch0;
		last_max_ch1 = ch1_modulation ? this_max_ch1/2 : this_max_ch1;
		this_max_ch0 = this_max_ch1 = 0;
	}
}

