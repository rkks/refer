/*
	piperead (goes with pipewrite in x6.c)
	AUP2, Sec. 6.02.3

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

/*[piperead]*/
int main(int argc, char *argv[])
{
	int fd;
	ssize_t nread;
	char s[100];

	fd = atoi(argv[1]);
	printf("reading file descriptor %d\n", fd);
	ec_neg1( nread = read(fd, s, sizeof(s)) )
	if (nread == 0)
		printf("EOF\n");
	else
		printf("read %ld bytes: %s\n", (long)nread, s);
	exit(EXIT_SUCCESS);

EC_CLEANUP_BGN
	exit(EXIT_FAILURE);
EC_CLEANUP_END
}
/*[]*/
