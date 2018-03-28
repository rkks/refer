#include <stdio.h>
#include <string.h>
#include "ssp.h"

extern void highlight (char *line, char *start, char *finish);

int main (int argc, char **argv)
{
	char buf [LINE_LEN];
	char *c;
	char look_for;

	if (argc != 2)
		err_quit ("Usage: strrchr character");

	look_for =  *argv [1];

	while (fgets (buf, LINE_LEN, stdin) != NULL) {
		buf [strlen (buf) - 1] = '\0';
		if ((c = strrchr (buf, look_for)) != NULL) {
			printf ("%s\n", buf);
			highlight (buf, c, c);
			printf ("%s\n", buf);
		}
	}

	return (0);
}
