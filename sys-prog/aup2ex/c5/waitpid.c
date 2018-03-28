/*
	waitpid example
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
#include "statuspr.h"

static bool wait_and_display(void);

/*[waitpid]*/
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

static bool wait_and_display(void)
{
	pid_t wpid;
	int status;

	ec_neg1( wpid = waitpid(-1, &status, 0) )
	display_status(wpid, status);
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}
/*[]*/
