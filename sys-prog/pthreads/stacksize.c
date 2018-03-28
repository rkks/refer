/*
 * Setzen der Stack size.
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "wrapper.h"

int cnt = 50;

void *Thread(void *);
void fun(int id, int n);

void *
Thread(void *v)
{
	fun((int)v, 1);
	printf("thread %d finished\n", (int)v);
	return NULL;
}

void 
fun(int id, int n)
{
	char v[1024];

	if (n % 10 == 0)
		printf("Thread %d: n = %d, v = %p\n", id, n, &v);
	if (++n <= cnt)
		fun(id, n);
}

int
main(int argc, char **argv)
{
	int n, num = 4;
	size_t sz, newsz = 1024*100;
	pthread_t t;
	pthread_attr_t attr;

	if (argc > 1)
		newsz = atoi(argv[1]) * 1024;
	if (argc > 2)
		num = atoi(argv[2]);
	if (argc > 3)
		cnt = atoi(argv[3]);

	Pthread_attr_init(&attr);
	if (newsz)
		Pthread_attr_setstacksize(&attr, newsz);
	Pthread_attr_getstacksize(&attr, &sz);
	printf("stack size %d K\n", sz / 1024);

	printf("creating %d threads\n", num);
	for (n = 1; n <= num; ++n) {
		if (pthread_create(&t, &attr, Thread, (void *)n)) {
			printf("error creating thread %d\n", n);
			exit(1);
		}
		if (n % 1000 == 0)
			printf("%d threads created\n", n);
	}
	printf("threads created\n");

	pthread_exit(NULL);

	return 0;
}
