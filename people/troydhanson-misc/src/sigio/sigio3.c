#define _GNU_SOURCE
#include <sys/types.h>
#include <errno.h>
#include <signal.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <setjmp.h>
#include <string.h>

/* signals that we'll unblock during sigsuspend; first is placeholder
 * because SIGRTMIN isn't necessarily a compile-time define.  */
int sigs[] = {0,SIGHUP,SIGCHLD,SIGTERM,SIGINT,SIGQUIT,SIGALRM};

void spawn_child(int fd[2], char *greeting, char *goodbye) {
  if (fork() != 0) { close(fd[1]); return; }  /* parent */
  close(fd[0]); /* child */
  sleep(1); write(fd[1],greeting,strlen(greeting));
  sleep(1); write(fd[1],goodbye, strlen(goodbye));
  close(fd[1]);
  exit(0);
}

int closed[2]; /* track whether we have closed the two fd's to children */

int main(int argc, char *argv[]) {
  int fl, fa[2], fb[2], rc, n, nclosed=0;
  char buf[100];

  sigs[0] = SIGRTMIN+0;  /* we'll choose this RT signal for I/O readiness */

  /* block all signals. we stay blocked always except in sigwaitinfo */
  sigset_t all;
  sigfillset(&all);
  sigprocmask(SIG_SETMASK,&all,NULL);

  /* a small set of signals we'll accept during sigwaitinfo */
  sigset_t sw;
  sigemptyset(&sw);
  for(n=0; n < sizeof(sigs)/sizeof(*sigs); n++) sigaddset(&sw, sigs[n]);

  pipe(fa); /* to read from child1 */
  pipe(fb); /* to read from child2 */

  /* request signal I/O, SIGRTMIN instead of SIGIO, sent to us on fd ready */
  fl = fcntl(fa[0], F_GETFL); fcntl(fa[0], F_SETFL, fl|O_ASYNC|O_NONBLOCK);
  fl = fcntl(fb[0], F_GETFL); fcntl(fb[0], F_SETFL, fl|O_ASYNC|O_NONBLOCK);
  fcntl(fa[0], F_SETSIG, sigs[0]); fcntl(fa[0], F_SETOWN, getpid());
  fcntl(fb[0], F_SETSIG, sigs[0]); fcntl(fb[0], F_SETOWN, getpid());

  spawn_child(fa, "hello\n", "world\n");
  spawn_child(fb, "enjoy\n", "cocoa\n");

  siginfo_t info;
  int signo,sigfd;

  while ( (signo = sigwaitinfo(&sw, &info)) > 0) {
    if (signo == SIGCHLD) printf("sigchld\n");
    else if (signo == sigs[0]) { /* SIGRTMIN */
      sigfd = info.si_fd;
      /* we must check if this fd is already closed; because a signal
       * for I/O may have been queued before we issued final close */
      if ((closed[0] == sigfd) || (closed[1] == sigfd)) continue;
      do {
        rc = read(sigfd, buf, sizeof(buf));
        if (rc > 0) printf("read(%d): %.*s", sigfd, rc, buf);
      } while (rc > 0);

      if (rc == 0)  {
        close(sigfd);
        closed[nclosed++] = sigfd;
        if (nclosed==2) break;
      }
      else if (rc == -1) {
        if (!(errno == EWOULDBLOCK || errno == EAGAIN)) {
          perror("read error"); 
          break;
        }
      }
    } else {
      printf("got signal %d\n", signo);
      break;
    }
  }

  return 0;
}
