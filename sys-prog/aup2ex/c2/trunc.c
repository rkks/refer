/*
	ftruncate test program
	AUP2, Sec. 2.17 (not in book)

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

#define TEXTLINE "This is a line.\n"

static int fd = -1;

void buildfile(void)
{
	int i;

	/* Build original file. */
	if ((fd = open("a.tmp", O_RDWR | O_CREAT | O_TRUNC, PERM_FILE)) == -1)
		syserr("open");
	for (i = 0; i < 5; i++)
		if (write(fd, TEXTLINE, strlen(TEXTLINE)) == -1)
			syserr("write");
}

int main(void)
{
	off_t pos_in, pos_out;
	char c;

	buildfile();
	printf("Original file:\n");
	system("cat a.tmp");

	pos_in = pos_out = 0;

	while (true) {
		if (lseek(fd, pos_in, SEEK_SET) == -1)
			syserr("lseek to pos_in");
		if (read(fd, &c, 1) != 1)
			break;
		pos_in++;
		if (c != ' ') {
			if (lseek(fd, pos_out, SEEK_SET) == -1)
				syserr("lseek to pos_out");
			if (write(fd, &c, 1) == -1)
				syserr("write at pos_out");
			pos_out++;
		}
	}
	printf("\nSqueezed file, not truncated:\n");
	system("cat a.tmp");

	if (ftruncate(fd, pos_out) == -1)
		syserr("ftruncate");
	printf("\nSqueezed file, truncated:\n");
	system("cat a.tmp");

	if (close(fd) == -1)
		syserr("close");
	exit(EXIT_SUCCESS);
}
