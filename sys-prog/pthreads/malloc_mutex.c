/*
 * Dynamisch mit malloc allociertes Mutex.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "wrapper.h"

struct dyn_mutex {
	int something_to_protect;
	pthread_mutex_t m;
};

int
main()
{
	struct dyn_mutex *p;

	p = malloc(sizeof(struct dyn_mutex));
	if (!p) {
		printf("no more memory\n");
		exit(1);
	}

	Pthread_mutex_init(&p->m, NULL);
	Pthread_mutex_lock(&p->m);
	/* ... */
	Pthread_mutex_unlock(&p->m);

	return 0;
}
