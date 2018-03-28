#include <sys/time.h>
#include <sys/types.h>
#include <stdio.h>
#include "micro.h"

/* returns a timestamp in microseconds */
int64_t micro(void)
{
	int64_t val;
	struct timeval tv;
	gettimeofday(&tv, NULL);
	val=tv.tv_sec;
	val*=1000000;
	val+=tv.tv_usec;
	return(val);
}
