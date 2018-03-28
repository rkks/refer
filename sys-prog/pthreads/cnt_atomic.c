/*
 * Fehlerhaftes Programm: Globaler counter cnt wird von zwei Threads N 
 * mal erhoeht ohne dass der Zugriff synchronisiert wird.
 */

#include <pthread.h>
#include <stdio.h>
#include <atomic.h>
#include "wrapper.h"

#define N 100000000
uint32_t cnt = 0;

void *Thread(void *);

void *
Thread(void *v)
{
     int n;

     for (n = 1; n <= N; ++n)
	atomic_add_32(&cnt, 1);

     return NULL;
}

int
main(int argc, char **argv)
{
     int n;
     pthread_t t[2];
     pthread_attr_t attr;

     Pthread_attr_init(&attr);

     Pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

     for (n = 0; n < 2; ++n)
     	Pthread_create(&t[n], &attr, Thread, NULL);

     for (n = 0; n < 2; ++n)
	Pthread_join(t[n], NULL);

     printf("cnt = %d, should be %d, diff %d\n", cnt, N * 2, N * 2 - cnt);

     return 0;
}
