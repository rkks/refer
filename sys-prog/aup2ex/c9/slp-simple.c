/*
	sleep implementation using alarm (naive)
	AUP2, Sec. 9.07.2

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
// assumes no other use of alarm
/*[pgm]*/
static void slp_handler(int signum)
{
}

unsigned aup_sleep(unsigned secs)
{
	struct sigaction act;
	unsigned unslept;

	memset(&act, 0, sizeof(act));
	act.sa_handler = slp_handler;
	ec_neg1( sigaction(SIGALRM, &act, NULL) )
	alarm(secs);
	pause();
	unslept = alarm(0);
	return unslept;

EC_CLEANUP_BGN
	EC_FLUSH("aup_sleep")
	return 0;
EC_CLEANUP_END
}
/*[]*/
int main(void)
{
	time_t tm;

	time(&tm);
	printf("Test 1 -- time %ld\n", (long)tm);
	aup_sleep(6);
	time(&tm);
	printf("Exiting  -- time %ld\n", (long)tm);
	exit(EXIT_SUCCESS);
}
