/*
	Testing timestart and timestop
	Sec. 1.7.2

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
#include <sys/times.h>
#include <sys/time.h>

/*[time_test]*/
#define REPS 1000000
#define TV_SUBTRACT(t2, t1)\
	(double)(t2).tv_sec + (t2).tv_usec / 1000000.0 -\
	  ((double)(t1).tv_sec + (t1).tv_usec / 1000000.0)

int main(void)
{
	int i;
	char msg[100];
	clock_t c1, c2;
	struct timeval tv1, tv2;

	snprintf(msg, sizeof(msg), "%d getpids", REPS);
	ec_neg1( c1 = clock() )
	gettimeofday(&tv1, NULL);
	timestart();
	for (i = 0; i < REPS; i++)
		(void)getpid();
	(void)sleep(2);
	timestop(msg);
	gettimeofday(&tv2, NULL);
	ec_neg1( c2 = clock() )
	printf("clock(): %.2f\n", (double)(c2 - c1) / CLOCKS_PER_SEC);
	printf("gettimeofday(): %.2f\n", TV_SUBTRACT(tv2, tv1));
	exit(EXIT_SUCCESS);

EC_CLEANUP_BGN
	exit(EXIT_FAILURE);
EC_CLEANUP_END
}
