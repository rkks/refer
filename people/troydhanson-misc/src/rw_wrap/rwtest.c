#include <stdio.h>
#include "rw_wrap.h"

int main() {
  int fds[2];
  pipe(fds);
  if (fork() == 0) { /* child */
    char buf[10];
    close(fds[1]);
    do_read(fds[0],buf,sizeof(buf));
    printf("child: got 10 bytes\n");
  } else { /* parent */
    printf("parent: writing 5 bytes\n");
    do_write(fds[1],"abcde",5);
    sleep(5);
    printf("parent: writing 5 bytes\n");
    do_write(fds[1],"hijkl",5);
    sleep(5);
    close(fds[0]);
  }
}
