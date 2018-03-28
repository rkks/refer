#include <stdio.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <ncurses.h>
#include "tpl.h"

char *fifo="/tmp/histogram.fifo";
int verbose,bar_width,rows,cols,sum,max;
char buf[4000];

void usage(char *prog) {
  fprintf(stderr,"usage: %s [-v] [-f fifo]\n",prog);
  exit(-1);
}

void draw_bar(int d, int start_col) {
  /* normalize d and figure height */
  int height = (1.0 * d / max) * rows;
  int i,j;
  for(i=start_col; i < start_col+bar_width; i++) {
    for(j=rows-1; j >= rows-height; j--) {
      assert(j >=0); assert( i >= 0); assert(j < rows); assert(i < cols);
      mvaddch(j,i,' ');
    }
  }
}

int main(int argc, char *argv[]) {
  int n, nitems, i, opt, rc,fd, d, *data=NULL, sfd;
  fd_set rfds; 
  char *c;

  /* this block is all about handling SIGWINCH. we convert
     window size changes to readable events in our select loop */
  sigset_t mask;
  sigemptyset(&mask);
  sigaddset(&mask, SIGWINCH);
  if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1) {
    fprintf(stderr, "sigprocmask: %s\n", strerror(errno));
    exit(-1);
  }
  sfd = signalfd(-1, &mask, 0);
  if (sfd == -1) {
    fprintf(stderr, "signalfd: %s\n", strerror(errno));
    exit(-1);
  }

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

 sigwinch:
  initscr();
  getmaxyx(stdscr, rows, cols);
  clear();
  if (verbose) printw("screen is %dx%d. ", rows, cols);
  //printw("waiting for [%s]...", fifo);
  refresh();

  /* wait for data on pipe or keystroke or SIGWINCH */
  while(1) {
    FD_ZERO(&rfds); 
    FD_SET(STDIN_FILENO,&rfds); 
    FD_SET(sfd,&rfds); 
    FD_SET(fd,&rfds); 
    assert(fd > sfd);
    rc = select(fd+1, &rfds, NULL, NULL, NULL);
    switch(rc) {
      case 0: /* timeout */ assert(0); break; 
      case -1: fprintf(stderr,"select: %s\n", strerror(errno)); goto done;
      default: /* one or more descriptors ready */ break;
    }
    if (FD_ISSET(STDIN_FILENO,&rfds)) goto done;
    if (FD_ISSET(sfd,&rfds)) {
      char tmp[1000];
      //fprintf(stderr, "got winch\n");
      read(sfd,tmp,sizeof(tmp)); // clear it
      goto sigwinch;
    }

    /* fifo is readable if we're here.. read and update screen */
    rc = read(fd,buf,sizeof(buf));
    if (rc < 0) {
      if (errno == EAGAIN || errno == EWOULDBLOCK) continue;
      fprintf(stderr,"read: %s\n", strerror(errno)); 
      goto done;
    }
    if (rc == 0) continue;  /* writer closed; wait for another one */
    tpl_node *tn = tpl_map("A(u)",&d);
    if (tpl_load(tn,TPL_MEM,buf,rc) != 0) {
      fprintf(stderr,"tpl_load error\n"); 
      goto done;
    }
    nitems = tpl_Alen(tn,1);
    data = realloc(data, nitems*sizeof(int)); assert(data);
    n=0; sum=0; max=0;
    while(tpl_unpack(tn,1) > 0) {
      data[n++] = d;
      sum += d; 
      if (d > max) max=d;
    }
    tpl_free(tn);

    /* we have our data array in data[]. figure our bar_width */
    bar_width = cols / nitems; // + ((cols % nitems) ? 1 : 0);
    if ((cols < nitems) || (bar_width == 0) || (bar_width * nitems > cols)) {
      fprintf(stderr,"window too narrow for %d bars of width %d (%d cols)\n",
      nitems, bar_width,cols); 
      goto done;
    }

    /* draw nitems bars */
    clear();
    attron(A_REVERSE);
    for(i=0,n=0; n < nitems; i+=bar_width,n++) {
      //fprintf(stderr,"drawing bar %d width %u\n",n,bar_width);
      draw_bar(data[n],i);
    }
    refresh();

  }


 done:
  close(fd);
  endwin();
  return 0;


}
