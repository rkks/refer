#include <sys/types.h>
#include <unistd.h>
#include "ssp.h"

static void print_chars (const char *string);

int main (void)
{
	pid_t pid;

	if (ssp_tell_wait_init () == -1)
		err_msg ("ssp_tell_wait_init failed");

	switch (pid = fork ()) {
		case -1:
			err_msg ("Can't fork");
			break;

		case 0:
			if (ssp_wait_parent () == -1)
				err_msg ("ssp_wait_parent failed");
			print_chars ("Hello from child\n");
			_exit (0);

		default:
			print_chars ("Hello from parent\n");
			if (ssp_tell_child (pid) == -1)
				err_msg ("ssp_tell_child failed");
			break;
	}

	return (0);
}

static void print_chars (const char *string)
{
	const char *c;

	for (c = string; *c; c++)
		writen (STDOUT_FILENO, c, 1);
}
