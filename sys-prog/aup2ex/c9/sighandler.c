/*
	Signal-handler example
	AUP2, Sec. 9.01.7

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

static volatile sig_atomic_t gotsig = -1;

static void handler(int signum)
{
	gotsig = signum;
}

int main(void)
{
	struct sigaction act;
	time_t start, stop;

	memset(&act, 0, sizeof(act));
	act.sa_handler = handler;
	act.sa_flags = SA_RESTART;
	ec_neg1( sigaction(SIGINT, &act, NULL) )
	printf("Type Ctrl-c in the next 10 secs.\n");
	ec_neg1( start = time(NULL) )
	sleep(20);
	ec_neg1( stop = time(NULL) )
	printf("Slept for %ld secs\n", (long)(stop - start));
	if (gotsig > 0)
		printf("Got signal number %ld\n", (long)gotsig);
	else
		printf("Did not get signal\n");
	exit(EXIT_SUCCESS);

EC_CLEANUP_BGN
	exit(EXIT_FAILURE);
EC_CLEANUP_END
}
/*[]*/
