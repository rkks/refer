#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include "ssp.h"

int main (void)
{
	int status;

	switch (fork ()) {
		case -1:
			err_msg ("Can't fork");

		case 0:
			_exit (123);
	}

	if (wait (&status) == -1)
		err_msg ("wait error");
	print_term_status (status);

	switch (fork ()) {
		case -1:
			err_msg ("Can't fork");

		case 0:
			abort ();
	}

	if (wait (&status) == -1)
		err_msg ("wait error");
	print_term_status (status);

	switch (fork ()) {
		case -1:
			err_msg ("Can't fork");

		case 0:
			status /= (getpid () - getpid ());
	}

	if (wait (&status) == -1)
		err_msg ("wait error");
	print_term_status (status);

	return (0);
}
