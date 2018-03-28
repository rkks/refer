/*
	getrlimit and setrlimit example
	AUP2, Sec. 5.16

	Copyright 2003 by Marc J. Rochkind. All rights reserved.
	May be copied only for purposes and under conditions described
	on the Web page www.basepath.com/aup/copyright.htm.

	The Example Files are provided "as is," without any warranty;
	without even the implied warranty of merchantability or fitness
	for a particular purpose. The author and his publisher are not
	responsible for any damages, direct or incidental, resulting
	from the use or non-use of these Example Files.

	The Example Files may contain defects, and some contain deliberate
	coding mistakes that were included for educational reasons.
	You are responsible for determining if and how the Example Files
	are to be used.

*/
#include "defs.h"
#include <sys/resource.h>
/*[pgm]*/
static void showvalue(rlim_t lim)
{
	/*
		All macros may equal RLIM_INFINITY; that test
		must be first; can't use switch statement.
	*/
	if (lim == RLIM_INFINITY)
		printf("RLIM_INFINITY");
#ifndef BSD_DERIVED
	else if (lim == RLIM_SAVED_CUR)
		printf("RLIM_SAVED_CUR");
	else if (lim == RLIM_SAVED_MAX)
		printf("RLIM_SAVED_MAX");
#endif
	else
		printf("%llu", (unsigned long long)lim);
}

static bool showlimit(int resource, const char *name)
{
	struct rlimit r;

	ec_neg1( getrlimit(resource, &r) )
	printf("%s: ", name);
	printf("rlim_cur = ");
	showvalue(r.rlim_cur);
	printf("; rlim_max = ");
	showvalue(r.rlim_max);
	printf("\n");
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}

int main(void)
{
	struct rlimit r;
	int fd;
	char buf[500] = { 0 };

	if (sizeof(rlim_t) > sizeof(long long))
		printf("Warning: rlim_t > long long; results may be wrong\n");
	ec_false( showlimit(RLIMIT_CORE, "RLIMIT_CORE") )
	ec_false( showlimit(RLIMIT_CPU, "RLIMIT_CPU") )
	ec_false( showlimit(RLIMIT_DATA, "RLIMIT_DATA") )
	ec_false( showlimit(RLIMIT_FSIZE, "RLIMIT_FSIZE") )
	ec_false( showlimit(RLIMIT_NOFILE, "RLIMIT_NOFILE") )
	ec_false( showlimit(RLIMIT_STACK, "RLIMIT_STACK") )
#ifndef BSD_DERIVED
	ec_false( showlimit(RLIMIT_AS, "RLIMIT_AS") )
#endif
	ec_neg1( getrlimit(RLIMIT_FSIZE, &r) )
	r.rlim_cur = 500;
	ec_neg1( setrlimit(RLIMIT_FSIZE, &r) )
	ec_neg1( fd = open("tmp", O_WRONLY | O_CREAT | O_TRUNC, PERM_FILE) )
	ec_neg1( write(fd, buf, sizeof(buf)) )
	ec_neg1( write(fd, buf, sizeof(buf)) )
	printf("Wrote two buffers! (?)\n");
	exit(EXIT_SUCCESS);

EC_CLEANUP_BGN
	exit(EXIT_FAILURE);
EC_CLEANUP_END
}
/*[]*/
