#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <lastlog.h>
#include <time.h>
#include <pwd.h>
#include "ssp.h"

static struct lastlog *getllnam (const char *name);

int main (int argc, char **argv)
{
	struct lastlog *ll_entry;
	int i;

	if (argc < 2)
		err_quit ("Usage: lastlog username ...");

	for (i = 1; i < argc; i++) {
		if ((ll_entry = getllnam (argv [i])) == NULL)
			continue;

		if (ll_entry -> ll_time != 0)
			printf ("%-8s  %-8s  %-16s  %s", argv [i], ll_entry -> ll_line,
				ll_entry -> ll_host, ctime (&ll_entry -> ll_time));
		else
			printf ("%-8s  never logged in\n", argv [i]);
	}

	return (0);
}

static struct lastlog *getllnam (const char *name)
{
	static struct lastlog ll_entry;
	struct passwd *pwd;
	int fd;

	if ((fd = open ("/var/adm/lastlog", O_RDONLY)) == -1)
		return (NULL);

	if ((pwd = getpwnam (name)) == NULL)
		return (NULL);

	pread (fd, &ll_entry, sizeof (struct lastlog),
		pwd -> pw_uid * sizeof (struct lastlog));
	close (fd);

	return (&ll_entry);
}
