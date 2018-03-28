/*
	Checking for an option.
	Sec. 1.5.4

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

#define PATHNAME "temporary"

int main(void)
{
#if !defined(_POSIX_ASYNCHRONOUS_IO) || _POSIX_ASYNCHRONOUS_IO == -1
	printf("_POSIX_ASYNCHRONOUS_IO unsupported\n");
	return EXIT_SUCCESS;
#else
	int fd;

	ec_neg1( fd = open(PATHNAME, O_WRONLY | O_CREAT, PERM_FILE) )
	ec_neg1( close(fd) )

	switch (option_async_io(PATHNAME)) {
	case OPT_YES:
		printf("async supported on %s\n", PATHNAME);
		break;
	case OPT_NO:
		printf("async unsupported on %s\n", PATHNAME);
		break;
	case OPT_ERROR:
		EC_FAIL
	}
	return EXIT_SUCCESS;

EC_CLEANUP_BGN
	return EXIT_FAILURE;
EC_CLEANUP_END
#endif
}
