#include <unistd.h>
#include <errno.h>

ssize_t readn (int fd, void *buf, size_t num)
{
	ssize_t res;
	size_t n;
	char *ptr;

	n = num;
	ptr = buf;
	while (n > 0) {
		if ((res = read (fd, ptr, n)) == -1) {
			if (errno == EINTR)
				res = 0;
			else
				return (-1);
		}
		else if (res == 0)
			break;

		ptr += res;
		n -= res;
	}

	return (num - n);
}

ssize_t writen (int fd, const void *buf, size_t num)
{
	ssize_t res;
	size_t n;
	const char *ptr;

	n = num;
	ptr = buf;
	while (n > 0) {
		if ((res = write (fd, ptr, n)) <= 0) {
			if (errno == EINTR)
				res = 0;
			else
				return (-1);
		}

		ptr += res;
		n -= res;
	}

	return (num);
}
