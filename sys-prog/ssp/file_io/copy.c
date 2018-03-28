#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "ssp.h"

int main (int argc, char **argv)
{
	char *buf;
	int buf_size;
	ssize_t n;
	ssize_t res;
	char *ptr;

	if (argc != 2)
		buf_size = 8192;
	else
		buf_size = atoi (argv [1]);

	if ((buf = malloc (buf_size)) == NULL)
		err_msg ("malloc failed");

	while ((n = read (STDIN_FILENO, buf, buf_size)) > 0) {
		ptr = buf;
		while ((n > 0) && ((res = write (STDOUT_FILENO, ptr, n)) > 0)) {
			ptr += res;
			n -= res;
		}

		if (res == -1)
			err_msg ("write failed");
	}

	if (n == -1)
		err_msg ("read failed");

	return (0);
}
