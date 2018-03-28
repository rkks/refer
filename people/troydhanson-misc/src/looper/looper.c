#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
 
/*
 * This is a simple template for a program that opens a file (or reads from
 * stdin if no file is specified), then iterates over the lines of a file.
 */

void usage(char *prog) {
  fprintf(stderr, "usage: %s [-v] <file>\n", prog);
  exit(-1);
}
 
int main(int argc, char * argv[]) {
  int opt,verbose=0,lines=0;
  FILE *ifilef=stdin;
  char *ifile=NULL,line[100];
 
  while ( (opt = getopt(argc, argv, "v+")) != -1) {
    switch (opt) {
      case 'v': verbose++; break;
      default: usage(argv[0]); break;
    }
  }
  if (optind < argc) ifile=argv[optind++];
 
  /* loop over the input */
  if (ifile) {
    if ( (ifilef = fopen(ifile,"r")) == NULL) {
      fprintf(stderr,"can't open %s: %s\n", ifile, strerror(errno));
      exit(-1);
    }
  }
 
  while (fgets(line,sizeof(line),ifilef) != NULL) {
    lines++;
    if (verbose) fprintf(stderr, "read line: %s", line);
  }
  printf("%d lines read.\n",lines);
}
