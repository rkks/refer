/*
 * Wenn ein externes SIGTERM Signal eintrifft, gibt es einen ""spurious wakeup"
 */

#define _POSIX_C_SOURCE 199506L

#define __EXTENSIONS__ /* fuer prototype fuer sigset() */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include "wrapper.h"

void
h(int sig)
{
printf("catch\n");
}


int
main()
{
	int e;
	pthread_cond_t c = PTHREAD_COND_INITIALIZER;
	pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

	sigset(SIGTERM, h);
	Pthread_mutex_lock(&m);
	e = pthread_cond_wait(&c, &m);
	printf("pthread_cond_wait e = %d\n", e);

	return 0;
}
