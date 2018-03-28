
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <signal.h>
#include <setjmp.h>
#include <sys/wait.h>
#include <sys/types.h>

/*
 *  This is a managed-worker implementation using signals.
 *  The original process starts a number of sub-processes,
 *  then restarts them when they die (delaying if too fast).
 *
 *  This is similar to the sigsuspend-based version but it
 *  uses sigwaitinfo (eliminating the signal handler entirely)
 *  to accept signals synchronously (in the main program).
 */
#define SHORT_DELAY 10

int wr;    /* workers running */
int wn=2;  /* workers needed */
int ed=10; /* exit delay for child */

typedef struct {
  pid_t pid;
  time_t start;
} worker_t;

worker_t *workers;

void usage() {
  fprintf(stderr,"usage: mworker [-n servers] [-d delay]\n");
  exit(0);
}

void worker(int w) {
  pid_t pid;
  int n;

  if ( (pid = fork()) == -1) {
    printf("fork error\n"); 
    exit(-1); 
  }
  if (pid > 0) { /* parent. */
    printf("worker %d started\n", (int)pid);
    /* record worker */
    workers[w].pid = pid;
    workers[w].start = time(NULL);
    return;
  } 

  /* child here */
  /* setproctitle() or prctl(PR_SET_NAME) */

  /* unblock all signals */
  sigset_t all;
  sigemptyset(&all);
  sigprocmask(SIG_SETMASK,&all,NULL);

  /* do something to simulate real work */
  sleep(ed);
  exit(0);
}

int main(int argc, char *argv[]) {
    pid_t pid;
    int n,es,defer_restart,opt;

    while ( (opt = getopt(argc, argv, "n:d:h")) != -1) {
      switch (opt) {
        case 'n': wn = atoi(optarg); break;
        case 'd': ed = atoi(optarg); break;
        default: usage(); break;
      }
    }

    if ( (workers = malloc(sizeof(worker_t)*wn)) == NULL) 
        exit(-1);

    /* block all signals. we stay blocked always except in sigwaitinfo */
    sigset_t all;
    sigfillset(&all);
    sigprocmask(SIG_SETMASK,&all,NULL);

    /* initial setup: bring up wn workers */
    while(wr < wn) worker(wr++); 

    while(1) {
      int signo = sigwaitinfo(&all,NULL);
      switch(signo) {
          case SIGCHLD:
            /* loop over children that have exited */
            defer_restart=0;
            while( (pid = waitpid(-1,&es,WNOHANG)) > 0) {
                for(n=0; n < wr; n++) if (workers[n].pid==pid) break;
                assert(n != wr);
                int elapsed = time(NULL) - workers[n].start;
                if (elapsed < SHORT_DELAY) defer_restart=1;
                printf("pid %d exited after %d seconds: ", (int)pid, elapsed);
                if (WIFEXITED(es)) printf("exit status %d\n", (int)WEXITSTATUS(es));
                else if (WIFSIGNALED(es)) printf("signal %d\n", (int)WTERMSIG(es));
                if (n < wr-1) memmove(&workers[n],&workers[n+1],sizeof(worker_t)*(wr-1-n));
                wr--;
            }
            if (defer_restart) {
              fprintf(stderr,"servers restarting too fast, delaying\n"); 
              alarm(SHORT_DELAY); 
            }
            else while(wr < wn) worker(wr++); /* bring up wn workers */
            break;
          case SIGALRM:
            while(wr < wn) worker(wr++); /* bring up wn workers */
            break;
          default:
            printf("got signal %d\n", signo);
            goto done;
            break;
      }
    }

done:
  /* reap any running workers */
  while (wr) {
    n = --wr;
    printf("terminating pid %d\n", workers[n].pid);
    kill(workers[n].pid, SIGTERM);
    waitpid(workers[n].pid, NULL, 0);
  }
}

