#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include "ssp.h"

int main (int argc, char **argv)
{
	char buf [LINE_LEN];
	char *old = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	char *new = "NOPQRSTUVWXYZABCDEFGHIJKLMnopqrstuvwxyzabcdefghijklm";
	char result [LINE_LEN];

	if (argc != 1)
		err_quit ("Usage: %s", argv [0]);

	while (fgets (buf, LINE_LEN, stdin) != NULL) {
		buf [strlen (buf) - 1] = '\0';
		strtrns (buf, old, new, result);
		printf ("%s\n", result);
	}

	return (0);
}
