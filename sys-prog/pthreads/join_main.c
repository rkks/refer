/* 
 * Erzeugt einen thread, der auf den main thread wartet.
 */

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include "wrapper.h"

void *Thread(void *);

pthread_t main_tid;

int
main(int argc, char **argv)
{
	pthread_t t;
	pthread_attr_t attr;

	main_tid = pthread_self();

	Pthread_attr_init(&attr);

	Pthread_create(&t, &attr, Thread, NULL);
	sleep(4);

	printf("main is exiting\n");
	pthread_exit(NULL);
}

void *
Thread(void *v)
{
	printf("Thread started, waiting for main thread\n");
	Pthread_join(main_tid, NULL);
	printf("joined main thread\n");
}
