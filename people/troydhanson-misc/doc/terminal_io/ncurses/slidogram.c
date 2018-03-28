#include <stdio.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>

char *fifo="/tmp/slidogram.fifo";
int verbose;

void usage(char *prog) {
  fprintf(stderr,"usage: %s [-v] [-f fifo]\n",prog);
  exit(-1);
}

char buf[1000];
int main(int argc, char *argv[]) {
  int i, j, opt, rc,fd, rows, cols, *data, d, max, height, bytes;
  fd_set rfds; 
  char *c;

  while ( (opt=getopt(argc,argv,"vf:h")) != -1) {
    switch(opt) {
      case 'v': verbose++; break;
      case 'f': fifo=strdup(optarg); break;
      default: case 'h': usage(argv[0]); break;
    }
  }

  umask(0); /* so others can write to our fifo */
  if ( ((rc = mkfifo(fifo,0622)) == -1) && (errno != EEXIST)) {
    fprintf(stderr,"cannot make fifo %s: %s\n", fifo, strerror(errno));
    return -1;
  }

  if ( (fd = open(fifo,O_NONBLOCK)) == -1) {
    fprintf(stderr,"cannot open fifo %s: %s\n", fifo, strerror(errno));
    return -1;
  }

  initscr();
  getmaxyx(stdscr, rows, cols);
  if (verbose) printw("screen is %dx%d. ", rows, cols);
  printw("waiting for [%s]...", fifo);
  refresh();

  data = calloc(cols,sizeof(int));

  /* wait for data on pipe or keystroke to exit */
  while(1) {
    FD_ZERO(&rfds); FD_SET(STDIN_FILENO,&rfds); FD_SET(fd,&rfds);
    rc = select(fd+1, &rfds, NULL, NULL, NULL);
    switch(rc) {
      case 0: /* timeout */
        assert(0); 
        break; 
      case -1: 
        fprintf(stderr,"select: %s\n", strerror(errno)); 
        goto done;
      default: /* one or both descriptors ready */
        break;
    }
    if (FD_ISSET(STDIN_FILENO,&rfds)) goto done;

    /* fifo is readable if we're here.. read and update screen */
    rc = read(fd,buf,sizeof(buf)-1);
    if (rc < 0) {
      if (errno == EAGAIN || errno == EWOULDBLOCK) continue;
      fprintf(stderr,"read: %s\n", strerror(errno)); 
      goto done;
    }
    if (rc == 0) continue;  /* writer closed; wait for another one */
    buf[rc]='\0'; /* so we can use it with sscanf */
    c = buf;
    while (rc) {
      if (sscanf(c,",%u%n",&d,&bytes) == 0) {
        fprintf(stderr,"syntax error\n"); 
        goto done; 
      }
      //fprintf(stderr,"read %d bytes from [%.*s]\n",bytes,rc,c);
      assert(bytes <= rc);
      rc -= bytes;
      c += bytes;
      for(i=1;i<cols;i++) data[i-1] = data[i]; data[cols-1] = d;
    }

    /* push data into last column, shifting other columns left to make room */
    for(max=0, i=0;i<cols;i++) {if (data[i]>max) max=data[i];}

    /* draw and refresh */
    clear();
    attron(A_REVERSE);
    for(i=0; i<cols; i++) {
      height = (int)((1.0 * data[i] / max) * rows);
      for(j=rows-1; j >= rows-height; j--) {
        assert(j >= 0); assert(i >= 0); assert(j < rows); assert(i < cols);
        mvaddch(j,i,' ');
      }
    }
    refresh();
  }


 done:
  close(fd);
  endwin();
  return 0;


}
