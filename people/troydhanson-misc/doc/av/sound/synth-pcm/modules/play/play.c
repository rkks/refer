#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <inttypes.h>
#include "tpl.h"

extern int play_pcm (int argc, char ** argv, int16_t *pcm, size_t pcmlen, 
              int sample_rate, int verbose);

struct {
  int verbose;
  int sample_rate;
  size_t nsamples;
  int resolution;
  int duration; /* sec TODO msec */
} cfg = {
  .resolution=sizeof(int16_t),
};

int main(int argc, char *argv[]) {
  int rc = -1;

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

  play_pcm(argc, argv, pcm, pcmlen, cfg.sample_rate, cfg.verbose);

  /* TODO cycle if requested, reusing buf? */
  rc = 0;

 done:
  if (buf) free(buf);
  return rc;
}
