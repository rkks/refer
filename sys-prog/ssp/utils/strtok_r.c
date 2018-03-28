#include <stdio.h>
#include <string.h>
#include "ssp.h"

int main (int argc, char **argv)
{
	char buf [LINE_LEN];
	char *c;
	char *ptr;

	if (argc != 2)
		err_quit ("Usage: strtok_r characters");

	while (fgets (buf, LINE_LEN, stdin) != NULL) {
		buf [strlen (buf) - 1] = '\0';
		if ((c = strtok_r (buf, argv [1], &ptr)) != NULL) {
			printf ("Tokens found: '%s'", c);
			while ((c = strtok_r (NULL, argv [1], &ptr)) != NULL)
				printf (", '%s'", c);
			printf ("\n");
		}
	}

	return (0);
}
