/*
	lockf test program (mandatory locking)
	AUP2, Sec. 7.11.5

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
/*[main]*/
int main(int argc, char *argv[])
{
	int fd;
	mode_t perms = PERM_FILE;

	if (fork() == 0) {
		sleep(1); /* wait for parent */
		ec_neg1( fd = open("tmpfile", O_WRONLY | O_NONBLOCK) )
		ec_neg1( write(fd, "x", 1) )
		printf("child wrote OK\n");
	}
	else {
		(void)unlink("tmpfile");
		if (argc == 2)
			perms |= S_ISGID; /* mandatory locking */
		ec_neg1( fd = open("tmpfile", O_CREAT | O_RDWR, perms) )
		ec_neg1( lockf(fd, F_LOCK, 0) )
		printf("parent has lock\n");
		ec_neg1( wait(NULL) )
	}
	exit(EXIT_SUCCESS);

EC_CLEANUP_BGN
	exit(EXIT_FAILURE);
EC_CLEANUP_END
}
