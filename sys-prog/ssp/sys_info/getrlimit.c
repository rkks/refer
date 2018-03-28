#include <stdio.h>
#include <sys/resource.h>
#include "ssp.h"

#define plimit(name) print_limits (#name, name)

static void print_limits (const char *name, int resource);

int main (void)
{
	plimit (RLIMIT_AS);
	plimit (RLIMIT_CORE);
	plimit (RLIMIT_CPU);
	plimit (RLIMIT_DATA);
	plimit (RLIMIT_FSIZE);
	plimit (RLIMIT_NOFILE);
	plimit (RLIMIT_STACK);
	plimit (RLIMIT_VMEM);

	return (0);
}

static void print_limits (const char *name, int resource)
{
	struct rlimit limits;

	if (getrlimit (resource, &limits) == -1)
		err_quit ("getrlimit (%s) failed", name);

	printf ("%-13s  ", name);

	if (limits.rlim_cur == RLIM_INFINITY)
		printf ("unlimited  ");
	else
		printf ("%9ld  ", limits.rlim_cur);

	if (limits.rlim_max == RLIM_INFINITY)
		printf ("unlimited\n");
	else
		printf ("%9ld\n", limits.rlim_max);
}
