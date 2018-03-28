/*
	Introductory signal example (catching)
	AUP2, Sec. 9.01.1

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
/*[pgm]*/
static void fcn(int signum)
{
	(void)write(STDOUT_FILENO, "Got signal\n", 11);
	_exit(EXIT_FAILURE);
}

int main(void)
{
	int i;
	struct sigaction act;

	memset(&act, 0, sizeof(act));
	act.sa_handler = fcn;
	ec_neg1( sigaction(SIGINT, &act, NULL) )

	for (i = 1; ; i++) {
		sleep(3);
		printf("%d\n", i);
	}
	exit(EXIT_SUCCESS);

EC_CLEANUP_BGN
	exit(EXIT_FAILURE);
EC_CLEANUP_END
}
/*[]*/
