/* 
 * Thread Signal Test
 */

#define __EXTENSIONS__ /* fuer prototype fuer sigset() */

#include <pthread.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <strings.h>
#include "wrapper.h"

void *Thread(void *);
void sig_hndl(int sig);

int sys_scope;

int
main(int argc, char **argv)
{
	int n;
	pthread_t t;
	pthread_attr_t attr;
	int scope;

	if (argc > 1) {
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

	for (n = 1; n < 32; ++n) {
		if (n == SIGINT)
			printf("skipping SIGINT\n");
		else if (sigset(n, sig_hndl))
			printf("cannot handle signal %d\n", n);
	}

	for (n = 0; n < 10; ++n) {
     		Pthread_create(&t, &attr, Thread, NULL);
		printf("created thread T@%d\n", t);
	}

	printf("PID %d\n", getpid());

	pthread_exit(NULL);

}

void *
Thread(void *arg)
{
	int n;

	if (!sys_scope)
		printf("now in T@%d\n", pthread_self());

	for (;;) {
		for (n = 0; n < 100000000; ++n)
			;
		sched_yield();

		if (!sys_scope)
			printf("now in T@%d\n", pthread_self());
	}
}

void
sig_hndl(int sig)
{
	char buf[100];

	sprintf(buf, "sig_hndl: signal %d in T@%d\n", sig, pthread_self());
	write(1, buf, strlen(buf));
}
