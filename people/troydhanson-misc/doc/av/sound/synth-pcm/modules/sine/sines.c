/*
 * This module takes command-line options describing one or more sine waves
 * (amplitude,freq,phase) and then builds a PCM buffer (e.g. 44.1khz, 16-bit)
 *
 * Troy D. Hanson
 */

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "utarray.h"
#include "tpl.h"

/* 
 * generate a sum of sine waves and play the waveform.
 *
 * Each sine wave is defined by a tuple of:
 *   frequency: in hz [2 - 20000 are reasonable values]
 *   amplitude: as the upper half of a 16-bit range [0-32767]
 *       phase: as an angle in degrees (0-360)
 * 
 */
void usage(char *exe) {
  fprintf(stderr,"usage: %s [-v] [-r <sample-rate>] (in Hz, e.g. 44100) \n"
                 "               [-s <seconds>]     (duration, seconds) \n"
                 "               freq[/amp[/phase]] (sine parameters, repeatable)\n"
                 "\nSine parameters consist of:                         \n"
                 "Frequency (hz)  (2-20000) this is thly required parameter\n"
                 "Amplitude       (0-32767) max positive amplitude @16-bit\n"
                 "Phase (degrees) (0-360)   offset vs relative sines\n", exe);
  exit(-1);
}

struct {
  UT_array *sines;
  int verbose;
  int sample_rate;  /* e.g. 44100 hz */
  int duration;     /* e.g. 10 seconds */
  int resolution;   /* e.g. 2 bytes (16-bit samples) */
} cfg = {
  .sample_rate=44100,
  .duration = 4, /* seconds of pcm audio to produce */
  .resolution=sizeof(int16_t), /* fixed in this program */
};

typedef struct {
  int freq;
  int amp;
  double phase; // radians
} sine_t;

UT_icd sines_icd = {.sz = sizeof(sine_t)};

int main (int argc, char *argv[]) {
  char *exe = argv[0];
  int16_t *pcm=NULL;
  sine_t sine;
  int opt;

  utarray_new(cfg.sines, &sines_icd);
  while ( (opt = getopt(argc, argv, "s:r:v+h")) != -1) {
    switch (opt) {
      case 'v': cfg.verbose++; break;
      case 'r': cfg.sample_rate=atoi(optarg); break;
      case 's': cfg.duration=atoi(optarg); break;
      case 'h': default: usage(exe); break;
    }
  }
  if (optind >= argc) usage(exe);

  while (optind < argc) {
    char *spec = argv[optind++];
    char *f,*a,*p,*slash;
    int freq, amp=32767/2, phase=0,end=0;
    f = spec;  // start of freq
    slash = f+1; 
    while(*slash != '/' && *slash != '\0') slash++; 
    if (*slash == '\0') end=1;
    else (*slash = '\0');
    freq = atoi(f);
    if (end) goto push;

    a=slash+1; // start of amp
    slash=a+1;
    while(*slash != '/' && *slash != '\0') slash++; 
    if (*slash == '\0') end=1;
    else (*slash = '\0');
    amp = atoi(a);
    if (end) goto push;

    p=slash+1; // start of phase
    phase = atoi(p);

   push:
    sine.freq = freq;
    sine.amp = amp;
    sine.phase = phase/360.0 * 2*M_PI; /* degrees to radians */
    utarray_push_back(cfg.sines, &sine);
  }

  /* set up a memory buffer of the appropriate duration */
  // (samples/sec * bytes/sample * sec) = bytes
  size_t nsamples = cfg.sample_rate * cfg.duration;
  size_t pcmlen = cfg.resolution * nsamples;
  pcm = (int16_t*)malloc(pcmlen);
  if (!pcm) {fprintf(stderr,"out of memory\n"); goto done;}
  memset(pcm, 0, pcmlen); 


  /* now iterate over them and produce a PCM waveform in memory */
  sine_t *s=NULL;
  double sec_per_sample = 1.0/cfg.sample_rate;
  while ( (s=(sine_t*)utarray_next(cfg.sines,s))) {
    fprintf(stderr,"amp: %d, freq: %d, phase: %.2f\n", s->amp, s->freq, s->phase);
    /* generate every sample. calculate the wave at each sample. */
    size_t i;
    for(i=0; i < cfg.duration*cfg.sample_rate; i++) {
      double t = (i*sec_per_sample); // time of sample
      double w = 2*M_PI*s->freq;     // ω (omega) angular freq (radians/sec)
      double p = s->phase;           // ϕ (phi) phase (radians)
      int    A = s->amp;             // amplitude 
      pcm[i] += A*sin(w*t + p);      // A*sin(ωt+ϕ)
    }
  }
  /* write it out with basis parameters */
  tpl_jot( TPL_FD, STDOUT_FILENO, "iiij#", 
    &cfg.sample_rate, &cfg.duration, &cfg.resolution, pcm, nsamples);

  /* TODO cycle if requested */

 done:
  if (pcm) free(pcm);
  utarray_free(cfg.sines);
  return 0;
}
