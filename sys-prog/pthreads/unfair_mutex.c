/*
 * Mutexe sind nicht gerecht. Es ist nicht sicher, dass der, der am laengsten
 * gewartet hat, das Mutex als naechster bekommt. Ein Mutex iat "halbwegs" 
 * gerecht, wenn es die meiste Zeit frei ist. Anderenfalls koennen Threads
 * evtl. vollkommen ausgesperrt werden.
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "wrapper.h"

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

int nloops = 0;

void *Thread(void *);

void *
Thread(void *v)
{
	int num = (int)v;
	int n;

	while( 1 )
	{
		Pthread_mutex_lock(&m);
		printf("%d> ", num);
 		while (getchar() != '\n')
			;
		Pthread_mutex_unlock(&m);

		for (n = 0; n < nloops; ++n)
			;
	}

 	/* not reached */
}

int
main(int argc, char **argv)
{
	int n;
	pthread_t t;
	pthread_attr_t attr;

	if (argc > 1)
		nloops = atoi(argv[1]);
	printf("nloops %d\n", nloops);

	Pthread_attr_init(&attr);

	Pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

	for (n = 0; n < 2; ++n)
		Pthread_create(&t, &attr, Thread, (void *)n);
	
	pthread_exit(NULL);
}
