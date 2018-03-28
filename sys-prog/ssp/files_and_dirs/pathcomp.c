#include <stdio.h>
#include <libgen.h>
#include <string.h>
#include <stdlib.h>
#include "ssp.h"

int main (int argc, char **argv)
{
	int n;
	char *ptr1;
	char *ptr2;
	char *dir;
	char *base;

	for (n = 1; n < argc; n++) {
		if ((ptr1 = strdup (argv [n])) == NULL)
			err_msg ("strdup failed");
		dir = dirname (ptr1);

		if ((ptr2 = strdup (argv [n])) == NULL)
			err_msg ("strdup failed");
		base = basename (ptr2);

		printf ("%s:\n", argv [n]);
		printf ("  dirname = %s\n", dir);
		printf ("  basename = %s\n\n", base);

		free (ptr1);
		free (ptr2);
	}

	return (0);
}
