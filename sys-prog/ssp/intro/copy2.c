#include <stdio.h>
#include "ssp.h"

int main (void)
{
	int c;

	while ((c = getchar ()) != EOF) {
		if (putchar (c) == EOF)
			err_msg ("putchar failed");
	}

	if (ferror (stdin))
		err_msg ("getchar failed");

	return (0);
}
