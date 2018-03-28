/*
	Synchronized I/O test program
	AUP2, Sec. 2.16.2 (not in book)

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

#define NUM_WRITES 250

int main(void)
{
	int try, fd, i;
	char buf[512], *caption;

#ifdef _POSIX_SYNCHRONIZED_IO
	printf("_POSIX_SYNCHRONIZED_IO is defined\n");
#else
	printf("_POSIX_SYNCHRONIZED_IO is undefined\n");
#endif

	memset(buf, 0, sizeof(buf));

	for (try = 0; try < 4; try++) {
		if ((fd = creat("a.tmp", PERM_FILE)) == -1)
			syserr("creat");
		timestart();
		for (i = 0; i < NUM_WRITES; i++) {
			if (write(fd, buf, sizeof(buf)) == -1)
				syserr("write");
			switch (try) {
			case 0:
				caption = "normal";
				break;
			case 1:
				sync();
				caption = "sync";
				break;
			case 2:
#if defined(_POSIX_SYNCHRONIZED_IO) || defined(FREEBSD)
				fsync(fd);
				caption = "fsync";
#else
				sync();
				caption = "fsync not defined; using sync";
#endif
				break;
			case 3:
#if defined(_POSIX_SYNCHRONIZED_IO)
				fdatasync(fd);
				caption = "fdatasync";
#else
				sync();
				caption = "fdatasync not defined; using sync";
#endif
			}
		}
		timestop(caption);
		if (close(fd) == -1)
			syserr("close");
	}

	exit(EXIT_SUCCESS);
}
