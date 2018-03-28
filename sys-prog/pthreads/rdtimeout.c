/*
 * Lesen vom Terminal mit timeout aus einem Thread.
 */

#define __EXTENSIONS__ /* fuer prototype fuer sigset() */

#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "wrapper.h"

pthread_t rtid;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cn = PTHREAD_COND_INITIALIZER;

void *Thread(void *);
void *ReadThread(void *);
void *TimeoutThread(void *v);

void *
Thread(void *v)
{
	sleep(10);
	printf("Thread exit\n");

	return v;
}

void *
ReadThread(void *v)
{
	int e;
	char c;
	pthread_attr_t attr;
	pthread_t t;

	printf("bitte CR eingeben> ");
	fflush(stdout);

	Pthread_attr_init(&attr);
	Pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
     	Pthread_create(&t, &attr, TimeoutThread, NULL);

	e = read(1, &c, 1);
	if (e == -1 && errno == EINTR)
		printf("\n ... timeout\n");
	else if (e < 0) {
		printf("unexpected error e = %d, errno %d\n", e, errno);
		exit(1);
	}
	else if (e == 1)
		printf("gelesen: 0x%02x\n", c & 0xff);
	pthread_cond_signal(&cn);
	Pthread_join(t, NULL);

	printf("ReadThread exit\n");

	return v;
}

void *
TimeoutThread(void *v)
{
	struct timespec timespec;

	timespec.tv_sec = time(NULL) + 5;
	timespec.tv_nsec = 0;
	pthread_cond_timedwait(&cn, &m, &timespec);
	pthread_kill(rtid, SIGTERM);

	return v;
}

void 
hndl(int sig)
{
}


int
main(int argc, char **argv)
{
	int e;
	pthread_t t;
	pthread_attr_t attr;

	sigset(SIGTERM, hndl);

	Pthread_attr_init(&attr);
	Pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

     	Pthread_create(&t, &attr, Thread, NULL);
     	Pthread_create(&t, &attr, Thread, NULL);
     	Pthread_create(&rtid, &attr, ReadThread, NULL);
     	Pthread_create(&t, &attr, Thread, NULL);
     	Pthread_create(&t, &attr, Thread, NULL);

	pthread_exit(NULL);
}
