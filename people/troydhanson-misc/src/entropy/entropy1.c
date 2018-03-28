#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
 
/*
 * Calculaty entropy on a stream. Entropy is updated for each symbol.
 * The final entropy is printed when the end of the stream occurs.
 */

void usage(char *prog) {
  fprintf(stderr, "usage: %s [-v] [file]\n", prog);
  exit(-1);
}
 
unsigned counts[256];
unsigned total;

int main(int argc, char * argv[]) {
  int opt,verbose=0,i;
  FILE *ifilef=stdin;
  char *ifile=NULL,line[100];
  double p, lp, sum=0;
 
  while ( (opt = getopt(argc, argv, "v+")) != -1) {
    switch (opt) {
      case 'v': verbose++; break;
      default: usage(argv[0]); break;
    }
  }
  if (optind < argc) ifile=argv[optind++];
 
  if (ifile) {
    if ( (ifilef = fopen(ifile,"r")) == NULL) {
      fprintf(stderr,"can't open %s: %s\n", ifile, strerror(errno));
      exit(-1);
    }
  }
 
  /* accumulate counts of each byte value */
  while ( (i=fgetc(ifilef)) != EOF) {
    counts[i]++;
    total++;
  }

  /* compute the final entropy. this is -SUM[0,255](p*l(p))
   * where p is the probability of byte value [0..255]
   * and l(p) is the base-2 log of p. Unit is bits per byte.
   */
  for(i=0; i < 256; i++) {
    if (counts[i] == 0) continue;
    p = 1.0*counts[i]/total;
    lp = log(p)/log(2);
    sum -= p*lp;
  }
  printf("%.2f bits per byte\n", sum);
}
