#include <stdio.h>
#include <string.h>
#include "ssp.h"

int main (int argc, char **argv)
{
	char buf [LINE_LEN];
	char *c;

	if (argc != 2)
		err_quit ("Usage: strtok characters");

	while (fgets (buf, LINE_LEN, stdin) != NULL) {
		buf [strlen (buf) - 1] = '\0';
		if ((c = strtok (buf, argv [1])) != NULL) {
			printf ("Tokens found: '%s'", c);
			while ((c = strtok (NULL, argv [1])) != NULL)
				printf (", '%s'", c);
			printf ("\n");
		}
	}

	return (0);
}
