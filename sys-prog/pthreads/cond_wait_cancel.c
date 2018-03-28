#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include "wrapper.h"

void *Thread(void *);
void cleanup(void *p);

pthread_cond_t c = PTHREAD_COND_INITIALIZER;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
int condition = 1;

int
main(int argc, char **argv)
{
        int n;
        pthread_t t;
        pthread_attr_t attr;

        Pthread_attr_init(&attr);
        Pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

        Pthread_create(&t, &attr, Thread, NULL);

        sleep(1);
	pthread_cancel(t);
        sleep(1);

        printf("main: entering critical section\n");
        Pthread_mutex_lock(&m);
        Pthread_mutex_unlock(&m);
        printf("main: left critical section\n");
}

void 
cleanup(void *p)
{
	Pthread_mutex_unlock(&m);
	printf("cleanup called\n");
}

void *
Thread(void *arg)
{
	pthread_cleanup_push(cleanup, NULL);
	pthread_mutex_lock(&m);

	while (condition)
		pthread_cond_wait(&c, &m);

	pthread_cleanup_pop(1);
}
