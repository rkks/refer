#include <fcntl.h>

int ssp_lock (int fd, int cmd, short type, short whence, off_t start,
	off_t len)
{
	flock_t lock;

	lock.l_type = type;
	lock.l_whence = whence;
	lock.l_start = start;
	lock.l_len = len;

	return (fcntl (fd, cmd, &lock));
}
