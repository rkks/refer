/*
 * Zwei Threads, die sich alternierend gegenseitig aufwecken.
 */

#include <pthread.h>
#include <stdio.h>
#include "wrapper.h"

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t c  = PTHREAD_COND_INITIALIZER;

int turn = 0;

void *Thread(void *);

void *
Thread(void *v)
{
	int num = (int)v;
	int n;

	while( 1 )
	{
		Pthread_mutex_lock(&m);
		while (turn != num)
			Pthread_cond_wait(&c, &m);
		printf("%d> ", num);
 		while (getchar() != '\n')
			;
		turn = !turn;
		Pthread_mutex_unlock(&m);
		Pthread_cond_broadcast(&c);
	}
}

int
main(int argc, char **argv)
{
	int n;
	pthread_t t;
	pthread_attr_t attr;

	Pthread_attr_init(&attr);

	Pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

	for (n = 0; n < 2; ++n)
		Pthread_create(&t, &attr, Thread, (void *)n);
	
	pthread_exit(NULL);
}
