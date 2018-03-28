#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include "ssp.h"

typedef struct {
	int sysid;
	pid_t pid;
} ssp_lowner_t;

ssp_lowner_t *ssp_lock_test (int fd, short type, short whence, off_t start,
	off_t len)
{
	flock_t lock;
	static ssp_lowner_t lock_owner;

	lock.l_type = type;
	lock.l_whence = whence;
	lock.l_start = start;
	lock.l_len = len;

	if (fcntl (fd, F_GETLK, &lock) == -1)
		err_quit ("fcntl failed");

	if (lock.l_type == F_UNLCK)
		return (NULL);

	lock_owner.sysid = lock.l_sysid;
	lock_owner.pid = lock.l_pid;

	return (&lock_owner);
}
