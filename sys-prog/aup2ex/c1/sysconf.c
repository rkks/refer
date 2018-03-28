/*
	Using sysconf
	Sec. 1.5.5

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
#include <sys/utsname.h>

int main(void)
{
	bool lookup, optsymdefined;
	char *when, *sc_symb;
	struct utsname name;
	FILE *out = fopen("/aup/c1/susopen.txt", "w");

/*[sysconf1]*/
	long value;

#if defined(_POSIX_ASYNCHRONOUS_IO)
	printf("_POSIX_ASYNCHRONOUS_IO = %ld\n", (long)_POSIX_ASYNCHRONOUS_IO);
#else
	printf("_POSIX_ASYNCHRONOUS_IO undefined\n");
#endif
#if defined(_SC_ASYNCHRONOUS_IO)
	errno = 0;
	if ((value = sysconf(_SC_ASYNCHRONOUS_IO)) == -1)
		if (errno == EINVAL)
			EC_FAIL
		else
			printf("sysconf(...) = unsupported\n");
	else
		printf("sysconf(...) = %ld\n", value);
#else
	printf("_SC_ASYNCHRONOUS_IO undefined\n");
#endif
/*[]*/
{
/*[sysconf2]*/
	long value;

#if defined(_SC_ATEXIT_MAX)
	errno = 0;
	if ((value = sysconf(_SC_ATEXIT_MAX)) == -1)
		if (errno == EINVAL)
			EC_FAIL
		else
			printf("max atexit registrations: unlimited\n");
	else
		printf("max atexit registrations: %ld\n", value);
#else
	printf("_SC_ATEXIT_MAX undefined\n");
#endif
/*[]*/
	printf("LONG_MAX = %ld\n", LONG_MAX);
}

	ec_null( out )
	ec_neg1( uname(&name) )
#if 0
	printf("sysname = %s\n", name.sysname);
	printf("nodename = %s\n", name.nodename);
	printf("release = %s\n", name.release);
	printf("version = %s\n", name.version);
	printf("machine = %s\n", name.machine);
#endif
	printf("_POSIX_VERSION = %ld\n", _POSIX_VERSION);
#ifdef _XOPEN_UNIX
	printf("_XOPEN_VERSION = %ld\n", (long)_XOPEN_VERSION);
#else
	printf("_XOPEN_UNIX undefined\n");
#endif
return 0;

EC_CLEANUP_BGN
	exit(EXIT_FAILURE);
EC_CLEANUP_END
}
