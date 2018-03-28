/*
	Thread example (pthread_join)
	AUP2, Sec. 5.17.2

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

/*[tx2]*/
static long x = 0;

static void *thread_func(void *arg)
{
	while (x < (long)arg) {
		printf("Thread 2 says %ld\n", ++x);
		sleep(1);
	}
	return (void *)x;
}

int main(void)
{
	pthread_t tid;
	void *status;

	assert(sizeof(long) <= sizeof(void *));
	ec_rv( pthread_create(&tid, NULL, thread_func, (void *)6) )
	while (x < 10) {
		printf("Thread 1 says %ld\n", ++x);
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
