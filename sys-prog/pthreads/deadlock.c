/*
 * Deadlock demo
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "wrapper.h"

int num_loops = 100000;
int cnt = 0;

pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t m2 = PTHREAD_MUTEX_INITIALIZER;

void *Thread1(void *);
void *Thread2(void *);

void *
Thread1(void *v)
{
     int n;

     for (n = 1; n <= num_loops; ++n) {
	Pthread_mutex_lock(&m1);
	Pthread_mutex_lock(&m2);

	++cnt;

	Pthread_mutex_unlock(&m2);
	Pthread_mutex_unlock(&m1);
     }

     return NULL;
}


void *
Thread2(void *v)
{
     int n;

     for (n = 1; n <= num_loops; ++n) {
	Pthread_mutex_lock(&m2);
	Pthread_mutex_lock(&m1);

	++cnt;

	Pthread_mutex_unlock(&m1);
	Pthread_mutex_unlock(&m2);
     }

     return NULL;
}

int
main(int argc, char **argv)
{
     int n;
     pthread_t t[2];
     pthread_attr_t attr;

     if (argc > 1)
	num_loops = atoi(argv[1]);
     printf("num_loops %d\n", num_loops);

     Pthread_attr_init(&attr);

     Pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

     Pthread_create(&t[0], &attr, Thread1, NULL);
     Pthread_create(&t[1], &attr, Thread2, NULL);

     for (n = 0; n < 2; ++n)
	Pthread_join(t[n], NULL);

     printf("cnt = %d, should be %d, diff %d\n", 
	cnt, num_loops * 2, num_loops * 2 - cnt);

     return 0;
}
