#include <stdio.h>
#include <string.h>
#include "ssp.h"

extern void highlight (char *line, char *start, char *finish);

int main (int argc, char **argv)
{
	char buf [LINE_LEN];
	char *c;

	if (argc != 2)
		err_quit ("Usage: strstrs character");

	while (fgets (buf, LINE_LEN, stdin) != NULL) {
		buf [strlen (buf) - 1] = '\0';
		if ((c = strstr (buf, argv [1])) != NULL) {
			printf ("%s\n", buf);
			highlight (buf, c, c + (strlen (argv [1]) - 1));
			printf ("%s\n", buf);
		}
	}

	return (0);
}
