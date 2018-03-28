/*
	writev test program
	AUP2, Sec. 2.15 (not in book)

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
#include <sys/uio.h>

#define REPS 50000
#define PIECES 16
#define PIECE_SIZE 200

int main(void)
{
	int i, fd, count, /*fillerlen,*/ bufsize, iovcnt;
	ssize_t n;
	char *piece[PIECES];
#if 0
	struct data1 {
		int num;
		char s[20];
	} d1, d3;
	struct data2 {
		int num;
		char s[50];
	} d2;
	char *buf, *filler;
#endif
	struct iovec v[PIECES];

#ifdef BSD_DERIVED
#define IOV_MAX 1024 /* sys/uio.h defines UIO_MAXIOV, but only if _KERNEL is defined */
#endif
	iovcnt = PIECES;
	if (iovcnt > IOV_MAX)
		iovcnt = IOV_MAX;
	for (i = 0; i < iovcnt; i++) {
		ec_null( piece[i] = malloc(PIECE_SIZE) )
		memset(piece[i], 123, PIECE_SIZE);
		v[i].iov_base = piece[i];
		v[i].iov_len = PIECE_SIZE;
	}
	printf("iovcnt = %d; piece size = %d; reps = %d\n",
	  iovcnt, PIECE_SIZE, REPS);

	printf("IOV_MAX = %ld; SSIZE_MAX = %ld\n",
	  (long)IOV_MAX, (long)SSIZE_MAX);

	printf("sizeof(int) = %d;BUFSIZ = %d\n",
	  sizeof(int), BUFSIZ);

	ec_neg1( fd = open("a.tmp", O_RDWR | O_CREAT | O_TRUNC, PERM_FILE) )
/*
	We would like a universal way to determine the best buffer size. _PC_ALLOC_SIZE_MIN
	(assuming that's even the right symbol) is optional, so not universal.
*/
	bufsize = 1024;
/*
#ifdef FREEBSD
	bufsize = 4096;
#else
	bufsize = fpathconf(fd, _PC_ALLOC_SIZE_MIN);
	printf("_PC_ALLOC_SIZE_MIN = %d\n", bufsize);
#endif
*/

#if 0
	ec_null( filler = malloc(bufsize) )
	ec_null( buf = malloc(bufsize) )

	d1.num = 123;
	strcpy(d1.s, "hello");
	d2.num = 456;
	strcpy(d2.s, "goodbye");
	d3.num = 789;
	strcpy(d3.s, "thankyou");
	memset(filler, 0, bufsize);
	fillerlen = bufsize - sizeof(d1) - sizeof(d2) - sizeof(d3);

	v[0].iov_base = &d1;
	v[0].iov_len = sizeof(d1);
	v[1].iov_base = &d2;
	v[1].iov_len = sizeof(d2);
	v[2].iov_base = &d3;
	v[2].iov_len = sizeof(d3);
	v[3].iov_base = filler;
	v[3].iov_len = fillerlen;
	ec_neg1( n = writev(fd, v, sizeof(v) / sizeof(v[0])) )
	printf("%ld bytes written\n", (long)n);

	memset(&d1, 0, sizeof(d1));
	memset(&d2, 0, sizeof(d2));
	memset(&d3, 0, sizeof(d3));

	if (lseek(fd, 0, SEEK_SET) == -1)
		syserr("lseek");

	ec_neg1( n = readv(fd, v, sizeof(v) / sizeof(v[0])) )
	printf("%ld bytes read\n", (long)n);

	printf("d1: %d %s\n", d1.num, d1.s);
	printf("d2: %d %s\n", d2.num, d2.s);
	printf("d3: %d %s\n", d3.num, d3.s);
#endif
	/* Now do some timing tests. */

	ec_neg1( lseek(fd, 0, SEEK_SET) )
	timestart();
	for (count = 0; count < REPS; count++)
		ec_neg1( n = writev(fd, v, iovcnt) )
	timestop("writev");
	printf("Position at end = %ld\n", (long)lseek(fd, 0, SEEK_CUR));

	ec_neg1( lseek(fd, 0, SEEK_SET) )
	timestart();
	for (count = 0; count < REPS; count++)
		for (i = 0; i < iovcnt; i++)
			ec_neg1( n = write(fd, v[i].iov_base, PIECE_SIZE) )
	timestop("write");
	printf("Position at end = %ld\n", (long)lseek(fd, 0, SEEK_CUR));
#if 0
	ec_neg1( lseek(fd, 0, SEEK_SET) )
	timestart();
	for (count = 0; count < REPS; count++) {
		int offset = 0;
		memcpy(buf, &d1, sizeof(d1));
		offset += sizeof(d1);
		memcpy(buf + offset, &d2, sizeof(d2));
		offset += sizeof(d2);
		memcpy(buf + offset, &d3, sizeof(d3));
		offset += sizeof(d3);
		assert(fillerlen == bufsize - offset);
		memcpy(buf + offset, filler, fillerlen);
		offset += fillerlen;
		assert(offset == bufsize);
		ec_neg1( n = write(fd, buf, bufsize) )
	}
	timestop("write (full buffer)");
	printf("Position at end = %ld\n", lseek(fd, 0, SEEK_CUR));
#endif
	ec_neg1( close(fd) )
	exit(EXIT_SUCCESS);

EC_CLEANUP_BGN
	exit(EXIT_FAILURE);
EC_CLEANUP_END
}
