/*
	Thread example (condition variable)
	AUP2, Sec. 5.17.4

	Copyright 2003 by Marc J. Rochkind. All rights reserved.
	May be copied only for purposes and under conditions described
	on the Web page www.basepath.com/aup/copyright.htm.

	The Example Files are provided "as is," without any warranty;
	without even the implied warranty of merchantability or fitness
	for a particular purpose. The author and his publisher are not
	responsible for any damages, direct or incidental, resulting
	from the use or non-use of these Example Files.

	The Example Files may contain defects, and some contain deliberate
	coding mistakes that were included for educational reasons.
	You are responsible for determining if and how the Example Files
	are to be used.

*/
#include "defs.h"
#include <pthread.h>

/*[tx5]*/
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

struct node {
	int n_number;
	struct node *n_next;
} *head = NULL;

static void *thread_func(void *arg)
{
	struct node *p;

	while (true) {
		ec_rv( pthread_mutex_lock(&mtx) )
		while (head == NULL)
			ec_rv( pthread_cond_wait(&cond, &mtx) )
		p = head;
		head = head->n_next;
		printf("Got %d from front of queue\n", p->n_number);
		free(p);
		ec_rv( pthread_mutex_unlock(&mtx) )
	}
	return (void *)true;

EC_CLEANUP_BGN
	(void)pthread_mutex_unlock(&mtx);
	EC_FLUSH("thread_func")
	return (void *)false;
EC_CLEANUP_END
}

int main(void)
{
	pthread_t tid;
	int i;
	struct node *p;

	ec_rv( pthread_create(&tid, NULL, thread_func, NULL) )
	for (i = 0; i < 10; i++) {
		ec_null( p = malloc(sizeof(struct node)) )
		p->n_number = i;
		ec_rv( pthread_mutex_lock(&mtx) )
		p->n_next = head;
		head = p;
		ec_rv( pthread_cond_signal(&cond) )
		ec_rv( pthread_mutex_unlock(&mtx) )
		sleep(1);
	}
	ec_rv( pthread_join(tid, NULL) )
	printf("All done -- exiting\n");
	return EXIT_SUCCESS;

EC_CLEANUP_BGN
	return EXIT_FAILURE;
EC_CLEANUP_END
}
/*[]*/
