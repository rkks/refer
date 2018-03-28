#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "ssp.h"

int main (void)
{
	char buf [BUF_SIZE];
	ssize_t n;

	if (set_fsflag (STDOUT_FILENO, O_SYNC) == -1)
		err_msg ("set_fsflag failed");

	while ((n = read (STDIN_FILENO, buf, BUF_SIZE)) > 0) {
		if (writen (STDOUT_FILENO, buf, n) == -1)
			err_msg ("writen failed");
	}

	if (n == -1)
		err_msg ("read failed");

	return (0);
}
