#include <sys/inotify.h>
#include <pthread.h>
#include <stdlib.h>
#include <signal.h>
#include <setjmp.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <stdio.h>

void usage(char *prog) {
  fprintf(stderr, "usage: %s [-v] [-n num]\n", prog);
  exit(-1);
}

int verbose;
int nthread=2;
int shutdown;
char *dir="/tmp";

typedef struct work_t {
  char *file;
  struct work_t *next;
} work_t;
/* our work queue, its mutex, and worker function */
work_t *head,*tail;
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
void *worker(void *data) {
  /* block all signals */
  sigset_t all;
  sigfillset(&all);
  pthread_sigmask(SIG_BLOCK, &all, NULL);

 again:
  pthread_mutex_lock(&mtx);
  while (!head && !shutdown) pthread_cond_wait(&cond, &mtx);
  if (shutdown) { 
        pthread_mutex_unlock(&mtx);
        return NULL;
  }
  work_t *w = head;
  head = head->next;
  if (!head) tail=NULL;
  fprintf(stderr,"thread %d claimed %s\n", (int)data, w->file);
  pthread_mutex_unlock(&mtx);

  /* do work with w */
  sleep(1);
  free(w->file);
  free(w);

  goto again;
}

union {
  struct inotify_event ev;
  char buf[sizeof(struct inotify_event) + PATH_MAX];
} eb;

char *get_file(int fd, void **nx) {
  struct inotify_event *ev;
  static int rc=0;
  size_t sz;

  if (*nx) ev = *nx;
  else {
    rc = read(fd,&eb,sizeof(eb));
    if (rc < 0) return NULL;
    ev = &eb.ev;
  }

  sz = sizeof(*ev) + ev->len;
  rc -= sz;
  *nx = (rc > 0) ? ((char*)ev + sz) : NULL;
  return ev->len ? ev->name : dir;
}

sigjmp_buf jmp;
int sigs[] = {SIGHUP,SIGTERM,SIGINT,SIGQUIT,SIGALRM,SIGUSR1};
void sighandler(int signo) {
  siglongjmp(jmp,signo);
}

int main(int argc, char *argv[]) {
  int opt, fd, wd, i, n, s;
  pthread_t *th;
  void *nx=NULL,*res;
  work_t *w;
  char *f;

  while ( (opt = getopt(argc, argv, "v+n:d:")) != -1) {
    switch(opt) {
      case 'v': verbose++; break;
      case 'n': nthread=atoi(optarg); break;
      case 'd': dir=strdup(optarg); break;
      default: usage(argv[0]);
    }
  }
  if (optind < argc) usage(argv[0]);

  /* establish handlers for signals that we catch */
  struct sigaction sa;
  sa.sa_handler=sighandler; 
  sa.sa_flags=0;
  sigfillset(&sa.sa_mask);
  for(n=0; n < sizeof(sigs)/sizeof(*sigs); n++) sigaction(sigs[n], &sa, NULL);

  /* block signals except for the few we'll catch */
  sigset_t ss;
  sigfillset(&ss);
  for(n=0; n < sizeof(sigs)/sizeof(*sigs); n++) sigdelset(&ss, sigs[n]);
  pthread_sigmask(SIG_SETMASK, &ss, NULL);

  /* start up our threads */
  th = malloc(sizeof(pthread_t)*nthread);
  for(i=0; i < nthread; i++) pthread_create(&th[i],NULL,worker,(void*)i);

  fd = inotify_init();
  wd = inotify_add_watch(fd,dir,IN_CLOSE);


  int signo = sigsetjmp(jmp,1);
  if (signo) goto done;

  while ( (f=get_file(fd,&nx))) {
    w = malloc(sizeof(*w)); w->file=strdup(f); w->next=NULL;
    pthread_mutex_lock(&mtx);
    if (tail) { tail->next=w; tail=w;} else head=tail=w;
    if (verbose) fprintf(stderr,"queued %s\n", w->file);
    pthread_mutex_unlock(&mtx);
    pthread_cond_signal(&cond);
  }

 done:

  /* if we're here, we got a signal or error. cancel threads */
  shutdown=1; pthread_cond_broadcast(&cond);
  for(i=0; i < nthread; i++) {
    //pthread_cancel(th[i]);
    pthread_join(th[i],&res);
    fprintf(stderr,"thread %d %sed\n",i,res==PTHREAD_CANCELED?"cancel":"exit");
  }
  close(fd);
}
