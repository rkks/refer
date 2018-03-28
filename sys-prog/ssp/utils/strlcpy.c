#include <stdio.h>
#include <string.h>
#include "ssp.h"

#define BUFFER_LEN 20
#define STR_LEN 80

int main (void)
{
	char string [STR_LEN];
	char buf [BUFFER_LEN];

	fgets (string, STR_LEN, stdin);
	string [strlen (string) - 1] = '\0';

	if (strlcpy (buf, string, BUFFER_LEN) >= BUFFER_LEN)
		err_quit ("Buffer overflow on detected, buffer = '%s'", buf);

	printf ("Buffer = '%s'\n", buf);

	return (0);
}
