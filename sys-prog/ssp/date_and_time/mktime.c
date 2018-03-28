#include <stdio.h>
#include <time.h>

int main (void)
{
	struct tm tp;
	char *wday [] = {
		"Sunday", "Monday", "Tuesday", "Wednesday",
		"Thursday", "Friday", "Saturday", "Unknown"
	};

	tp.tm_sec = 1;
	tp.tm_min = 0;
	tp.tm_hour = 0;
	tp.tm_mday = 18;
	tp.tm_mon = 9 - 1;
	tp.tm_year = 1967 - 1900;
	tp.tm_isdst = -1;

	if (mktime (&tp) == -1)
		tp.tm_wday = 7;
	
	printf ("September 18th, 1967 was a %s.\n", wday [tp.tm_wday]);

	return (0);
}
