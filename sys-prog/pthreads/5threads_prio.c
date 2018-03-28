/*
 * Aufgabe: Erzeugen von 5 rechenintensive SCHED_FIFO Threads.
 *          Muss als root gestartet werden. Dann bekommt man class RT
 *	    threads mit unterschiedlichen Prioritaeten (1 - 6), die die
 *	    Reihenfolge des Ablaufs steuern. Legt alles andere so lange lahm!!!
 */

#include <pthread.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "wrapper.h"

#define NTHREADS 5

struct thread_data {
	int my_id;
	pthread_t tid;
};

struct thread_data thread_data[NTHREADS];

void *Thread(void *);
void Error(const char *str, ...);
int get_prio();

void *
Thread(void *v)
{
	struct thread_data *p = (struct thread_data *)v;
	int n;

	printf("Start thread %d, prio %d\n", p->my_id, get_prio());
	for (n = 0; n < 500000000; ++n)
		;
	printf("   Ende thread %d\n", p->my_id);

	return v;
}

int
main(int argc, char **argv)
{
	int n, e;
	pthread_attr_t attr;
	struct thread_data *res;
	struct sched_param param;

	Pthread_attr_init(&attr);

	Pthread_attr_getschedparam(&attr, &param);
	param.sched_priority = 0;
	e = pthread_setschedparam(pthread_self(), SCHED_FIFO, &param);
	if (e) {
		printf("pthread_setschedparam: error %d, %s\n",
			e, strerror(e));
		exit(1);
	}
	printf("main thread, prio %d\n", get_prio());

	for (n = 0; n < NTHREADS; ++n) {
		thread_data[n].my_id = n;

		param.sched_priority = n + 1;
		Pthread_attr_setschedparam(&attr, &param);

     		Pthread_create(&thread_data[n].tid, &attr, Thread, 
						(void *)&thread_data[n]);
	}

	for (n = NTHREADS - 1; n >= 0; --n) {
		Pthread_join(thread_data[n].tid, (void **)&res);
		printf("   Das war thread %d\n", res->my_id);
	}

	return 0;
}

int 
get_prio()
{
	struct sched_param param;
	int policy;

	Pthread_getschedparam(pthread_self(), &policy, &param);
	return param.sched_priority;
}
