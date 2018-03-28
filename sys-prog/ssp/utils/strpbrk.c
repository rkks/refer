#include <stdio.h>
#include <string.h>
#include "ssp.h"

extern void highlight (char *line, char *start, char *finish);

int main (int argc, char **argv)
{
	char buf [LINE_LEN];
	char *c;

	if (argc != 2)
		err_quit ("Usage: %s character", argv [0]);

	while (fgets (buf, LINE_LEN, stdin) != NULL) {
		buf [strlen (buf) - 1] = '\0';
		if ((c = strpbrk (buf, argv [1])) != NULL) {
			printf ("%s\n", buf);
			highlight (buf, c, c);
			printf ("%s\n", buf);
		}
	}

	return (0);
}
