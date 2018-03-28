/*
 * Mutexes are granted to waiting threads in priority order (0 - 127).
 * The program creates a numer of threads with random priorities to show this.
 */

#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <sched.h>
#include <stdlib.h>
#include <unistd.h>
#include "wrapper.h"

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

void *Thread(void *);

int
main(int argc, char **argv)
{
	int n, scope;
	pthread_t t;
	pthread_attr_t attr;
	struct sched_param param;
	unsigned seed = 1;

	Pthread_attr_init(&attr);
	if (argc == 1) {
		printf("PTHREAD_SCOPE_PROCESS\n");
		Pthread_attr_setscope(&attr, PTHREAD_SCOPE_PROCESS);
	}
	else {
		printf("PTHREAD_SCOPE_SYSTEM\n");
		Pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
	}

	Pthread_attr_getschedparam(&attr, &param);
	printf("sched_priority %d\n", param.sched_priority);

	Pthread_mutex_lock(&m);

	for (n = 0; n < 20; ++n) {
		param.sched_priority = rand_r(&seed) % 128;

		printf("creating thread %02d prio %02d\n", 
			n, param.sched_priority);
		Pthread_attr_setschedparam(&attr, &param);
     		Pthread_create(&t, &attr, Thread, (void *)n);
	}

	sleep(1);
	Pthread_mutex_unlock(&m);

	pthread_exit(NULL);
}

void *
Thread(void *arg)
{
	int num = (int)arg;
	struct sched_param param;
	int policy;


	Pthread_mutex_lock(&m);

	Pthread_getschedparam(pthread_self(), &policy, &param);
	printf("Thread %02d got mutex: prio %02d\n", 
		num, param.sched_priority);

	Pthread_mutex_unlock(&m);
}
