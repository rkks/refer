/* 
 * Umgang mit conditions
 */

#include <pthread.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <poll.h>
#include "wrapper.h"

pthread_mutex_t pause_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  pause_cond  = PTHREAD_COND_INITIALIZER;
int pause_flag = 0;

void *Thread(void *);
char *threadname();

int
main(int argc, char **argv)
{
	int n, e, scope;
	pthread_t t;
	pthread_attr_t attr;

	Pthread_attr_init(&attr);
	Pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

	for (n = 0; n < 4; ++n)
     		Pthread_create(&t, &attr, Thread, (void *)n);

	for (;;) {
		sleep(5);

		Pthread_mutex_lock(&pause_mutex);
		pause_flag = 1;
		Pthread_mutex_unlock(&pause_mutex);
		printf("Pause!!\n");

		sleep(5);

		printf("Weiter!!\n");
		Pthread_mutex_lock(&pause_mutex);
		pause_flag = 0;
		Pthread_cond_broadcast(&pause_cond);
		Pthread_mutex_unlock(&pause_mutex);

	}
}

void *
Thread(void *arg)
{
	int num = (int)arg;

	for (;;) {
		Pthread_mutex_lock(&pause_mutex);
		while (pause_flag)
			 Pthread_cond_wait(&pause_cond, &pause_mutex);
		Pthread_mutex_unlock(&pause_mutex);

		printf("ich bin thread %d\n", num);
		poll(0, NULL, 100); /* schlaeft 100 Millisekunden */
	}
}
