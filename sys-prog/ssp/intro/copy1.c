#include <unistd.h>
#include "ssp.h"

int main (void)
{
	ssize_t n;
	ssize_t res;
	char buf [BUF_SIZE];
	char *ptr;

	while ((n = read (STDIN_FILENO, buf, BUF_SIZE)) > 0) {
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
