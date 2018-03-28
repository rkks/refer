/*
 * Thread specific data.
 */

#include <pthread.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include "wrapper.h"

void f();
void *Thread(void *);
void destruct(void *p);

struct thread_data {
	char name[20];
};

pthread_key_t key;

void destruct(void *p)
{
	printf("destructing %s\n", ((struct thread_data *)p)->name);
	free(p);
}

#define N 4

void *
Thread(void *v)
{
	Pthread_setspecific(key, v);
	f();
	return NULL;
}

void f()
{
	struct thread_data *p;

	p = pthread_getspecific(key);
	printf("ich bin thread %s\n", p->name);

	sleep(4);
}

int
main(int argc, char **argv)
{
	int n;
	pthread_t t[N];
	struct thread_data *p;
	char b[20];

	Pthread_key_create(&key, destruct);

	for (n = 0; n < N; ++n) {
		p = malloc(sizeof(struct thread_data));
		sprintf(b, "Hugo%d", n);
		strcpy(p->name, b);
     		Pthread_create(t + n, NULL, Thread, (void *)p);
	}

	for (n = 0; n < N; ++n) 
		Pthread_join(t[n], NULL);

	return 0;
}
