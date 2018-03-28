/*
 * Asynchronous cancellation eines rechenintensiven Threads.
 */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include "wrapper.h"

void *Thread(void *);

void *
Thread(void *v)
{
	int n;


	printf("Thread started\n");
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

	for (n = 0; ;++n)
		;
}

int
main(int argc, char **argv)
{
	int n;
	pthread_attr_t attr;
	pthread_t tid;
	void *res;

	Pthread_attr_init(&attr);

	Pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

     	Pthread_create(&tid, &attr, Thread, NULL);

	sleep(4);
	printf("canceling Thread\n");
	Pthread_cancel(tid);

	Pthread_join(tid, &res);
	if (res == PTHREAD_CANCELED)
		printf("cancelled\n");
	else
		printf("exited normally\n");

	return 0;
}
