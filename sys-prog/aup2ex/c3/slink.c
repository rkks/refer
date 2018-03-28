/*
	Symbolic-link test program (uses stat and lstat)
	AUP2, Sec. 3.5.1 (not in book)

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

const char *get_file_type(mode_t mode)
{
	if (S_ISBLK(mode))
		return "block special file";
	if (S_ISCHR(mode))
		return "character special file";
	if (S_ISDIR(mode))
		return "directory";
	if (S_ISFIFO(mode))
		return "pipe or FIFO special file";
	if (S_ISREG(mode))
		return "regular file";
	if (S_ISLNK(mode))
		return "symbolic link";
	if (S_ISSOCK(mode))
		return "socket";
	return "unknown type";
}

int main(void)
{
	struct stat sbuf;

	(void)unlink("a.tmp");
	(void)creat("b.tmp", PERM_FILE);

	if (symlink("b.tmp", "a.tmp") == -1)
		syserr("symlink");
	if (link("b.tmp", "c.tmp") == -1)
		syserr("link");

	if (stat("b.tmp", &sbuf) == -1)
		syserr("stat 1");
	printf("stat(\"b.tmp\"):\n");
	printf("\tst_ino = %u\n", (unsigned)sbuf.st_ino);
	printf("\tst_nlink = %u\n", (unsigned)sbuf.st_nlink);
	printf("\tst_mode = 0%o\n", (unsigned)sbuf.st_mode);
	printf("\tfile type: %s\n", get_file_type(sbuf.st_mode));

	if (stat("a.tmp", &sbuf) == -1)
		syserr("stat 2");
	printf("\nstat(\"a.tmp\"):\n");
	printf("\tst_ino = %u\n", (unsigned)sbuf.st_ino);
	printf("\tst_nlink = %u\n", (unsigned)sbuf.st_nlink);
	printf("\tst_mode = 0%o\n", (unsigned)sbuf.st_mode);
	printf("\tfile type: %s\n", get_file_type(sbuf.st_mode));

	if (lstat("a.tmp", &sbuf) == -1)
		syserr("lstat 1");
	printf("\nlstat(\"a.tmp\"):\n");
	printf("\tst_ino = %u\n", (unsigned)sbuf.st_ino);
	printf("\tst_nlink = %u\n", (unsigned)sbuf.st_nlink);
	printf("\tst_mode = 0%o\n", (unsigned)sbuf.st_mode);
	printf("\tfile type: %s\n", get_file_type(sbuf.st_mode));

	if (stat("c.tmp", &sbuf) == -1)
		syserr("stat 3");
	printf("\nstat(\"c.tmp\"):\n");
	printf("\tst_ino = %u\n", (unsigned)sbuf.st_ino);
	printf("\tst_nlink = %u\n", (unsigned)sbuf.st_nlink);
	printf("\tst_mode = 0%o\n", (unsigned)sbuf.st_mode);
	printf("\tfile type: %s\n", get_file_type(sbuf.st_mode));

	if (lstat("c.tmp", &sbuf) == -1)
		syserr("lstat 2");
	printf("\nlstat(\"c.tmp\"):\n");
	printf("\tst_ino = %u\n", (unsigned)sbuf.st_ino);
	printf("\tst_nlink = %u\n", (unsigned)sbuf.st_nlink);
	printf("\tst_mode = 0%o\n", (unsigned)sbuf.st_mode);
	printf("\tfile type: %s\n", get_file_type(sbuf.st_mode));

	exit(EXIT_SUCCESS);
}
