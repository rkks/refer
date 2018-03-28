#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include "ssp.h"

#define BUFFER_SIZE 100000

static char buf [BUFFER_SIZE];

int main (void)
{
	ssize_t n;
	ssize_t res;
	char *ptr;
	int errs;

	errs = 0;
	n = read (STDIN_FILENO, buf, BUFFER_SIZE);
	log_msg ("Read %d bytes", n);

	set_fsflag (STDOUT_FILENO, O_NONBLOCK);

	while (n > 0) {
		ptr = buf;
		while ((n > 0) && ((res = write (STDOUT_FILENO, ptr, n)) > 0)) {
			if (errs > 0) {
				err_ret ("write failed %d times", errs);
				errs = 0;
			}

			log_msg ("Wrote %d bytes", res);
			ptr += res;
			n -= res;
		}

		if (res == -1)
			errs++;
	}

	clear_fsflag (STDOUT_FILENO, O_NONBLOCK);

	return (0);
}
