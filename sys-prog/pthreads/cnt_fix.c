/*
 * Berichtigte Version von cnt.c. Benutzt ein Mutex um Variable cnt
 * zu schuetzen.
 */

#include <pthread.h>
#include <stdio.h>
#include "wrapper.h"

#define N 100000000
int cnt = 0;

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

void *Thread(void *);

void *
Thread(void *v)
{
     int n;

     for (n = 1; n <= N; ++n) {
	Pthread_mutex_lock(&m);

	++cnt;

	Pthread_mutex_unlock(&m);
     }

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
