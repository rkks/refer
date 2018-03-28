#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include "ssp.h"

#define NUM_TOKENS 10

int main (int argc, char **argv)
{
	char buf [LINE_LEN];
	char *tokens [NUM_TOKENS];
	int c;
	int i;

	if (argc != 2)
		err_quit ("Usage: bufsplit characters");

	bufsplit (argv [1], 0, NULL);

	while (fgets (buf, LINE_LEN, stdin) != NULL) {
		buf [strlen (buf) - 1] = '\0';
		if ((c = bufsplit (buf, NUM_TOKENS, tokens)) != NULL) {
			printf ("%d tokens found: '%s'", c, tokens [0]);
			for (i = 1; i < c; i++)
				printf (", '%s'", tokens [i]);
			printf ("\n");
		}
	}

	return (0);
}
