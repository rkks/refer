/* 
 * Umgang mit conditions: main Thread erzeugt einen Thread und wartet bis 
 * dieser laeuft und die globale variable init gesetzt hat.
 */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include "wrapper.h"

pthread_mutex_t init_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t init_cond  = PTHREAD_COND_INITIALIZER;
int init = 0;

void *Thread(void *);

int
main(int argc, char **argv)
{
	pthread_t t;

     	Pthread_create(&t, NULL, Thread, NULL);

	Pthread_mutex_lock(&init_mutex);
	while (!init)
		Pthread_cond_wait(&init_cond, &init_mutex);
	Pthread_mutex_unlock(&init_mutex);

	printf("main: init %d\n", init);
}

void *
Thread(void *arg)
{
	printf("Thread: sleeping ...\n");
	sleep(4);

	Pthread_mutex_lock(&init_mutex);
	init = 1;
	Pthread_mutex_unlock(&init_mutex);

	printf("Thread: initialized, waking up main\n");

	Pthread_cond_broadcast(&init_cond);
}
