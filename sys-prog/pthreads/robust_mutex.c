/*
 * Robust mutex. 
 */

#include <pthread.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "wrapper.h"

pthread_mutex_t m;

void *Thread(void *);

void *
Thread1(void *v)
{
	printf("Thread1 start\n");
	Pthread_mutex_lock(&m);
	printf("Thread1 exiting\n");

	pthread_exit(NULL);
}

void *
Thread2(void *v)
{
	int e, n;

	printf("Thread %d start\n", (int) v);

	for (n = 0; n < 5; ++n) {
		e = pthread_mutex_lock(&m);
		if (e == 0)
			printf("Thread %d locked mutex\n", (int) v);
		else if (e == EOWNERDEAD) {
			printf("Thread %d got EOWNERDEAD\n", (int) v);
			Pthread_mutex_consistent_np(&m);
		}
		else {
			printf("pthread_mutex_lock error %d\n", e);
			exit(1);
		}
		printf("Thread %d locked mutex\n", (int) v);
		Pthread_mutex_unlock(&m);
	}
	
	printf("Thread %d exiting\n", (int) v);
	
	return NULL;
}

int
main(int argc, char **argv)
{
	int n, e;
	pthread_attr_t attr;
	pthread_t t;
	pthread_mutexattr_t mattr;

	Pthread_mutexattr_init(&mattr);

	e = pthread_mutexattr_setprotocol(&mattr, PTHREAD_PRIO_INHERIT);
	if (e) {
		printf("pthread_mutexattr_setprotocol error %d: %s\n", 
			e, strerror(e));
		exit(1);
	}
	e = pthread_mutexattr_setrobust_np(&mattr, PTHREAD_MUTEX_ROBUST_NP);
	if (e) {
		printf("pthread_mutexattr_setrobust_np error %d: %s\n", 
			e, strerror(e));
		exit(1);
	}
	e = pthread_mutex_init(&m, &mattr);
	if (e) {
		printf("pthread_mutex_init error %d: %s\n", e, strerror(e));
		exit(1);
	}

	Pthread_attr_init(&attr);

	Pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

     	Pthread_create(&t, &attr, Thread1, (void *)1);
	Pthread_join(t, NULL);

     	Pthread_create(&t, &attr, Thread2, (void *)2);
     	Pthread_create(&t, &attr, Thread2, (void *)3);

	pthread_exit(NULL);

	return 0;
}
