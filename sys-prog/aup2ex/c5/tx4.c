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

/*[tx4]*/
static long get_and_incr_x(long incr)
{
	static long x = 0;
	static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
	long rtn;

	ec_rv( pthread_mutex_lock(&mtx) )
	rtn = x += incr;
	ec_rv( pthread_mutex_unlock(&mtx) )
	return rtn;

EC_CLEANUP_BGN
	exit(EXIT_FAILURE);
EC_CLEANUP_END
}

static void *thread_func(void *arg)
{
	while (get_and_incr_x(0) < (long)arg) {
		printf("Thread 2 says %ld\n", get_and_incr_x(1));
		sleep(1);
	}
	return (void *)get_and_incr_x(0);
}

int main(void)
{
	pthread_t tid;
	void *status;

	assert(sizeof(long) <= sizeof(void *));
	ec_rv( pthread_create(&tid, NULL, thread_func, (void *)6) )
	while (get_and_incr_x(0) < 10) {
		printf("Thread 1 says %ld\n", get_and_incr_x(1));
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
