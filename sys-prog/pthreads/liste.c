/*
 * Einfuegen in lineare Liste mit Mutex. Es gibt 2 Einfuegeprozesse und
 * einen Leseprozess.
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "wrapper.h"

#define N 10000

struct node {
	int val;
	struct node *next;
};

struct node *first = NULL;

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

void *InsThread(void *);
void *SumThread(void *);

void *
InsThread(void *v)
{
     int n;
	struct node *p;

	for (n = 1; n <= N; ++n) {
		p = malloc(sizeof(struct node));
		if (!p) {
			printf("malloc failed\n");
			exit(1);
		}

		Pthread_mutex_lock(&m);
		if(first)
		 p->val = first->val + 1;
		else
			p->val = 1;
		p->next = first;
		first = p;
		Pthread_mutex_unlock(&m);
	}

     sleep(1);
     exit(0);
}

void *
SumThread(void *v)
{
     int n, sum, asum = -1;
     struct node *p;

     for (;;) {
	sum = 0;
	Pthread_mutex_lock(&m);
	for (p = first, n = 0; p; p = p->next, ++n)
		sum += p->val;
	Pthread_mutex_unlock(&m);
	if (sum != asum) {
		printf("n %d, sum %d\n", n, sum);
		if( sum != n * (n + 1) / 2) {
			printf("ERROR\n");
			exit(1);
		}
		asum = sum;
	}
     }
}

int
main(int argc, char **argv)
{
	int n;
	pthread_t t, t1, t2;
	pthread_attr_t attr;

	Pthread_attr_init(&attr);

	Pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

	Pthread_create(&t, &attr, SumThread, NULL);
	Pthread_create(&t1, &attr, InsThread, NULL);
	Pthread_create(&t2, &attr, InsThread, NULL);

	Pthread_join(t1, NULL);
	Pthread_join(t2, NULL);
	return 0;
}
