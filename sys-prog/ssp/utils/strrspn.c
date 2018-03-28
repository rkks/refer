#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include "ssp.h"

extern void highlight (char *line, char *start, char *finish);

int main (int argc, char **argv)
{
	char buf [LINE_LEN];
	char *c;

	if (argc != 2)
		err_quit ("Usage: strrspn character");

	while (fgets (buf, LINE_LEN, stdin) != NULL) {
		buf [strlen (buf) - 1] = '\0';
		if ((c = strrspn (buf, argv [1])) != NULL) {
			printf ("%s\n", buf);
			highlight (buf, c, c);
			printf ("%s\n", buf);
		}
	}

	return (0);
}
