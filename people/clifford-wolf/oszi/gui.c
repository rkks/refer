
#include "oszi.h"

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <assert.h>
#include <unistd.h>
#include <stdio.h>

SDL_Surface *screen;
TTF_Font *font;

void gui_init()
{
	CHECK(SDL_Init(SDL_INIT_VIDEO), >= 0);
	SDL_WM_SetCaption("Oszi", "Oszi");
	atexit(SDL_Quit);

	CHECK(TTF_Init(), >= 0);

	screen = CHECK(SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE), != NULL);
	font = CHECK(TTF_OpenFont("font.ttf", 16), != NULL);
}

extern void gui_print_sdlerror()
{
	fprintf(stderr, "SDL Error: %s\n", SDL_GetError());
}

static void drawtext(int x, int w, int y, const char* text)
{
	SDL_Color color = {255, 255, 255};
	SDL_Surface *text_surface;
	text_surface = CHECK(TTF_RenderText_Solid(font, text, color), != NULL);
	SDL_Rect drect = text_surface->clip_rect;
	drect.x = x; drect.y = y;
	if (w && w > drect.w)
		drect.x += (w-drect.w)/2;
	SDL_BlitSurface(text_surface, NULL, screen, &drect);
	SDL_FreeSurface(text_surface);
}

int button_x;
static void drawbutton(int w, Uint32 bgcolor, const char* text)
{
	Uint32 *pixel = screen->pixels + button_x*4;

	int i, j;
	for (i=0; i<22; i++) {
		*(pixel++) = 0x00ffffff;
		for (j=1; j<w; j++)
			*(pixel++) = bgcolor;
		*pixel = 0x00ffffff;
		pixel += 640 - w;
	}

	for (i=0; i<=w; i++) {
		*pixel = 0x00ffffff;
		pixel++;
	}

	drawtext(button_x, w, 0, text);
	button_x += w;
}

int button_mode_x1;
int button_mode_x2;
int button_ld_x1;
int button_ld_x2;
int button_input_x1;
int button_input_x2;
int button_help_x1;
int button_help_x2;
int osd = 0;

#define NUM_HELP_SCREEN 3
int help_screen = 0;

void drawbuttons()
{
	Uint32 *p = (void*)screen->pixels;
	for (int i=0; i<640*22; i++)
		*(p++) = 0;

	button_x = 10;

	button_mode_x1 = button_x;
	switch (display_mode)
	{
	case DISPLAY_MODE_CH1:
		drawbutton(100, osd == OSD_MODE ? 0x00338833 : 0x00000000, "CH1 ONLY");
		break;
	case DISPLAY_MODE_CH2:
		drawbutton(100, osd == OSD_MODE ? 0x00338833 : 0x00000000, "CH2 ONLY");
		break;
	case DISPLAY_MODE_CH12:
		drawbutton(100, osd == OSD_MODE ? 0x00338833 : 0x00000000, "CH1 + CH2");
		break;
	case DISPLAY_MODE_XY:
		drawbutton(100, osd == OSD_MODE ? 0x00338833 : 0x00000000, "X/Y");
		break;
	}
	button_mode_x2 = button_x;

	button_x += 10;

	button_ld_x1 = button_x;
	switch (ld_mode)
	{
	case LD_MODE_LINES:
		drawbutton(60, osd == OSD_LD ? 0x00338833 : 0x00000000, "LINES");
		break;
	case LD_MODE_DOTS:
		drawbutton(60, osd == OSD_LD ? 0x00338833 : 0x00000000, "DOTS");
		break;
	case LD_MODE_AUTO1:
		drawbutton(60, osd == OSD_LD ? 0x00338833 : 0x00000000, "LDA1");
		break;
	case LD_MODE_AUTO2:
		drawbutton(60, osd == OSD_LD ? 0x00338833 : 0x00000000, "LDA2");
		break;
	case LD_MODE_AUTO5:
		drawbutton(60, osd == OSD_LD ? 0x00338833 : 0x00000000, "LDA5");
		break;
	case LD_MODE_SMART:
		drawbutton(60, osd == OSD_LD ? 0x00338833 : 0x00000000, "SMART");
		break;
	}
	button_ld_x2 = button_x;

	button_x += 10;

	button_input_x1 = button_x;
	drawbutton(60, osd == OSD_INPUT ? 0x00338833 : 0x00000000, "INPUT");
	button_input_x2 = button_x;

	button_x += 330;

	button_help_x1 = button_x;
	drawbutton(50, osd == OSD_HELP ? 0x00338833 : 0x00000000, "HELP");
	button_help_x2 = button_x;
}

#define AREA(_x, _y, _w, _h) (x >= _x && x < _x+_w && y >= _y && y < _y+_h)

static void osd_click_mode(int x, int y)
{
	if AREA(100, 100, 440, 25) {
		if (display_mode == DISPLAY_MODE_CH1)
			osd = OSD_NONE;
		display_mode = DISPLAY_MODE_CH1;
		avoid_glitch_hack = 1;
	}
	if AREA(100, 150, 440, 25) {
		if (display_mode == DISPLAY_MODE_CH2)
			osd = OSD_NONE;
		display_mode = DISPLAY_MODE_CH2;
		avoid_glitch_hack = 1;
	}
	if AREA(100, 200, 440, 25) {
		if (display_mode == DISPLAY_MODE_CH12)
			osd = OSD_NONE;
		display_mode = DISPLAY_MODE_CH12;
		avoid_glitch_hack = 1;
	}
	if AREA(100, 250, 440, 25) {
		if (display_mode == DISPLAY_MODE_XY)
			osd = OSD_NONE;
		display_mode = DISPLAY_MODE_XY;
		avoid_glitch_hack = 1;
	}
}

static void osd_click_ld(int x, int y)
{
	if AREA(100, 80, 440, 25) {
		if (ld_mode == LD_MODE_LINES)
			osd = OSD_NONE;
		ld_mode = LD_MODE_LINES;
	}
	if AREA(100, 130, 440, 25) {
		if (ld_mode == LD_MODE_DOTS)
			osd = OSD_NONE;
		ld_mode = LD_MODE_DOTS;
	}
	if AREA(100, 180, 440, 25) {
		if (ld_mode == LD_MODE_AUTO1)
			osd = OSD_NONE;
		ld_mode = LD_MODE_AUTO1;
	}
	if AREA(100, 230, 440, 25) {
		if (ld_mode == LD_MODE_AUTO2)
			osd = OSD_NONE;
		ld_mode = LD_MODE_AUTO2;
	}
	if AREA(100, 280, 440, 25) {
		if (ld_mode == LD_MODE_AUTO5)
			osd = OSD_NONE;
		ld_mode = LD_MODE_AUTO5;
	}
	if AREA(100, 330, 440, 25) {
		if (ld_mode == LD_MODE_SMART)
			osd = OSD_NONE;
		ld_mode = LD_MODE_SMART;
	}
}

static void osd_click_input(int x, int y)
{
	if AREA(280, 80, 60, 25) {
		if (ch0_stream == 0)
			osd = OSD_NONE;
		ch0_stream = 0;
	}
	if AREA(350, 80, 60, 25) {
		if (ch0_stream == 1)
			osd = OSD_NONE;
		ch0_stream = 1;
	}
	if AREA(420, 80, 70, 25) {
		ch0_invert = !ch0_invert;
	}
	if AREA(500, 80, 60, 25) {
		ch0_filterdc = !ch0_filterdc;
	}
	if AREA(80, 110, 150, 25) {
		if (ch0_modulation == MODULATION_NONE)
			osd = OSD_NONE;
		ch0_modulation = MODULATION_NONE;
	}
	if AREA(240, 110, 100, 25) {
		if (ch0_modulation == MODULATION_AM1KHZ)
			osd = OSD_NONE;
		ch0_modulation = MODULATION_AM1KHZ;
	}
	if AREA(350, 110, 100, 25) {
		if (ch0_modulation == MODULATION_AM5KHZ)
			osd = OSD_NONE;
		ch0_modulation = MODULATION_AM5KHZ;
	}
	if AREA(460, 110, 100, 25) {
		if (ch0_modulation == MODULATION_AM10KHZ)
			osd = OSD_NONE;
		ch0_modulation = MODULATION_AM10KHZ;
	}
	if AREA(80, 140, 90, 25) {
		if (ch0_process == PROCESS_NONE)
			osd = OSD_NONE;
		ch0_process = PROCESS_NONE;
	}
	if AREA(180, 140, 110, 25) {
		if (ch0_process == PROCESS_FREQ)
			osd = OSD_NONE;
		ch0_process = PROCESS_FREQ;
	}
	if AREA(300, 140, 140, 25) {
		if (ch0_process == PROCESS_SPEC_T)
			osd = OSD_NONE;
		ch0_process = PROCESS_SPEC_T;
	}
	if AREA(450, 140, 110, 25) {
		if (ch0_process == PROCESS_SPECTRUM)
			osd = OSD_NONE;
		ch0_process = PROCESS_SPECTRUM;
	}
	if AREA(280, 230, 60, 25) {
		if (ch1_stream == 0)
			osd = OSD_NONE;
		ch1_stream = 0;
	}
	if AREA(350, 230, 60, 25) {
		if (ch1_stream == 1)
			osd = OSD_NONE;
		ch1_stream = 1;
	}
	if AREA(420, 230, 70, 25) {
		ch1_invert = !ch1_invert;
	}
	if AREA(500, 230, 60, 25) {
		ch1_filterdc = !ch1_filterdc;
	}
	if AREA(80, 260, 150, 25) {
		if (ch1_modulation == MODULATION_NONE)
			osd = OSD_NONE;
		ch1_modulation = MODULATION_NONE;
	}
	if AREA(240, 260, 100, 25) {
		if (ch1_modulation == MODULATION_AM1KHZ)
			osd = OSD_NONE;
		ch1_modulation = MODULATION_AM1KHZ;
	}
	if AREA(350, 260, 100, 25) {
		if (ch1_modulation == MODULATION_AM5KHZ)
			osd = OSD_NONE;
		ch1_modulation = MODULATION_AM5KHZ;
	}
	if AREA(460, 260, 100, 25) {
		if (ch1_modulation == MODULATION_AM10KHZ)
			osd = OSD_NONE;
		ch1_modulation = MODULATION_AM10KHZ;
	}
	if AREA(80, 290, 90, 25) {
		if (ch1_process == PROCESS_NONE)
			osd = OSD_NONE;
		ch1_process = PROCESS_NONE;
	}
	if AREA(180, 290, 110, 25) {
		if (ch1_process == PROCESS_FREQ)
			osd = OSD_NONE;
		ch1_process = PROCESS_FREQ;
	}
	if AREA(300, 290, 140, 25) {
		if (ch1_process == PROCESS_SPEC_T)
			osd = OSD_NONE;
		ch1_process = PROCESS_SPEC_T;
	}
	if AREA(450, 290, 110, 25) {
		if (ch1_process == PROCESS_SPECTRUM)
			osd = OSD_NONE;
		ch1_process = PROCESS_SPECTRUM;
	}
}

static void osd_click_help(int x, int y)
{
	if AREA(510, 385, 80, 25) {
		if (help_screen < NUM_HELP_SCREEN-1)
			help_screen++;
	}
	if AREA(50, 385, 80, 25) {
		if (help_screen > 0)
			help_screen--;
	}
}

int osd_click(int x, int y)
{
	if (y < 22) {
		if (x > button_mode_x1 && x < button_mode_x2)
			osd = osd == OSD_MODE ? OSD_NONE : OSD_MODE;
		if (x > button_ld_x1 && x < button_ld_x2)
			osd = osd == OSD_LD ? OSD_NONE : OSD_LD;
		if (x > button_input_x1 && x < button_input_x2)
			osd = osd == OSD_INPUT ? OSD_NONE : OSD_INPUT;
		if (x > button_help_x1 && x < button_help_x2) {
			osd = osd == OSD_HELP ? OSD_NONE : OSD_HELP;
			help_screen = 0;
		}
		drawbuttons();
		return 1;
	}

	if (osd != OSD_NONE && x >= 50 && x < 590 && y >= 50 && y < 410)
	{
		switch (osd)
		{
		case OSD_MODE:
			osd_click_mode(x, y);
			break;
		case OSD_LD:
			osd_click_ld(x, y);
			break;
		case OSD_INPUT:
			osd_click_input(x, y);
			break;
		case OSD_HELP:
			osd_click_help(x, y);
			break;
		}
		drawbuttons();
		return 1;
	}
	return 0;
}

int backframes = 16;
char statusmsg[1024] = "";

unsigned int hlines_n;
unsigned int hlines[10][2];

unsigned int vlines_n;
unsigned int vlines[10][2];

unsigned int tick = 1000;
unsigned int oszibuffer[640][420];

void drawframe_bg(int x, int y, int w, int h, Uint32 bgcolor)
{
	for (int j=y; j<y+h; j++)
	{
		Uint32 *p = (void*)screen->pixels + 4*(640*j+x);
		for (int i=x; i<x+w; i++)
			*(p++) = (j==y || i==x || j==y+h-1 || i==x+w-1) ? 0x00000000 :
					(j==y+1 || i==x+1 || j==y+h-2 || i==x+w-2) ? 0x00ffffff : bgcolor;
	}
}

void drawframe(int x, int y, int w, int h)
{
	drawframe_bg(x, y, w, h, 0x00000000);
}

static void osd_draw_mode()
{

	drawframe_bg(100, 100, 440, 25, display_mode == DISPLAY_MODE_CH1 ? 0x00338833 : 0x00000000);
	drawtext(100, 440, 102, "Channel 1 Only");

	drawframe_bg(100, 150, 440, 25, display_mode == DISPLAY_MODE_CH2 ? 0x00338833 : 0x00000000);
	drawtext(100, 440, 152, "Channel 2 Only");

	drawframe_bg(100, 200, 440, 25, display_mode == DISPLAY_MODE_CH12 ? 0x00338833 : 0x00000000);
	drawtext(100, 440, 202, "Channel 1+2 Split Screen");

	drawframe_bg(100, 250, 440, 25, display_mode == DISPLAY_MODE_XY ? 0x00338833 : 0x00000000);
	drawtext(100, 440, 252, "Channel 1+2 in X/Y Mode (CH1=X, CH2=Y)");
}

static void osd_draw_ld()
{

	drawframe_bg(100, 80, 440, 25, ld_mode == LD_MODE_LINES ? 0x00338833 : 0x00000000);
	drawtext(100, 440, 82, "Connect Samples with Lines");

	drawframe_bg(100, 130, 440, 25, ld_mode == LD_MODE_DOTS ? 0x00338833 : 0x00000000);
	drawtext(100, 440, 132, "Single Dot for every Sample");

	drawframe_bg(100, 180, 440, 25, ld_mode == LD_MODE_AUTO1 ? 0x00338833 : 0x00000000);
	drawtext(100, 440, 182, "Lines for distance < avg distance");

	drawframe_bg(100, 230, 440, 25, ld_mode == LD_MODE_AUTO2 ? 0x00338833 : 0x00000000);
	drawtext(100, 440, 232, "Lines for distance < avg distance * 2");

	drawframe_bg(100, 280, 440, 25, ld_mode == LD_MODE_AUTO5 ? 0x00338833 : 0x00000000);
	drawtext(100, 440, 282, "Lines for distance < avg distance * 5");

	drawframe_bg(100, 330, 440, 25, ld_mode == LD_MODE_SMART ? 0x00338833 : 0x00000000);
	drawtext(100, 440, 332, "Smart line/dot + intesity calculation");
}

static void osd_draw_input()
{
	drawtext(80, 0, 82, "Channel 1 Settings:");

	drawframe_bg(280, 80, 60, 25, ch0_stream == 0 ? 0x00338833 : 0x000000000);
	drawtext(280, 60, 82, "LEFT");

	drawframe_bg(350, 80, 60, 25, ch0_stream == 1 ? 0x00338833 : 0x00000000);
	drawtext(350, 60, 82, "RIGHT");

	drawframe_bg(420, 80, 70, 25, ch0_invert ? 0x00888833 : 0x00000000);
	drawtext(420, 70, 82, "INVERT");

	drawframe_bg(500, 80, 60, 25, ch0_filterdc ? 0x00888833 : 0x00000000);
	drawtext(500, 60, 82, "NODC");

	drawframe_bg(80, 110, 150, 25, ch0_modulation == MODULATION_NONE ? 0x00338833 : 0x00000000);
	drawtext(80, 150, 112, "No Modulation");

	drawframe_bg(240, 110, 100, 25, ch0_modulation == MODULATION_AM1KHZ ? 0x00338833 : 0x00000000);
	drawtext(240, 100, 112, "AM 1 kHz");

	drawframe_bg(350, 110, 100, 25, ch0_modulation == MODULATION_AM5KHZ ? 0x00338833 : 0x00000000);
	drawtext(350, 100, 112, "AM 5 kHz");

	drawframe_bg(460, 110, 100, 25, ch0_modulation == MODULATION_AM10KHZ ? 0x00338833 : 0x00000000);
	drawtext(460, 100, 112, "AM 10 kHz");

	drawframe_bg(80, 140, 90, 25, ch0_process == PROCESS_NONE ? 0x00338833 : 0x00000000);
	drawtext(80, 90, 142, "Value");

	drawframe_bg(180, 140, 110, 25, ch0_process == PROCESS_FREQ ? 0x00338833 : 0x00000000);
	drawtext(180, 110, 142, "Frequency");

	drawframe_bg(300, 140, 140, 25, ch0_process == PROCESS_SPEC_T ? 0x00338833 : 0x00000000);
	drawtext(300, 140, 142, "Spectrum / t");

	drawframe_bg(450, 140, 110, 25, ch0_process == PROCESS_SPECTRUM ? 0x00338833 : 0x00000000);
	drawtext(450, 110, 142, "Spectrum");


	drawtext(80, 0, 232, "Channel 2 Settings:");

	drawframe_bg(280, 230, 60, 25, ch1_stream == 0 ? 0x00338833 : 0x00000000);
	drawtext(280, 60, 232, "LEFT");

	drawframe_bg(350, 230, 60, 25, ch1_stream == 1 ? 0x00338833 : 0x00000000);
	drawtext(350, 60, 232, "RIGHT");

	drawframe_bg(420, 230, 70, 25, ch1_invert ? 0x00888833 : 0x00000000);
	drawtext(420, 70, 232, "INVERT");

	drawframe_bg(500, 230, 60, 25, ch1_filterdc ? 0x00888833 : 0x00000000);
	drawtext(500, 60, 232, "NODC");

	drawframe_bg(80, 260, 150, 25, ch1_modulation == MODULATION_NONE ? 0x00338833 : 0x00000000);
	drawtext(80, 150, 262, "No Modulation");

	drawframe_bg(240, 260, 100, 25, ch1_modulation == MODULATION_AM1KHZ ? 0x00338833 : 0x00000000);
	drawtext(240, 100, 262, "AM 1 kHz");

	drawframe_bg(350, 260, 100, 25, ch1_modulation == MODULATION_AM5KHZ ? 0x00338833 : 0x00000000);
	drawtext(350, 100, 262, "AM 5 kHz");

	drawframe_bg(460, 260, 100, 25, ch1_modulation == MODULATION_AM10KHZ ? 0x00338833 : 0x00000000);
	drawtext(460, 100, 262, "AM 10 kHz");

	drawframe_bg(80, 290, 90, 25, ch1_process == PROCESS_NONE ? 0x00338833 : 0x00000000);
	drawtext(80, 90, 292, "Value");

	drawframe_bg(180, 290, 110, 25, ch1_process == PROCESS_FREQ ? 0x00338833 : 0x00000000);
	drawtext(180, 110, 292, "Frequency");

	drawframe_bg(300, 290, 140, 25, ch1_process == PROCESS_SPEC_T ? 0x00338833 : 0x00000000);
	drawtext(300, 140, 292, "Spectrum / t");

	drawframe_bg(450, 290, 110, 25, ch1_process == PROCESS_SPECTRUM ? 0x00338833 : 0x00000000);
	drawtext(450, 110, 292, "Spectrum");
}

static void osd_draw_help()
{
	static const char *dummy_textdata[] = {
		"??", NULL
	};
	const char **textdata = dummy_textdata;

	if (help_screen == 0) {
		static const char *this_textdata[] = {
		//	"012345678901234567890123456789012345678901234567",
			"MOUSE COMMANDS",
			"==============",
			" ",
			"Drag Left:  Decrase sample window size",
			"Drag Right: Incrase sample window size",
			" ",
			"Drag Down:  Decrase signal amplitude",
			"Drag Up:    Incrase signal amplitude",
			" ",
			"SHIFT + Drag Left/Right:",
			"    1/100th times smaller sample window steps",
			" ",
			"SHIFT + ALT + Drag Left/Right:",
			"    1/10000th times smaller sample window steps",
			NULL
		};
		textdata = this_textdata;
	}

	if (help_screen == 1) {
		static const char *this_textdata[] = {
		//	"012345678901234567890123456789012345678901234567",
			"KYBOARD COMMANDS (1/2)",
			"======================",
			" ",
			"LEFT/RIGHT: Decrase/Incrase sample window size",
			"SHIFT + LEFT/RIGHT: 1/100th times smaller steps",
			" ",
			"UP/DOWN: Large dec/inc sample window size",
			"PAGEUP/PAGEDOWN: Half/Double sample window size",
			" ",
			"1: Reset sample window size",
			"2..7: Divide sample window size by 2..7",
			" ",
			"+/-: Inc/dec display latency",
			NULL
		};
		textdata = this_textdata;
	}

	if (help_screen == 2) {
		static const char *this_textdata[] = {
		//	"012345678901234567890123456789012345678901234567",
			"KYBOARD COMMANDS (2/2)",
			"======================",
			" ",
			"a: Auto-adjust signal amplitude for channel 1",
			"s: Auto-adjust signal amplitude for both channels",
			"d: Auto-adjust signal amplitude for channel 2",
			" ",
			"SPACE: Pause / Resume",
			NULL
		};
		textdata = this_textdata;
	}

	int i = 0;
	for (i=0; textdata[i]; i++) {
		drawtext(80, 0, 82+i*18, textdata[i]);
	}

	drawframe_bg(510, 385, 80, 25, help_screen < NUM_HELP_SCREEN-1 ? 0x00338833 : 0x00000000);
	drawtext(510, 80, 385, ">>");

	drawframe_bg(50, 385, 80, 25, help_screen > 0 ? 0x00338833 : 0x00000000);
	drawtext(50, 80, 385, "<<");
}

void drawscreen()
{
	int hc = 0;
	Uint32 *pixel = screen->pixels + 640*30*4;
	for (int y=0; y<420; y++)
	{
		int vc = 0;
		int hc_active = hc < hlines_n && y == hlines[hc][0];
		for (int x=0; x<640; x++)
		{
			int vc_active = vc < vlines_n && x == vlines[vc][0];
			unsigned int pixel_val = 0;
			if (hc_active)
				pixel_val = hlines[hc][1];
			if (vc_active) {
				pixel_val = vlines[vc][1];
				vc++;
			}
			if (tick == oszibuffer[x][y]) {
				pixel_val = 0x00a0ffa0;
			}
			else
			if (tick - oszibuffer[x][y] < backframes && !hc_active) {
				pixel_val = ((backframes - (tick - oszibuffer[x][y]))*255/backframes) << 8;
			}
			*(pixel++) = pixel_val;
		}
		if (hc_active)
			hc++;
	}

	for (int y=0; y<30; y++)
	for (int x=0; x<640; x++)
	{
		*pixel = 0;
		pixel++;
	}

	if (osd != OSD_NONE)
	{
		drawframe(50, 50, 540, 360);
		switch (osd)
		{
		case OSD_MODE:
			osd_draw_mode();
			break;
		case OSD_LD:
			osd_draw_ld();
			break;
		case OSD_INPUT:
			osd_draw_input();
			break;
		case OSD_HELP:
			osd_draw_help();
			break;
		}
		drawbuttons();
	}

	drawtext(10, 0, 460, statusmsg);
	SDL_UpdateRect(screen, 0, 0, 640, 480);
}

void reset_oszibuffer()
{
	memset(oszibuffer, 0, sizeof(oszibuffer));
}

int ld_mode = LD_MODE_DOTS;

int this_total_pathlen;
int this_total_pathnum;
int last_avg_pathlen;

void drawline(int x1, int y1, int x2, int y2, int force_line)
{
	int w = abs(x2-x1), h = abs(y2-y1);
	int pathlen = w>h ? w+h/8+h/4 : h+w/8+w/4;
	int thistick = tick;

	if (pathlen < 100) {
		this_total_pathlen += pathlen*pathlen;
		this_total_pathnum++;
	}

	if (!force_line)
	switch (ld_mode)
	{
	case LD_MODE_DOTS:
		goto forced_dot;
	case LD_MODE_AUTO1:
		if (pathlen > last_avg_pathlen)
			goto forced_dot;
		break;
	case LD_MODE_AUTO2:
		if (pathlen > 2*last_avg_pathlen)
			goto forced_dot;
		break;
	case LD_MODE_AUTO5:
		if (pathlen > 5*last_avg_pathlen)
			goto forced_dot;
		break;
	case LD_MODE_SMART:
		if (pathlen > last_avg_pathlen/2)
		{
			if (last_avg_pathlen/2 != 0)
				thistick -= pathlen / (last_avg_pathlen/2);
			else
				thistick -= pathlen;
			thistick -= backframes/2;
		}
	}

	if (x1 == x2 && y1 == y2) {
forced_dot:
		oszibuffer[x2%640][y2%420] = thistick;
	} else
	if (abs(x1-x2) > abs(y1-y2)) {
		if (x1>x2) {
			x1 ^= x2; x2 ^= x1; x1 ^= x2;
			y1 ^= y2; y2 ^= y1; y1 ^= y2;
		}
		for (int x = x1; x<=x2; x++)
			oszibuffer[x%640][(y1 + (y2-y1)*(x-x1)/(x2-x1))%420] = thistick;
	} else {
		if (y1>y2) {
			x1 ^= x2; x2 ^= x1; x1 ^= x2;
			y1 ^= y2; y2 ^= y1; y1 ^= y2;
		}
		for (int y = y1; y<=y2; y++)
			oszibuffer[(x1 + (x2-x1)*(y-y1)/(y2-y1))%640][y%420] = thistick;
	}

	if (ld_mode == LD_MODE_SMART)
		oszibuffer[x2%640][y2%420] = tick;
}

void gui_done()
{
}

