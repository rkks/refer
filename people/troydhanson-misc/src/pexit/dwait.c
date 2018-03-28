#include <stdio.h>
#include <sys/inotify.h>
#include <sys/prctl.h>
#include <signal.h>
char ev[500];
int main(int argc, char *argv[]) {
  if (argc > 1) {
    prctl(PR_SET_PDEATHSIG, SIGHUP);
  }
  int id = inotify_init();
  int iw = inotify_add_watch(id, ".", IN_CLOSE_WRITE);
  read(id, ev, sizeof(ev));
  close(id);
}
