/*
	Display STREAMS modules
	AUP2, Sec. 4.09

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

#define WANT_TEST

#if _XOPEN_STREAMS > 0

#include "streamlist.h"

#include <stropts.h>
#include <sys/conf.h>

bool stream_list(int fd)
{
	int num_modules, i;
	struct str_list *list;

	ec_neg1( num_modules = ioctl(fd, I_LIST, NULL) )
	printf("Stream has %d modules\n", num_modules);
	if (num_modules > 0) {
		ec_null( list = malloc(sizeof(struct str_list)) )
		list->sl_nmods = num_modules;
		ec_null( list->sl_modlist = malloc(num_modules *
		  sizeof(struct str_mlist)) )
		ec_neg1( ioctl(fd, I_LIST, list) )
		for (i = 0; i < list->sl_nmods; i++)
			printf("Module %d: \"%s\"\n", i, list->sl_modlist[i].l_name);
	}
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}

#ifdef WANT_TEST

int main(void)
{
	int fds[2], fd;

	printf("Standard input:\n");
	ec_false( stream_list(STDIN_FILENO) )

	ec_neg1( pipe(fds) )
	printf("\nPipe:\n");
	ec_false( stream_list(fds[0]) )

	ec_neg1( fd = open("/dev/console", O_WRONLY) )
	printf("\n/dev/console:\n");
	ec_false( stream_list(fd) )

	ec_neg1( fd = open("/dev/tty", O_WRONLY) )
	printf("\n/dev/tty:\n");
	ec_false( stream_list(fd) )

	exit(EXIT_SUCCESS);

EC_CLEANUP_BGN
	exit(EXIT_FAILURE);
EC_CLEANUP_END
}

#endif /* WANT_TEST */

/*

I_LIST
Allows the user to list all the module names on the stream, up to and including the topmost driver name. If arg is NULL, the return value is the number of modules, including the driver, that are on the STREAM pointed to by fildes. This allows the user to allocate enough space for the module names. If arg is non-null, it should point to an str_list structure that has the following members:

int sl_nmods;
struct  str_mlist  *sl_modlist;
The str_mlist structure has the following member:

char l_name[FMNAMESZ+1];
The sl_nmods member indicates the number of entries the process has allocated in the array. Upon return, the sl_modlist member of the str_list structure contains the list of module names, and the number of entries that have been filled into the sl_modlist array is found in the sl_nmods member (the number includes the number of modules including the driver). The return value from ioctl() is 0. The entries are filled in starting at the top of the STREAM and continuing downstream until either the end of the STREAM is reached, or the number of requested modules (sl_nmods) is satisfied. On failure, errno may be set to one of the following values:

EINVAL
The sl_nmods member is less than 1.

EAGAIN
Unable to allocate buffers

*/

#else /* _XOPEN_STREAMS */

#ifdef WANT_TEST

int main(void)
{
	printf("STREAMS not supported.\n");
	exit(EXIT_FAILURE);
}

#endif /* WANT_TEST */

#endif /* _XOPEN_STREAMS */
