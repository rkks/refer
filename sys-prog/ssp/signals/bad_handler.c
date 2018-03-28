#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <pwd.h>
#include <sys/time.h>
#include "ssp.h"

static void sigalrm (int sig);

int main (void)
{
	struct passwd *buf;
	struct itimerval interval;

	sigset (SIGALRM, sigalrm);
	interval.it_interval.tv_sec = 0;
	interval.it_interval.tv_usec = 1000;
	interval.it_value.tv_sec = 0;
	interval.it_value.tv_usec = 1000;
	setitimer (ITIMER_REAL, &interval, NULL);

	for (;;) {
		if ((buf = getpwnam ("rich")) == NULL)
			err_msg ("getpwnam failed");
		if (strcmp ("rich", buf -> pw_name) != 0)
			printf ("Buffer corrupted, pw_name = %s\n", buf -> pw_name);
	}
}

static void sigalrm (int sig)
{
	struct passwd *buf;

	printf ("In signal handler\n");
	if ((buf = getpwnam ("root")) == NULL)
		err_msg ("getpwnam (root) failed");
}
