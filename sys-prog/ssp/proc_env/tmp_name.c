#include <stdio.h>
#include <unistd.h>
#include <sys/param.h>

char *tmp_name (const char *base)
{
	char name [MAXPATHLEN];

	snprintf (name, MAXPATHLEN, "/tmp/%s%d", base, getpid ());
	return (name);
}
