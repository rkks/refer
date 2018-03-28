/* 
 * Barrier
 */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "wrapper.h"

struct mybarrier {
	pthread_mutex_t m;
	pthread_cond_t c;
	int members;
	int cnt;
	int turn;
};

struct mybarrier b;
#define NUM 10

void *Thread(void *);
void barrier_init(struct mybarrier *b, int num);
int barrier(struct mybarrier *b);

void barrier_init(struct mybarrier *b, int num)
{
	Pthread_mutex_init(&b->m, NULL);
	Pthread_cond_init(&b->c, NULL);
	b->members = num;
	b->cnt = b->turn= 0;
}

int barrier(struct mybarrier *b)
{
	int t;
	int last = 0;

	Pthread_mutex_lock(&b->m);;

	if (++b->cnt == b->members) {
		last = 1;
		b->turn = !b->turn;
		b->cnt = 0;
		Pthread_cond_broadcast(&b->c);
	}
	else {
		t = b->turn;
		do
			Pthread_cond_wait(&b->c, &b->m);
		while (b->turn == t);
	}

	Pthread_mutex_unlock(&b->m);

	return last;
}

int
main(int argc, char **argv)
{
	int n;
	pthread_t t;
	pthread_attr_t attr;

	Pthread_attr_init(&attr);
	Pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

	barrier_init(&b, NUM);
	for (n = 1; n <= NUM; ++n)
     		Pthread_create(&t, &attr, Thread, (void *)n);

	pthread_exit(NULL);
}

void *
Thread(void *arg)
{
	int num = (int)arg;
	int n;
	unsigned seed = num;
	int tim;

	for (n = 1; n <= 5; ++n) {
		tim = rand_r(&seed);
		tim = tim % 4;
		printf("-- thread %2d: sleeping %d\n", num, tim);
		if (tim)
			sleep(tim);
		if (barrier(&b))
			printf("thread %2d: alle angekommen bei barriere %d\n", 
				num, n);
	}
}
