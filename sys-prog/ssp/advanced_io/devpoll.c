#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/devpoll.h>
#include "ssp.h"

#define BUFFER_SIZE 1000000

static char buf [BUFFER_SIZE];

int main (void)
{
	ssize_t n;
	ssize_t res;
	char *ptr;
	int errs;
	struct pollfd fds;
	int fd;
	dvpoll_t devpoll;

	errs = 0;
	n = read (STDIN_FILENO, buf, BUFFER_SIZE);
	log_msg ("Read %d bytes", n);

	set_fsflag (STDOUT_FILENO, O_NONBLOCK);

	if ((fd = open ("/dev/poll", O_RDWR)) == -1)
		err_msg ("Can't open /dev/poll");

	fds.fd = STDOUT_FILENO;
	fds.events = POLLOUT;
	fds.revents = 0;

	devpoll.dp_fds = &fds;
	devpoll.dp_nfds = 1;
	devpoll.dp_timeout = -1;

	if (write (fd, &fds, sizeof (struct pollfd)) != sizeof (struct pollfd))
		err_msg ("Can't write pollfd structure");

	ptr = buf;
	while (n > 0) {
		if (ioctl (fd, DP_POLL, &devpoll) == -1)
			err_msg ("DP_POLL ioctl failed");

		while ((n > 0) && ((res = write (STDOUT_FILENO, ptr, n)) > 0)) {
			if (errs > 0) {
				err_ret ("write failed %d times\n", errs);
				errs = 0;
			}

			log_msg ("Wrote %d bytes", res);
			ptr += res;
			n -= res;
		}
	}

	clear_fsflag (STDOUT_FILENO, O_NONBLOCK);

	return (0);
}
