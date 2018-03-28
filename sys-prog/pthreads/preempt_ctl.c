/*
 * Demonstriert Gebrauch von Solaris LWP preemption control.
 *
 * usage: preempt_ctl preempt num_threads nloops crit_fact
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <schedctl.h>
#include "wrapper.h"

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

int preempt = 0;
int num_threads = 8;
int nloops = 50000;
double crit_fact = 1.0;
int crit_loops;
int noncrit_loops;
int last_in_crit = 0;
int real_switches = 0;
int yield_cnt= 0;

#define N 1000
#define MAXTHREADS 1000
pthread_t tids[MAXTHREADS + 1];

void *Thread(void *);

void *
Thread(void *v)
{
	int num = (int)v;
	int n, k, e = 0;
	schedctl_t *schedctl;

	schedctl = schedctl_init();
	if (!schedctl) {
		printf("thread %d: schedctl_init failed\n", num);
		exit(1);
	}

	for (n = 0; n < nloops; ++n)
		{
		if (!preempt)
			{
			/*schedctl_start(schedctl);*/
			schedctl->sc_nopreempt = 1;
			}
		Pthread_mutex_lock(&m);

		if (last_in_crit != num) {
			++real_switches;
			last_in_crit = num;
		}

		for (k = 0; k < crit_loops; ++k)
			e += k;

		Pthread_mutex_unlock(&m);
		if (!preempt)
			{
			/*schedctl_stop(schedctl);*/
			if (schedctl->sc_yield)
				++yield_cnt;
			}

		for (k = 0; k < noncrit_loops; ++k)
			e += k;
		}

 	return NULL;
}

int
main(int argc, char **argv)
{
	int n;
	pthread_attr_t attr;

	if (argc > 1)
		preempt = atoi(argv[1]);
	if (argc > 2)
		num_threads = atoi(argv[2]);
	if (num_threads > MAXTHREADS)
		num_threads = MAXTHREADS;
	if (argc > 3)
		nloops = atoi(argv[3]);
	if (argc > 4)
		crit_fact = atof(argv[4]);

	crit_loops = N;
	noncrit_loops = N * crit_fact;

	printf("preempt %d num_threads %d nloops %d crit_fact %.2f\n", 
		preempt, num_threads, nloops, crit_fact);
	printf("crit_loops %d noncrit_loops %d\n", crit_loops, noncrit_loops);

	Pthread_attr_init(&attr);

	Pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

	for (n = 1; n <= num_threads; ++n)
		Pthread_create(&tids[n], &attr, Thread, (void *)n);
	
	for (n = 1; n <= num_threads; ++n)
		Pthread_join(tids[n], NULL);
	
	printf("real_switches %d, total critical sections %d, factor %.2f\n",
		real_switches, num_threads * nloops, 
		(double)(num_threads * nloops) / (double)real_switches);
}
