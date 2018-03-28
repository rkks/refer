/*
	Thread example (mutex)
	AUP2, Sec. 5.17.3

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

/*[tx3]*/
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static long x = 0;

static void *thread_func(void *arg)
{
	bool done;

	while (true) {
		ec_rv( pthread_mutex_lock(&mtx) )
		done = x >= (long)arg;
		ec_rv( pthread_mutex_unlock(&mtx) )
		if (done)
			break;
		ec_rv( pthread_mutex_lock(&mtx) )
		printf("Thread 2 says %ld\n", ++x);
		ec_rv( pthread_mutex_unlock(&mtx) )
		sleep(1);
	}
	return (void *)x;

EC_CLEANUP_BGN
	(void)pthread_mutex_unlock(&mtx);
	EC_FLUSH("thread_func")
	return NULL;
EC_CLEANUP_END
}

int main(void)
{
	pthread_t tid;
	void *status;
	bool done;

	assert(sizeof(long) <= sizeof(void *));
	ec_rv( pthread_create(&tid, NULL, thread_func, (void *)6) )
	while (true) {
		ec_rv( pthread_mutex_lock(&mtx) )
		done = x >= 10;
		ec_rv( pthread_mutex_unlock(&mtx) )
		if (done)
			break;
		ec_rv( pthread_mutex_lock(&mtx) )
		printf("Thread 1 says %ld\n", ++x);
		ec_rv( pthread_mutex_unlock(&mtx) )
		sleep(2);
	}
	ec_rv( pthread_join(tid, &status) )
	printf("Thread 2's exit status is %ld\n", (long)status);
	return EXIT_SUCCESS;

EC_CLEANUP_BGN
	return EXIT_FAILURE;
EC_CLEANUP_END
}
/*[]*/
