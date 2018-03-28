#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "ssp.h"

int main (void)
{
	char *tz;

	if ((tz = getenv ("TZ")) == NULL)
		err_quit ("TZ is not in environment\n");

	tzset ();

	printf ("Time zone = %s\n", tz);
	printf ("  timezone difference = %ld seconds\n", timezone);
	printf ("  altzone difference = %ld seconds\n", altzone);
	printf ("  daylight = %d\n", daylight);
	printf ("  tzname [0] = %s\n", tzname [0]);
	printf ("  tzname [1] = %s\n", tzname [1]);

	return (0);
}
