#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "tpl.h"
 
extern int layout(char *buf, size_t sz,char **obuf, size_t *osz);

/*
 * test driver for layout.c
 * consumes a file (stdin or the named file)
 * and prints the resulting layout
 * also writes a tpl image that can be loaded
 * into draw-cairo/render-png
 */

void usage(char *prog) {
  fprintf(stderr, "usage: %s [-v] [-i] <file>\n", prog);
  fprintf(stderr, "   -i writes layout tpl to stdout\n");
  exit(-1);
}
 
int main(int argc, char * argv[]) {
  int opt,verbose=0, dump_image=0, x, y;
  FILE *ifilef=stdin;
  char *ifile=NULL,line[100], *s;
 
  tpl_node *tn;
  char *buf, *obuf;
  size_t sz,  osz;

  while ( (opt = getopt(argc, argv, "v+ih")) != -1) {
    switch (opt) {
      case 'v': verbose++; break;
      case 'i': dump_image=1; break;
      case 'h': default: usage(argv[0]); break;
    }
  }
  if (optind < argc) ifile=argv[optind++];
 
  if (ifile) {
    if ( (ifilef = fopen(ifile,"r")) == NULL) {
      fprintf(stderr,"can't open %s: %s\n", ifile, strerror(errno));
      exit(-1);
    }
  }

  /* prepare input buffer for layout algorithm */
  s = line;
  tn = tpl_map("A(s)", &s);
  while (fgets(line,sizeof(line),ifilef) != NULL) tpl_pack(tn, 1);
  tpl_dump(tn, TPL_MEM, &buf, &sz);
  tpl_free(tn);

  /* perform layout */
  layout(buf,sz,&obuf,&osz);
  free(buf);

  /* inspect output buffer from layout algorithm */
  tn = tpl_map("A(sii)", &s, &x, &y);
  if (tpl_load(tn, TPL_MEM, obuf, osz)) exit(-1);
  while (tpl_unpack(tn,1) > 0) {
    if (verbose) fprintf(stderr,"%-5d %-5d    %s", x, y, s);
    free(s);
  }
  tpl_free(tn);
  if (dump_image) write(STDOUT_FILENO, obuf, osz);
  free(obuf);

  fclose(ifilef);
  return 0;
}
