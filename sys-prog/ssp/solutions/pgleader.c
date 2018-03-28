#include <sys/types.h>
#include <unistd.h>
#include "ssp.h"

int main (void)
{
	pid_t pid;

	if ((pid = fork ()) == -1)
		err_msg ("fork failed");
	if (pid == 0) {
		if (setsid () == -1)
			err_msg ("setsid failed");
		sleep (5);
	}

	return (0);
}
