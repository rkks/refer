/*
 * Troy D. Hanson, July 2014. 
 */

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "utarray.h"
#include "tpl.h"

/* 
 */

void usage(char *exe) {
  fprintf(stderr,"usage: %s [-v] \n", exe);
  exit(-1);
}

struct {
  int verbose;
  int sample_rate;  /* e.g. 44100 hz */
  int duration;     /* e.g. 10 seconds */
  int resolution;   /* e.g. 2 bytes (16-bit samples) */
  size_t nsamples;  /* number of samples (duration*sample_rate) */
  char *pcm_file;   /* filename to read */
} cfg = {
  .resolution=sizeof(int16_t), 
};

int main (int argc, char *argv[]) {
  char *exe = argv[0];
  int opt;

  while ( (opt = getopt(argc, argv, "t:v+h")) != -1) {
    switch (opt) {
      case 'v': cfg.verbose++; break;
      case 't': cfg.pcm_file=strdup(optarg); break;
      case 'h': default: usage(exe); break;
    }
  }

  if (cfg.pcm_file == NULL) usage(exe);
  
  /* peek into the saved image to see how many samples it has in it */
  uint32_t num_fxlens, *fxlens;
  char *fmt = tpl_peek(TPL_FILE|TPL_FXLENS, cfg.pcm_file, &num_fxlens, &fxlens);
  if (!fmt) {fprintf(stderr,"invalid pcm file %s\n", cfg.pcm_file); goto done;}
  if (num_fxlens<1) {fprintf(stderr,"data err %s\n", cfg.pcm_file); goto done;}
  cfg.nsamples = fxlens[0];
  free(fxlens);

  /* make a buffer to load the PCM data into */
  /* TODO assert cfg.resolution == cfg.resolution in the image */
  size_t pcmlen = cfg.resolution * cfg.nsamples;
  int16_t *pcm;
  pcm = (int16_t*)malloc(pcmlen);
  if (!pcm) {fprintf(stderr,"out of memory\n"); goto done;}
  
  /* read the PCM data */
  tpl_node *tn = tpl_map("iiij#", &cfg.sample_rate, &cfg.duration, &cfg.resolution, 
                         pcm, cfg.nsamples);
  tpl_load(tn, TPL_FILE, cfg.pcm_file);
  tpl_unpack(tn,0);
  tpl_free(tn);

  fprintf(stderr,"read the PCM file: "
                 "duration %u s, sample rate %u hz, resolution %u bits\n",
                 cfg.duration, cfg.sample_rate, cfg.resolution*8);

 done:
  return 0;
}
