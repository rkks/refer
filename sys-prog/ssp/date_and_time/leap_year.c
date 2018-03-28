#include <stdio.h>
#include <stdlib.h>
#include "ssp.h"

int main (int argc, char **argv)
{
	int y;
	int ly;

	if (argc != 2)
		err_quit ("Usage: leap_year year");

	y = atoi (argv [1]);

	ly = (y % 4 == 0) ? ((y % 100 == 0) ? ((y % 400 == 0) ? 1 : 0) : 1) : 0;

	printf ("%d %s a leap year\n", y, (ly == 0) ? "is not" : "is");

	return (0);
}
