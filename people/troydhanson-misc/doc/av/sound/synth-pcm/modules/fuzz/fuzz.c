#include <stdio.h>
#include <getopt.h>
#include <unistd.h>
#include <stdlib.h>
#include <inttypes.h>
#include "tpl.h"

struct {
  int verbose;
  int a;
  int b;
  int sample_rate;
  size_t nsamples;
  int resolution;
  int duration; /* sec TODO msec */
} cfg = {
  .resolution=sizeof(int16_t),
};

void usage(char *exe) {
  fprintf(stderr,"usage: %s [-v] -a <fuzz-bottom> -b <fuzz-top>\n", exe);
  fprintf(stderr,"fuzz scales samples with amplitude>a into [a,b]\n");
  exit(-1);
}

int main(int argc, char *argv[]) {
  int rc = -1;
  int opt;
  char *exe=argv[0];

  while ( (opt=getopt(argc,argv,"v+a:b:h")) != -1) {
    switch (opt) {
      case 'v': cfg.verbose++; break;
      case 'a': cfg.a = atoi(optarg); break;
      case 'b': cfg.b = atoi(optarg); break;
      default: case 'h': usage(exe); break;
    }
  }

  void *buf=NULL;
  size_t sz;
  if (tpl_gather(TPL_GATHER_BLOCKING, STDIN_FILENO, &buf, &sz) <= 0) goto done;

  /* peek into the saved image to see how many samples it has in it */
  uint32_t num_fxlens, *fxlens;
  char *fmt = tpl_peek(TPL_MEM|TPL_FXLENS, buf, sz, &num_fxlens, &fxlens);
  if ((!fmt) || (num_fxlens<1)) {fprintf(stderr,"invalid buffer\n"); goto done;}
  cfg.nsamples = fxlens[0];
  free(fxlens);

  /* make a buffer to load the PCM data into */
  /* TODO assert cfg.resolution == cfg.resolution in the image */
  size_t pcmlen = cfg.resolution * cfg.nsamples;
  int16_t *pcm;
  pcm = (int16_t*)malloc(pcmlen);
  if (!pcm) {fprintf(stderr,"out of memory\n"); goto done;}
  
  tpl_node *tn = tpl_map("iiij#", &cfg.sample_rate, &cfg.duration, &cfg.resolution, 
                         pcm, cfg.nsamples);
  tpl_load(tn, TPL_MEM, buf, sz);
  tpl_unpack(tn,0);
  tpl_free(tn);

  if (cfg.verbose) fprintf(stderr,"read the PCM file: "
                 "duration %u s, sample rate %u hz, resolution %u bits\n",
                 cfg.duration, cfg.sample_rate, cfg.resolution*8);

  /* loop over the samples applying our configured fuzzbox 
   * (peak compression) effect. samples whose amplitude exceeds
   * a threshold get scaled to a subrange 'squaring up' peaks
   */

  // the scale factor is used in this way
  // f' = (f-a) * s + a
  // to produce a new amplitude for a sample
  // in the output domain [a,b]
  int amp_max = 32767; // max positive value of 16-bit signed 
  double s = (amp_max - cfg.a) * 1.0 / 
             ( cfg.b - cfg.a);

  size_t n;
  for(n=0; n < cfg.nsamples; n++) {
    int16_t abs_n = abs(pcm[n]);
    if (abs_n < cfg.a) continue;
    pcm[n] = (((abs_n-cfg.a) * s) + cfg.a) *  // scale sample
             (pcm[n] < 0 ? -1 : 1);           // restore sign 
  }

  tpl_jot( TPL_FD, STDOUT_FILENO, "iiij#", 
    &cfg.sample_rate, &cfg.duration, &cfg.resolution, pcm, cfg.nsamples);

  free(pcm); // TODO recycle this? utstring


  /* TODO cycle if requested, reusing buf? */
  rc = 0;

 done:
  if (buf) free(buf);
  return rc;
}
