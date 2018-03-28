#include <unistd.h>
#include <stdlib.h>
#include "ssp.h"

int main (void)
{
	switch (fork ()) {
		case -1:
			err_msg ("fork failed");

		case 0:
			_exit (0);

		default:
			sleep (2);
			system ("ps -o s,user,pid,ppid,comm -u rich");
			break;
	}

	return (0);
}
