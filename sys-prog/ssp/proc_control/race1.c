#include <sys/types.h>
#include <unistd.h>
#include "ssp.h"

static void print_chars (const char *string);

int main (void)
{
	switch (fork ()) {
		case -1:
			err_msg ("Can't fork");
			break;

		case 0:
			print_chars ("Hello from child\n");
			_exit (0);

		default:
			print_chars ("Hello from parent\n");
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
