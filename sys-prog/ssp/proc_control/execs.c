#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "ssp.h"

static char *new_env [] = {
	"USER=richteer",
	"PATH=/tmp",
	NULL
};

int main (void)
{
	switch (fork ()) {
		case -1:
			err_msg ("Can't fork");
			break;

		case 0:
			execle ("./echo_all", "echo_all", "arg1", "arg 2", NULL, new_env);
			err_msg ("execle failed");
	}

	if (wait (NULL) == -1)
		err_msg ("wait failed");

	switch (fork ()) {
		case -1:
			err_msg ("Can't fork");
			break;

		case 0:
			execlp ("echo_all", "echo_all", "only 1 arg", NULL);
			err_msg ("execlp failed");
	}

	return (0);
}
