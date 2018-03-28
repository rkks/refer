#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "ssp.h"

int main (int argc, char **argv)
{
	int n;
	char buf1 [PATH_MAX];
	char buf2 [PATH_MAX];

	for (n = 1; n < argc; n++) {
		memset (buf1, 0, PATH_MAX);
		if (resolvepath (argv [n], buf1, PATH_MAX) == -1)
			err_msg ("resolvepath failed");

		if (realpath (argv [n], buf2) == NULL)
			err_msg ("realpath failed");

		printf ("%s:\n", argv [n]);
		printf ("  resolvepath = %s\n", buf1);
		printf ("  realpath = %s\n\n", buf2);
	}

	return (0);
}
