#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include "tpl.h"

char *fifo="/tmp/histogram.fifo";

int main() {
  tpl_node *tn;
  int i,j,fd;


  if ( (fd = open(fifo,O_WRONLY)) == -1) {
    fprintf(stderr,"cannot open fifo %s: %s\n", fifo, strerror(errno));
    return -1;
  }

  tn = tpl_map("A(u)",&i);
  for(i=0;i<10;i++) tpl_pack(tn,1);
  tpl_dump(tn,TPL_FD,fd);
  tpl_free(tn);

  sleep(2);
  tn = tpl_map("A(u)",&j);
  j=1;
  for(i=0;i<10;i++) tpl_pack(tn,1);
  tpl_dump(tn,TPL_FD,fd);
  tpl_free(tn);

  sleep(2);
  tn = tpl_map("A(u)",&j);
  for(i=0;i<10;i++) {j = (i ==3)?1:0; tpl_pack(tn,1);}
  tpl_dump(tn,TPL_FD,fd);
  tpl_free(tn);

}
