#include <stdio.h>
#include <unistd.h>
#include "ssp.h"

int main (void)
{
	if (chdir ("/tmp") != 0)
		err_msg ("chdir failed");

	printf ("chdir to /tmp was successful\n");

	return (0);
}
