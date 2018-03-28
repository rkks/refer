#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "ssp.h"

int main (void)
{
	switch (fork ()) {
		case -1:
			err_msg ("Can't fork");
			break;

		case 0:
			execle ("./script", "script", "arg1", "arg 2", NULL, NULL);
			err_msg ("execl failed");
	}

	if (wait (NULL) == -1)
		err_msg ("wait failed");

	return (0);
}
