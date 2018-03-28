#define _GNU_SOURCE
#include <sys/types.h>
#include <errno.h>
#include <sys/inotify.h>
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
int sigs[] = {0,SIGHUP,SIGPIPE,SIGTERM,SIGINT,SIGQUIT,SIGALRM};

void sighandler(int signo, siginfo_t *info, void *ucontext) {
  siglongjmp(jmp,signo);
}

void usage(char *prog) {
  fprintf(stderr,"usage: %s dir [dir ...]\n", prog);
  exit(-1);
}

struct inotify_event *eb;
size_t eb_sz = sizeof(*eb) + PATH_MAX;
int fd;
int setup_watch(int argc, char *argv[]) {
  int i,wd,mask = IN_ALL_EVENTS;
  char *dir;

  for(i=1; i < argc; i++) {
    dir = argv[i];
    if ( (wd = inotify_add_watch(fd, dir, mask)) == -1) {
      perror("inotify_add_watch failed");
      exit(-1); 
    }
  }
  /* see inotify(7) as inotify_event has a trailing name
   * field allocated beyond the fixed structure; we must
   * allocate enough room for the kernel to populate it */
  if ( (eb = malloc(eb_sz)) == NULL) {
    fprintf(stderr, "out of memory\n");
    exit(-1);
  }

  return fd;
}

int read_events(int fd) {
  struct inotify_event *ev, *nx;
  char *dir="<dir>", *name;
  size_t sz;
  int rc;

  while ( (rc=read(fd,eb,eb_sz)) > 0) {
    for(ev = eb; rc > 0; ev = nx) {

      sz = sizeof(*ev) + ev->len;
      nx = (struct inotify_event*)((char*)ev + sz);
      rc -= sz;

      name = (ev->len ? ev->name : dir);
      printf("%s ", name);
      if (ev->mask & IN_ACCESS) printf(" IN_ACCESS");
      if (ev->mask & IN_MODIFY) printf(" IN_MODIFY");
      if (ev->mask & IN_ATTRIB) printf(" IN_ATTRIB");
      if (ev->mask & IN_CLOSE_WRITE) printf(" IN_CLOSE_WRITE");
      if (ev->mask & IN_CLOSE_NOWRITE) printf(" IN_CLOSE_NOWRITE");
      if (ev->mask & IN_OPEN) printf(" IN_OPEN");
      if (ev->mask & IN_MOVED_FROM) printf(" IN_MOVED_FROM");
      if (ev->mask & IN_MOVED_TO) printf(" IN_MOVED_TO");
      if (ev->mask & IN_CREATE) printf(" IN_CREATE");
      if (ev->mask & IN_DELETE) printf(" IN_DELETE");
      if (ev->mask & IN_DELETE_SELF) printf(" IN_DELETE_SELF");
      if (ev->mask & IN_MOVE_SELF) printf(" IN_MOVE_SELF");
      printf("\n");
    }
  }
  return rc;
}

int main(int argc, char *argv[]) {
  int rc, n;
  sigs[0] = SIGRTMIN+0;  /* we'll choose this RT signal for I/O readiness */

  if (argc < 2) usage(argv[0]);
  if ( (fd = inotify_init()) == -1) {
    perror("inotify_init failed");
    exit(-1); 
  }

  /* request SIGIO to our pid when fd is ready; see fcntl(2) */
  int fl = fcntl(fd, F_GETFL);
  fl |= O_ASYNC | O_NONBLOCK;
  fcntl(fd, F_SETFL, fl);
  fcntl(fd, F_SETSIG, sigs[0]); 
  fcntl(fd, F_SETOWN, getpid()); 

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
  sa.sa_sigaction=sighandler;  /* instead of sa_handler; takes extra args */
  sa.sa_flags=SA_SIGINFO;      /* requests extra info in handler */
  sigfillset(&sa.sa_mask);
  for(n=0; n < sizeof(sigs)/sizeof(*sigs); n++) sigaction(sigs[n], &sa, NULL);


  /* here is a special line. we'll come back here whenever a signal happens */
  int signo = sigsetjmp(jmp,1);

  switch (signo) {
    case 0:
      /* initial setup. no signal yet */
      setup_watch(argc,argv);
      break;
    case SIGIO:
      printf("unexpected SIGIO\n");
      break;
    default:
      if (signo == sigs[0]) {
        printf("got SIGRTMIN %d\n", signo);
        rc = read_events(fd);
        if (rc == 0) { close(fd); goto done; }
        if (rc == -1 && errno != EWOULDBLOCK) {perror("error"); close(fd);}
      } else {
        printf("got signal %d\n", signo);
        goto done;
      }
      break;
  }

  /* wait for signals */
  sigsuspend(&ss);

  /* the only way we get past this point
   * is from the "goto done" above, because
   * sigsuspend waits for signals, and when
   * one arrives we longjmp back to sigsetjmp! */

done:
  close(fd);
  return 0;
}
