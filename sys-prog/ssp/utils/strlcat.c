#include <stdio.h>
#include <string.h>
#include "ssp.h"

#define BUFFER_LEN 80
#define STR_LEN 80
#define NUM_STR 1024

int main (void)
{
	char strings [NUM_STR] [STR_LEN];
	char buf [BUFFER_LEN];
	int num_strings;
	int i;

	for (num_strings = 0; num_strings < NUM_STR; num_strings++) {
		if (fgets (strings [num_strings], STR_LEN, stdin) == NULL)
			break;
		strings [num_strings] [strlen (strings [num_strings]) - 1] = '\0';
	}

	buf [0] = '\0';

	for (i = 0; i < num_strings; i++) {
		if (strlcat (buf, strings [i], BUFFER_LEN) >= BUFFER_LEN)
			err_quit ("Buffer overflow on string %d: '%s'", i + 1, buf);
		if (strlcat (buf, ", ", BUFFER_LEN) >= BUFFER_LEN)
			err_quit ("Buffer overflow on string %d: '%s'", i + 1, buf);
	}

	printf ("Buffer = '%s'\n", buf);

	return (0);
}
