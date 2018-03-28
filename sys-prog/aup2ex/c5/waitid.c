/*
	waitid example
	AUP2, Sec. 5.08

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

#if defined(_XOPEN_UNIX)

void display_status(siginfo_t *infop);

/*[waitid-wait_and_display]*/
static bool wait_and_display(void)
{
	siginfo_t info;

#if defined(LINUX) && !defined(WEXITED)
#define WEXITED 0
#endif
	ec_neg1( waitid(P_ALL, 0, &info, WEXITED) )
	display_status(&info);
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}

void display_status(siginfo_t *infop)
{
	printf("Process %ld terminated:\n", (long)infop->si_pid);
	printf("\tcode =  %s\n",
	  get_macrostr("sigchld-code", infop->si_code, NULL));
	if (infop->si_code == CLD_EXITED)
		printf("\texit value =  %d\n", infop->si_status);
	else
		printf("\tsignal = %s\n",
		  get_macrostr("signal", infop->si_status, NULL));
}

/*[]*/
int main(void)
{
	pid_t pid;

	/* Case 1: Explicit call to _exit */
	if (fork() == 0) /* child */
		_exit(123);
	/* parent */
	ec_false( wait_and_display() )

	/* Case 2: Termination by kernel */
	if (fork() == 0) { /* child */
		int a, b = 0;

		a = 1 / b;
		_exit(EXIT_SUCCESS);
	}
	/* parent */
	ec_false( wait_and_display() )

	/* Case 3: External signal */
	if ((pid = fork()) == 0) { /* child */
		sleep(100);
		_exit(EXIT_SUCCESS);
	}
	/* parent */
	ec_neg1( kill(pid, SIGHUP) )
	ec_false( wait_and_display() )

	exit(EXIT_SUCCESS);

EC_CLEANUP_BGN
	exit(EXIT_FAILURE);
EC_CLEANUP_END
}

#else /* _XOPEN_UNIX */

int main(void)
{
	printf("waitid not implemented\n");
	exit(EXIT_FAILURE);
}

#endif /* _XOPEN_UNIX */
