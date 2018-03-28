#include <stdio.h>
#include <ftw.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "ssp.h"

static int print (const char *name, const struct stat *stat_buf, int flags);

int main (int argc, char **argv)
{
	if (ftw (argv [1], print, 64) != 0)
		err_msg ("ftw failed");

	return (0);
}

static int print (const char *name, const struct stat *stat_buf, int flags)
{
	printf ("%s\n", name);

	return (0);
}
