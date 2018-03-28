/*
 * Parallelisierte Version von random_add_ser.c
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "wrapper.h"

#define N 10000000
#define NSUMS 4

unsigned results[NSUMS];

void *Thread(void *);

void *
Thread(void *v)
{
	int threadnum = (int)v;
	unsigned seed = threadnum;
	int n;
	unsigned sum = 0;

	for (n = 0; n < N; ++n)
		sum += rand_r(&seed);

	results[threadnum] = sum;

	printf("thread  %d finished\n", threadnum);
	return NULL;
}

int
main(int argc, char **argv)
{
	int n, e;
	pthread_t t[NSUMS];
	pthread_attr_t attr;
	unsigned sum = 0;

	Pthread_attr_init(&attr);

	Pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

	for (n = 0; n < NSUMS; ++n)
     		Pthread_create(&t[n], &attr, Thread, (void *)n);

	for (n = 0; n < NSUMS; ++n) {
		Pthread_join(t[n], NULL);

		sum += results[n];
	}

	printf("sum %u\n", sum);

	return 0;
}
