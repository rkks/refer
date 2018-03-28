
#ifndef OSZI_H
#define OSZI_H

extern void gui_print_sdlerror();
#define CHECK(_expr, _test) ({					\
	typeof(_expr) _rc = _expr;				\
	if (!(_rc _test)) {					\
		fprintf(stderr, "CHECK(%s, %s) failed: %d\n",	\
				#_expr, #_test, (int)_rc);	\
		gui_print_sdlerror();				\
		abort();					\
	}							\
	_rc; })

#define OSD_NONE	0
#define OSD_MODE	1
#define OSD_LD		2
#define OSD_INPUT	3
#define OSD_HELP	4

extern int osd;

#define DISPLAY_MODE_CH1	1
#define DISPLAY_MODE_CH2	2
#define DISPLAY_MODE_CH12	3
#define DISPLAY_MODE_XY		4

#define LD_MODE_DOTS	1
#define LD_MODE_LINES	2
#define LD_MODE_AUTO1	3
#define LD_MODE_AUTO2	4
#define LD_MODE_AUTO5	5
#define LD_MODE_SMART	6

#define MODULATION_NONE		0
#define MODULATION_AM1KHZ	1
#define MODULATION_AM5KHZ	2
#define MODULATION_AM10KHZ	3

#define PROCESS_NONE		0
#define PROCESS_FREQ		1
#define PROCESS_SPEC_T		2
#define PROCESS_SPECTRUM	3

extern int ch0_stream;
extern int ch0_invert;
extern int ch0_modulation;
extern int ch0_filterdc;
extern int ch0_process;

extern int ch1_stream;
extern int ch1_invert;
extern int ch1_modulation;
extern int ch1_filterdc;
extern int ch1_process;

extern int this_total_pathlen;
extern int this_total_pathnum;
extern int last_avg_pathlen;

extern int ld_mode;
extern int display_mode;
extern int avoid_glitch_hack;

extern int use_oss;
extern int use_alsa;
extern int use_portaudio;
extern int use_pipe;
extern char *sound_device;
extern int mono_mode;
extern int sound_out_mode;

extern void capture_init();
extern int capture(void *data, int num);
extern void capture_done();

extern int backframes;
extern char statusmsg[1024];

extern unsigned int tick;
extern unsigned int oszibuffer[640][420];

extern unsigned int hlines_n;
extern unsigned int hlines[10][2];

extern unsigned int vlines_n;
extern unsigned int vlines[10][2];

extern void gui_init();
extern void drawbuttons();
extern int osd_click(int x, int y);
extern void drawscreen();
extern void reset_oszibuffer();
extern void drawline(int x1, int y1, int x2, int y2, int force_line);
extern void gui_done();

extern void dft_wrapper(int n, const int *in, int *out);

#endif

