/*
	Display i-node
	AUP2, Sec. 3.2.2

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
#ifndef FREEBSD
#include "defs.h"

int main(void)
{
	printf("Program is for FreeBSD only.\n");
	exit(EXIT_FAILURE);
}
#else

/*[inode]*/
#include "defs.h"
#include <sys/param.h>
#include <ufs/ffs/fs.h>
#include <ufs/ufs/dinode.h>

#define DEVICE "/dev/ad0s1g"

int main(int argc, char *argv[])
{
	int fd;
	long inumber;
	char sb_buf[((sizeof(struct fs) / DEV_BSIZE) + 1) * DEV_BSIZE];
	struct fs *superblock = (struct fs *)sb_buf;
	struct dinode *d;
	ssize_t nread;
	off_t fsbo, fsba;
	char *inode_buf;
	size_t inode_buf_size;

	if (argc < 2) {
		printf("(AUP) Usage: inode n\n");
		exit(EXIT_FAILURE);
	}
	inumber = atol(argv[1]);

	ec_neg1( fd = open(DEVICE, O_RDONLY) )
	ec_neg1( lseek(fd, SBLOCK * DEV_BSIZE, SEEK_SET) )
	switch (nread = read(fd, sb_buf, sizeof(sb_buf))) {
	case 0:
		errno = 0;
		printf("EOF from read (1)\n");
		EC_FAIL
	case -1:
		EC_FAIL
	default:
		if (nread != sizeof(sb_buf)) {
			errno = 0;
			printf("Read only %d bytes instead of %d\n", nread, sizeof(sb_buf));
			EC_FAIL
		}
	}
	printf("Superblock info for %s:\n", DEVICE);
	printf("\tlast time written = %s", ctime(&superblock->fs_time));
	printf("\tnumber of blocks in fs = %ld\n", (long)superblock->fs_size);
	printf("\tnumber of data blocks in fs = %ld\n", (long)superblock->fs_dsize);
	printf("\tsize of basic blocks in fs = %ld\n", (long)superblock->fs_bsize);
	printf("\tsize of frag blocks in fs = %ld\n", (long)superblock->fs_fsize);
	printf("\tname mounted on = %s\n", superblock->fs_fsmnt);

	inode_buf_size = superblock->fs_bsize;
	ec_null( inode_buf = malloc(inode_buf_size) )

	fsba = ino_to_fsba(superblock, inumber);
	fsbo = ino_to_fsbo(superblock, inumber);

	ec_neg1( lseek(fd, fsbtodb(superblock, fsba) * DEV_BSIZE, SEEK_SET) )
	switch (nread = read(fd, inode_buf, inode_buf_size)) {
	case 0:
		errno = 0;
		printf("EOF from read (2)\n");
		EC_FAIL
	case -1:
		EC_FAIL
	default:
		if (nread != inode_buf_size) {
			errno = 0;
			printf("Read only %d bytes instead of %d\n", nread, inode_buf_size);
			EC_FAIL
		}
	}
	d = (struct dinode *)&inode_buf[fsbo * sizeof(struct dinode)];
	printf("\ninumber %ld info:\n", inumber);
	printf("\tmode = 0%o\n", d->di_mode);
	printf("\tlinks = %d\n", d->di_nlink);
	printf("\towner = %d\n", d->di_uid);
	printf("\tmod. time = %s", ctime((time_t *)&d->di_mtime));
	exit(EXIT_SUCCESS);

EC_CLEANUP_BGN
	exit(EXIT_FAILURE);
EC_CLEANUP_END
}
/*[]*/
#endif /* FREEBSD */
