#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "ssp.h"

static int global_var = 123;

int main (void)
{
	int local_var;

	local_var = 456;
	printf ("Before vfork...\n");

	switch (vfork ()) {
		case -1:
			err_msg ("Can't vfork");
			break;

		case 0:				/* Child */
			global_var++;
			local_var++;
			_exit (0);
	}

	printf ("PID = %ld, global_var = %d, local_var = %d\n", (long) getpid (),
		global_var, local_var);

	return (0);
}
