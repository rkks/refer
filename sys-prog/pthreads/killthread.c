/* 
 * Erzeugt 5 Threads, die mit kill -41 ... -45 einzeln gekillt werden
 * koennen.
 */

#define __EXTENSIONS__ /* fuer prototype fuer sigset() */

#include <pthread.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <strings.h>
#include "wrapper.h"

void *Thread(void *);
void sig_hndl(int sig);
void *SigWaitThread(void *arg);

sigset_t mask;

#define N 5
pthread_t threads[N];
int killed[N];
int nThreads = N;

int
main(int argc, char **argv)
{
	int n;
	pthread_t t;

	sigemptyset(&mask);
	for (n = 0; n < N; ++n)
		sigaddset(&mask, n + 41);
	pthread_sigmask(SIG_SETMASK, &mask, NULL);

	sigset(SIGHUP, sig_hndl);

	for (n = 0; n < N; ++n) {
     		Pthread_create(threads + n, NULL, Thread, NULL);
		printf("created thread #%d = T@%d\n", n, threads[n]);
	}

	printf("PID %d\n", getpid());
	sleep(2);

     	Pthread_create(&t, NULL, SigWaitThread, NULL);
	printf("SigWaitThread thread is T@%d\n", t);

	pthread_exit(NULL);
}

void *
Thread(void *arg)
{
	for (;;) {
		printf("T@%d\n", pthread_self());
		sleep(2);
	}
}

void *
SigWaitThread(void *arg)
{
	int sig, e;

	for (;;) {
		e = sigwait(&mask, &sig);
		if (e < 0) {
			printf("sigwait error\n");
			continue;
		}
		if (sig < 41 || sig > 45) {
			printf("SigWaitThread: signal %d ignored\n", sig);
			continue;
		}
		if (killed[sig - 41]) {
			printf("SigWaitThread: thread T@%d not running\n", 
				threads[sig - 41]);
			continue;
		}

		printf("SigWaitThread: sending signal %d as SIGHUP to T@%d\n", 
			sig, threads[sig - 41]);
		killed[sig - 41] = 1;
		pthread_kill(threads[sig - 41], SIGHUP);

		if (--nThreads == 0) {
			printf("killed last thread\n");
			pthread_exit(NULL);
		}
	}
}

void
sig_hndl(int sig)
{
	char buf[100];

	sprintf(buf, "sig_hndl: signal %d in T@%d, exit\n", 
		sig, pthread_self());
	write(1, buf, strlen(buf));

	pthread_exit(NULL);
}
