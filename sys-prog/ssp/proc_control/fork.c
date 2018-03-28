#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "ssp.h"

int global_var = 123;
char buf [] = "A write to stdout.\n";

int main (void)
{
	int local_var;

	local_var = 456;
	if (writen (STDOUT_FILENO, buf, strlen (buf)) != strlen (buf))
		err_msg ("writen failed");
	printf ("Before fork...\n");

	switch (fork ()) {
		case -1:
			err_msg ("Can't fork");
			break;

		case 0:				/* Child */
			global_var++;
			local_var++;
			break;

		default:			/* Parent */
			sleep (2);
			break;
	}

	printf ("PID = %ld, global_var = %d, local_var = %d\n", (long) getpid (),
		global_var, local_var);

	return (0);
}
