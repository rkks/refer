
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
 *  starts a subprocess, then terminates it after x time if its running
 *
 */

void usage() {
  fprintf(stderr,"usage: afta <time> cmd args\n");
  fprintf(stderr," <time> e.g. 1s, 2m, 3h (sec or min or hours)\n");
  exit(0);
}

pid_t run_child(int argc, char *argv[]) {
  char **argvn;
  pid_t pid;
  int n;

  //for(n=0;n<argc;n++) fprintf(stderr,"%u: %s\n",n,argv[n]);

  pid = fork();
  if (pid == -1) { fprintf(stderr,"fork error\n"); exit(-1); }
  if (pid > 0) return pid;  /* parent */

  /* child here */

  /* unblock all signals */
  sigset_t all;
  sigemptyset(&all);
  sigprocmask(SIG_SETMASK,&all,NULL);

  /* copy argv so we can add a NULL and exec child */
  argvn = malloc((argc+1) * sizeof(char*));
  memcpy(argvn,argv,argc*sizeof(char*)); argvn[argc]=NULL;
  execvp(argv[0], argvn);
  // not reached
  return 0;
}

int main(int argc, char *argv[]) {
    char suffix,*exe=argv[0];
    int n,opt,verbose=0;
    pid_t pid;

    if (argc < 3) usage(exe); argc--; argv++;
    if (!strcmp(*argv,"-v")) {verbose++; argc--; argv++;}
    n = atoi(*argv); if(n<=0) usage(exe);
    suffix = (*argv)[strlen(*argv)-1];
    if (suffix <= '0' || suffix >= '9') {
      switch (suffix) {
        case 'h': case 'H': n *=  60; /* fall through */
        case 'm': case 'M': n *= 60; break;
        case 's': case 'S': break;
        default: usage(exe);
      }
    }
    argc--; argv++; if (argc == 0) usage(exe);
    if (verbose) fprintf(stderr,"alarm in %u seconds\n", n);

    /* block all signals. accept any signal in sigwaitinfo */
    sigset_t all;
    sigfillset(&all);
    sigprocmask(SIG_SETMASK,&all,NULL);

    alarm(n);
    pid = run_child(argc, argv);

    int signo = sigwaitinfo(&all,NULL); // could use sigtimedwait
    switch(signo) {
      case SIGCHLD: 
        if (verbose) fprintf(stderr,"child exited\n");
        waitpid(pid,NULL,0); 
        break;
      case SIGALRM: 
      default: 
        if (verbose) fprintf(stderr,"quitting sub-process %u\n",pid);
        kill(pid,SIGTERM); if (waitpid(pid,NULL,WNOHANG) > 0) break;
        sleep(1); if (waitpid(pid,NULL,WNOHANG) > 0) break;
        if (verbose) fprintf(stderr,"trying again\n"); kill(pid,SIGKILL); sleep(1);
        if (waitpid(pid,NULL,WNOHANG) > 0) break;
        if (verbose) fprintf(stderr,"failed to terminate sub process %u\n",pid);
        break;
    }
}

