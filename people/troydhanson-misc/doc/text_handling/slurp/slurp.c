#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/fcntl.h>
 
int verbose=0;
void usage(char *prog) {
  fprintf(stderr, "usage: %s [-v] <file>\n", prog);
  exit(-1);
}
 
char *slurp(char *file, size_t *len) {
  struct stat s;
  char *buf;
  int fd;
  if (stat(file, &s) == -1) {
      fprintf(stderr,"can't stat %s: %s\n", file, strerror(errno));
      exit(-1);
  }
  *len = s.st_size;
  if ( (fd = open(file, O_RDONLY)) == -1) {
      fprintf(stderr,"can't open %s: %s\n", file, strerror(errno));
      exit(-1);
  }
  buf = malloc(*len);
  if (buf) {
    if (read(fd, buf,*len) != *len) {
       fprintf(stderr,"incomplete read\n");
       exit(-1);
    }
  }
  close(fd);
  return buf;
}

int main(int argc, char * argv[]) {
  int opt;
  char *file, *buf;
  size_t len;;
 
  while ( (opt = getopt(argc, argv, "v+")) != -1) {
    switch (opt) {
      case 'v':
        verbose++;
        break;
      default:
        usage(argv[0]);
        break;
    }
  }
 
  if (optind < argc) file=argv[optind++];
  else usage(argv[0]);
  buf = slurp(file, &len);
  printf("slurped %s: %u bytes\n", file, (unsigned)len);
}
