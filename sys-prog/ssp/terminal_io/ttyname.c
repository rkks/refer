#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <termios.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include "ssp.h"

static int ssp_ttyname_r (int fd, char *name, size_t namesize);
static int dir_search (char *dir, struct stat *fd_stat, char *name);

int main (void)
{
	int i;
	char buf [_POSIX_PATH_MAX];

	for (i = 0; i < 3; i++) {
		if (ssp_ttyname_r (i, buf, _POSIX_PATH_MAX) == 0)
			printf ("fd %d: %s\n", i, buf);
		else
			printf ("fd %d: Not a TTY\n", i);
	}

	return (0);
}

static int ssp_ttyname_r (int fd, char *name, size_t namesize)
{
	struct stat fd_stat;
	char buf [_POSIX_PATH_MAX];

	if (isatty (fd) == 0)
		return (ENOTTY);
	if (fstat (fd, &fd_stat) == -1)
		return (errno);
	if (namesize < _POSIX_PATH_MAX)
		return (ERANGE);

	if (dir_search ("/dev/", &fd_stat, buf) == 0) {
		strcpy (name, buf);
		return (0);
	}
	else
		return (errno);
}

static int dir_search (char *dir, struct stat *fd_stat, char *name)
{
	struct stat dev_stat;
	DIR *dp;
	struct dirent *dirp;
	char buf [_POSIX_PATH_MAX];

	if ((dp = opendir (dir)) == NULL)
		return (-1);

	if ((readdir (dp) == NULL) || (readdir (dp) == NULL)) {
		closedir (dp);
		return (-1);
	}

	while ((dirp = readdir (dp)) != NULL) {
		snprintf (buf, _POSIX_PATH_MAX, "%s%s", dir, dirp -> d_name);
		if (stat (buf, &dev_stat) == -1)
			continue;
		if (S_ISDIR (dev_stat.st_mode)) {
			strcat (buf, "/");
			if (dir_search (buf, fd_stat, name) == 0) {
				closedir (dp);
				return (0);
			}
		}
		else {
			if ((dev_stat.st_ino == fd_stat -> st_ino) &&
				(dev_stat.st_dev == fd_stat -> st_dev) &&
				(dev_stat.st_rdev == fd_stat -> st_rdev)) {
				strcpy (name, buf);
				closedir (dp);
				return (0);
			}
		}
	}

	closedir (dp);
	errno = EINVAL;
	return (-1);
} 
