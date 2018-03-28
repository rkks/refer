#include <sys/types.h>
#include <signal.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <setjmp.h>

sigjmp_buf jmp;
/* signals that we'll unblock during sigsuspend */
int sigs[] = {SIGIO,SIGHUP,SIGCHLD,SIGTERM,SIGINT,SIGQUIT,SIGALRM};

void sighandler(int signo) {
  siglongjmp(jmp,signo);
}

void spawn_child(int fd[2], char *greeting, char *goodbye) {
  if (fork() != 0) return;  /* parent */
  close(fd[0]);
  sleep(1); write(fd[1],greeting,strlen(greeting));
  sleep(1); write(fd[1],goodbye, strlen(goodbye));
  close(fd[1]);
  exit(0);
}

int main(int argc, char *argv[]) {
  int fd[2], rc, n;
  char buf[100];

  pipe(fd);
  spawn_child(fd,"hello\n","world\n");
  close(fd[1]);

  /* request SIGIO to our pid when fd[0] is ready; see fcntl(2) */
  int fl = fcntl(fd[0], F_GETFL);
  fl |= O_ASYNC | O_NONBLOCK;
  fcntl(fd[0], F_SETFL, fl);
  fcntl(fd[0], F_SETOWN, getpid()); 

  /* block all signals. we stay blocked always except in sugsuspend */
  sigset_t all;
  sigfillset(&all);
  sigprocmask(SIG_SETMASK,&all,NULL);

  /* a smaller set of signals we'll block during sigsuspend */
  sigset_t ss;
  sigfillset(&ss);
  for(n=0; n < sizeof(sigs)/sizeof(*sigs); n++) sigdelset(&ss, sigs[n]);

  /* establish handlers for signals that'll be unblocked in sigsuspend */
  struct sigaction sa;
  sa.sa_handler=sighandler;  /* no fd information (not sa_sigaction)  */
  sa.sa_flags=0;             /* no extra information (not SA_SIGINFO* */
  sigfillset(&sa.sa_mask);
  for(n=0; n < sizeof(sigs)/sizeof(*sigs); n++) sigaction(sigs[n], &sa, NULL);

  /* here is a special line. we'll come back here whenever a signal happens */
  int signo = sigsetjmp(jmp,1);

  switch (signo) {
    case 0:
      /* initial setup. no signal yet */
      break;
    case SIGCHLD:
      /* this can happen before we get the last SIGIO,
         so fall through to grab any final pipe content */
      printf("got sigchld\n");
    case SIGIO:
      rc = read(fd[0], buf, sizeof(buf));
      if (rc > 0) printf("read: %.*s", rc, buf);
      else if (rc == 0) { close(fd[0]); goto done; }
      else if (rc == -1) {perror("read error"); close(fd[0]);}
      break;
    default:
      printf("got signal %d\n", signo);
      goto done;
      break;
  }

  /* wait for signals */
  sigsuspend(&ss);

  /* the only way we get past this point
   * is from the "goto done" above, because
   * sigsuspend waits for signals, and when
   * one arrives we longjmp back to sigsetjmp! */

done:
  return 0;
}
