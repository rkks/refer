#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include "ssp.h"

int main (void)
{
	char *bin = "1010101";
	char *dec = "12345";
	char *hex = "Deadbeef";
	char *ptr;
	long val;

	printf ("%s base 2 is %ld base 10\n", bin, strtol (bin, NULL, 2));
	printf ("%s base 10 is %ld base 10\n", dec, strtol (dec, NULL, 10));
	printf ("%s base 16 is %ld base 10\n", hex, strtol (hex, NULL, 16));

	errno = 0;

	val = strtol (hex, &ptr, 16);

	if ((val == 0) && ((errno != 0) || (ptr == hex))) {
		err_quit ("No conversion took place for %s", hex);
	}
	else if ((errno != 0) && ((val == LONG_MAX) || (val == LONG_MIN))) {
		err_quit ("%s is out of range", hex);
	}
	else
		printf ("%s base 16 is %ld base 10\n", hex, val);

	return (0);
}
