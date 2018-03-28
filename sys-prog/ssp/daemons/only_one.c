#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "ssp.h"

int main (void)
{
	int res;

	if ((res = one_copy ("/tmp/macleod")) == -1)
		err_msg ("one_copy failed");

	if (res == 1) {
		log_msg ("There can be only one!");
		exit (1);
	}

	pause ();

	return (0);
}
