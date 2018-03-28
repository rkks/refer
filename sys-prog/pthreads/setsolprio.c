/*
 * Set Solaris scheduler parameters (class TS, IA) for individual threads.
 * For thread n (n = 1 - 5) the new values are set to argv[1] - n.
 *
 */

#define __EXTENSIONS__

#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <sched.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/priocntl.h>
#include <sys/rtpriocntl.h>
#include <sys/tspriocntl.h>
#include <sys/fsspriocntl.h>
#include <sys/fxpriocntl.h>
#include <sys/lwp.h>
#include "wrapper.h"

void *Thread(void *);
void doprio(int t, pri_t prio);

int gprio = -10;

int
main(int argc, char **argv)
{
	int n;
	pthread_t t;
	char b[80];

	if (argc > 1) {
		gprio = atoi(argv[1]);
	}
	printf("new value is %d\n", gprio);

	for (n = 2; n <= 5; ++n) {
		printf("creating thread %02d\n", n);
     		Pthread_create(&t, NULL, Thread, (void *)n);
	}

	doprio(1, gprio - 1);
	sleep(1);
	sprintf(b, "ps -cfL -p %d; priocntl -d %d", getpid(), getpid());
	system(b);
	pthread_exit(NULL);
}

char *
getschedclass(char *b)
{
	if (priocntl(P_LWPID, _lwp_self(), PC_GETXPARMS, NULL,
					    PC_KY_CLNAME, b, 0) < 0) {
		perror("priocntl in getschedclass");
		exit(1);
	}
	return b;
}

void *
Thread(void *arg)
{
	int num = (int)arg;
	doprio(num, gprio - num);
	
	sleep(2);
}

void 
doprio(int t, pri_t prio)
{
	pri_t upri, uprilim;
	char cbuf[PC_CLNMSZ], *class;

	/* get current values */
	class = getschedclass(cbuf);
	if (priocntl(P_LWPID, _lwp_self(), PC_GETXPARMS, 
		class, TS_KY_UPRILIM, &uprilim, TS_KY_UPRI, &upri, 0) < 0) {
		perror("priocntl");
		exit(1);
	}
	printf("old Thread %02d: class %s, ts_upri %d, ts_uprilim %d\n", 
		t, class, upri, uprilim);

	/* set new values */
	if (priocntl(P_LWPID, _lwp_self(), PC_SETXPARMS, 
	    class, TS_KY_UPRILIM, prio, TS_KY_UPRI, prio, 0) < 0) {
		perror("priocntl2");
		exit(1);
	}

	/* get new values */
	if (priocntl(P_LWPID, _lwp_self(), PC_GETXPARMS, 
		class, TS_KY_UPRILIM, &uprilim, TS_KY_UPRI, &upri, 0) < 0) {
		perror("priocntl3");
		exit(1);
	}
	printf("new Thread %02d: class %s, ts_upri %d, ts_uprilim %d\n", 
		t, class, upri, uprilim);
}
