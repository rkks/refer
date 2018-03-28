#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include "ssp.h"

int main (void)
{
	pid_t pid;

	if ((pid = fork ()) == -1)
		err_msg ("1st fork failed");
	else if (pid != 0) {
		sleep (2);
		exit (2);
	}

	/* In first child */

	if ((pid = fork ()) == -1)
		err_msg ("2nd fork failed");
	else if (pid != 0) {
		sleep (4);
		abort ();
	}

	/* In second child */

	if ((pid = fork ()) == -1)
		err_msg ("3rd fork failed");
	else if (pid != 0) {
		execl ("/bin/dd", "dd", "if=/kernel/genunix", "of=/dev/null", NULL);
		_exit (8);
	}

	/* In third child */

	if ((pid = fork ()) == -1)
		err_msg ("2nd fork failed");
	else if (pid != 0) {
		sleep (8);
		_exit (0);
	}

	/* In fourth child */

	sleep (6);
	kill (getpid (), SIGKILL);
	return (6);
}
