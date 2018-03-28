#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>
 
/* either scans a file using the iscan incremental scan; or with -p 
 * it reads input on the parent and sends it via pipe to the child
 * where it is iscanned */

void usage(char *prog) {
  fprintf(stderr, "usage: %s [-v] <pattern> [file]\n", prog);
  fprintf(stderr, "       %s -p <pattern>\n", prog);
  exit(-1);
}


/* try to read 'len' bytes from fd, but tolerate less. see if
 * the bytes match the target pattern, or if there is a match
 * if we slide the 'window' back over previously-
 * read bytes and the prefix of the currently-read buffer. 
 * This gives 'len' possible match windows. */
void iscan(int fd, char *pat, size_t len) {
  int rlen, alen, w, rw;
  off_t at=0;
  char *last = calloc(2,len), *l = &last[len];
  if (!last) {fprintf(stderr,"out of memory\n"); exit(-1);}
  while ( (rlen = read(fd,l,len)) > 0) {
    for(w=0,rw=rlen; w<len && at>=w; w++) {
      if (w+rw > len) rw--;
      if (w+rw == len) {
        if (!memcmp(l-w,pat,w+rw)) printf("found at %p\n",(void*)at-w);
      }
    }
    memmove(last,l-(len-rlen),len); /* slide */
    at += rlen;
  }
  if (rlen == -1) fprintf(stderr,"read failed: %s\n",strerror(errno));
}

/* convert a hex string into a buffer having those bytes */
char *unhex(char *hex, int *len) {
  unsigned int u;
  static unsigned char buf[100], *b = buf;
  int hexlen = strlen(hex);
  *len = hexlen/2;
  if (hexlen & 1) {
    fprintf(stderr,"hex pattern ust have even # digits\n");
    exit(-1);
  }
  while(hexlen) {
    if (sscanf(hex, "%2x", &u) != 1) {
      fprintf(stderr,"invalid hex in pattern\n");
      exit(-1);
    }
    *b++ = (unsigned char)u;
    hex += 2; hexlen -= 2;
  }
  return buf;
}
 
int verbose=0;
int pipemode=0;

 
int main(int argc, char * argv[]) {
  int opt, fd=0, len;
  unsigned char *pattern;
 
  while ( (opt = getopt(argc, argv, "v+p")) != -1) {
    switch (opt) {
      case 'p':
        pipemode++;
        break;
      case 'v':
        verbose++;
        break;
      default:
        usage(argv[0]);
        break;
    }
  }
 
  if (optind < argc) pattern=argv[optind++];
  else usage(argv[0]);
  if (optind < argc) {
    char *file = argv[optind++];
    if ( (fd = open(file, O_RDONLY)) == -1) {
      fprintf(stderr,"can't open %s: %s\n", file, strerror(errno));
      exit(-1);
    }
  }
 
  if (!strncmp(pattern,"0x",2)) 
    pattern=unhex(&pattern[2],&len);
  else 
    len = strlen(pattern);

  if (pipemode) {
    int fds[2];
    pipe(fds);
    if (fork() == 0) { /* child */
      close(fds[1]); /* close write end */
      iscan(fds[0],pattern,len);
      //close(fds[0]); /* close read end */
    } else {           /* parent */
      close(fds[0]); /* close read end */
      char *line;
      while ( (line = readline("enter text to iscan:\n")) != NULL) { 
        write(fds[1], line, strlen(line));
        free(line);
      }
      //close(fds[1]); /* close pipe to child */
    }

  } {
    iscan(fd,pattern,len); /* file mode */
  }
  close(fd);
}
