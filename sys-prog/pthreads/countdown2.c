/* 
 * Variante von countdown.c mit 2 conditions
 */

#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "wrapper.h"

pthread_mutex_t cm = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t zero  = PTHREAD_COND_INITIALIZER;
pthread_cond_t abgeholt  = PTHREAD_COND_INITIALIZER;
#define MAX 1000000
int cnt = MAX;
int who = -1;
time_t when;

void *Thread(void *);
char *threadname();

int
main(int argc, char **argv)
{
	int n, e, scope;
	pthread_t t;
	pthread_attr_t attr;
	time_t tim;
	int w;
	char buf[100];

	Pthread_attr_init(&attr);
	Pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

	for (n = 0; n < 7; ++n)
     		Pthread_create(&t, &attr, Thread, (void *)n);

	for (;;) {
		Pthread_mutex_lock(&cm);
		while (who == -1)
			 Pthread_cond_wait(&zero, &cm);
		w = who;
		tim = when;
		who = -1;
		Pthread_cond_broadcast(&abgeholt);
		Pthread_mutex_unlock(&cm);

		printf("thread %d reached zero at %s", 
			w, ctime(&tim), buf, sizeof(buf));

		sleep(10);
	}
}

void *
Thread(void *arg)
{
	int num = (int)arg;
	int w;

	for (;;) {
		Pthread_mutex_lock(&cm);
		if (--cnt == 0) {
			while (who != -1) {
				printf("noch nicht abgeholt\n");
				Pthread_cond_wait(&abgeholt, &cm);
			}
			w = who;
			who = num;
			when = time(NULL);
			cnt = MAX;
			Pthread_cond_broadcast(&zero);
		}
		Pthread_mutex_unlock(&cm);

		sched_yield();
	}
}
