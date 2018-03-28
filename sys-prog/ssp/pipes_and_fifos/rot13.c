#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include "ssp.h"

int main (void)
{
	char buf [2];
	char *old = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	char *new = "NOPQRSTUVWXYZABCDEFGHIJKLMnopqrstuvwxyzabcdefghijklm";
	char result [2];

	buf [1] = '\0';
	result [1] = '\0';

	while ((buf [0] = getchar ()) != EOF) {
		strtrns (buf, old, new, result);

		if (putchar (result [0]) == EOF)
			err_msg ("putchar failed");

		if (buf [0] == '\n')
			fflush (stdout);
	}

	return (0);
}
