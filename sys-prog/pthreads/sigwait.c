/* 
 * sigwait() Demo. Ein spezieller sigwait thread faengt alle moeglichen 
 * Signale ausser SIGHUP auf und leitet sie als SIGHUP an einen festen
 * Thread weiter.
 *
 * Aufruf: sigwait thread-nr [b]
 *         thread-nr : 1 - 10, Thread an den signal weitergeleitet wird
 *	   b         : bound threads
 */

#define __EXTENSIONS__ /* fuer prototype fuer sigset() */

#include <pthread.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <strings.h>
#include <stdlib.h>
#include "wrapper.h"

void *Thread(void *);
void sig_hndl(int sig);
void *SigWaitThread(void *arg);

int sys_scope;
pthread_t receiver;
sigset_t mask;

int
main(int argc, char **argv)
{
	int n;
	pthread_t t;
	pthread_attr_t attr;
	int scope;
	int thread_no;

	if (argc <= 1) {
		printf("usage: %s thread-nr [b]\n", argv[0]);
		exit(1);
	}
	thread_no = atoi(argv[1]);
	if (thread_no <= 0 || thread_no > 10)
		thread_no = 1;

	if (argc > 2) {
		printf("using PTHREAD_SCOPE_SYSTEM\n");
		scope = PTHREAD_SCOPE_SYSTEM;
		sys_scope = 1;
	}
	else {
		printf("using PTHREAD_SCOPE_PROCESS\n");
		scope = PTHREAD_SCOPE_PROCESS;
		sys_scope = 0;
	}

	Pthread_attr_init(&attr);
	Pthread_attr_setscope(&attr, scope);

	sigfillset(&mask);
	sigdelset(&mask, SIGHUP);
	sigdelset(&mask, SIGINT);
	sigdelset(&mask, SIGSEGV);
	sigdelset(&mask, SIGBUS);
	sigdelset(&mask, SIGILL);
	pthread_sigmask(SIG_SETMASK, &mask, NULL);

	sigset(SIGHUP, sig_hndl);

	for (n = 0; n < 10; ++n) {
     		Pthread_create(&t, &attr, Thread, NULL);
		printf("created thread #%d = T@%d\n", n, t);

		if (n == thread_no)
			receiver = t;
	}
	printf("redirecting all signals to thread #%d = T@%d\n" , 
		thread_no,  receiver);

	/* SigWaitThread immer als PTHREAD_SCOPE_SYSTEM, immer aktiv */
	Pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
     	Pthread_create(&t, &attr, SigWaitThread, NULL);
	printf("SigWaitThread thread is T@%d\n", t);

	printf("PID %d\n", getpid());

	pthread_exit(NULL);
}

void *
Thread(void *arg)
{
	if (!sys_scope)
		printf("now in T@%d\n", pthread_self());

	for (;;) {
		pause();
		printf("T@%d, errno %d: %s\n", pthread_self(), 
			errno, strerror(errno));
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

		printf("SigWaitThread: sending signal %d as SIGHUP to T@%d\n", 
			sig, receiver);
		pthread_kill(receiver, SIGHUP);
	}
}

void
sig_hndl(int sig)
{
	char buf[100];

	sprintf(buf, "sig_hndl: signal %d in T@%d\n", sig, pthread_self());
	write(1, buf, strlen(buf));
}
