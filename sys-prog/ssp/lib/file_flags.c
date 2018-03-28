#include <fcntl.h>

int set_fsflag (int fd, int new_flags)
{
	int flags;

	if ((flags = fcntl (fd, F_GETFL)) == -1)
		return (-1);

	flags |= new_flags;

	if ((flags = fcntl (fd, F_SETFL, flags)) == -1)
		return (-1);

	return (0);
}

int clear_fsflag (int fd, int new_flags)
{
	int flags;

	if ((flags = fcntl (fd, F_GETFL)) == -1)
		return (-1);

	flags &= ~new_flags;

	if ((flags = fcntl (fd, F_SETFL, flags)) == -1)
		return (-1);

	return (0);
}
