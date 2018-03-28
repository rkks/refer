#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/fcntl.h>
 
int verbose=0;
void usage(char *prog) {
  fprintf(stderr, "usage: %s [-v] <file>\n", prog);
  exit(-1);
}
 
char *map(char *file, size_t *len) {
  struct stat s;
  char *buf;
  int fd;

  if ( (fd = open(file, O_RDONLY)) == -1) {
      fprintf(stderr,"can't open %s: %s\n", file, strerror(errno));
      exit(-1);
  }
  if (fstat(fd, &s) == -1) {
      close(fd);
      fprintf(stderr,"can't stat %s: %s\n", file, strerror(errno));
      exit(-1);
  }
  *len = s.st_size;
  buf = mmap(0, s.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (buf == MAP_FAILED) {
    close(fd);
    fprintf(stderr, "failed to mmap %s: %s\n", file, strerror(errno));
    exit(-1);
  }
  /* don't: close(fd); */
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
  buf = map(file, &len);
  printf("mmap'd %s: %u bytes\n", file, (unsigned)len);
}
