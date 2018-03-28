/*
	Error handling examples
	AUP2, Sec. 1.4

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
#include <netdb.h>
#include <errno.h>

static bool fcn(void)
{
	char buf[100], filein[20], fileout[20];
/*[cleanup3]*/
	char *p = NULL;
	int fdin = -1, fdout = -1;

	ec_null( p = malloc(sizeof(buf)) )
	ec_neg1( fdin = open(filein, O_RDONLY) )
	ec_neg1( fdout = open(fileout, O_WRONLY) )

	return true;

EC_CLEANUP_BGN
	free(p);
	if (fdin != -1)
		(void)close(fdin);
	if (fdout != -1)
		(void)close(fdout);
	return false;
EC_CLEANUP_END
/*[]*/
}

int main(void)
{

{
	int amt, fd = -1,  numbyte = 10, r;
	char buf[100], filein[20], fileout[20];
	char *p = NULL;
	int fdin = -1, fdout = -1;

/*[syserrmsg]*/
if ((amt = read(fd, buf, numbyte)) == -1) {
	fprintf(stderr, "%s\n", syserrmsg(buf, sizeof(buf),
	  "Call to read function failed", errno));
	exit(EXIT_FAILURE);
}
/*[]*/
/*[cleanup3call]*/
	ec_false( fcn() )

	/* other stuff here */

	exit(EXIT_SUCCESS);

EC_CLEANUP_BGN
	exit(EXIT_FAILURE);
EC_CLEANUP_END
/*[]*/
{
/*[cleanup2]*/
	char *p = NULL;
	int fdin = -1, fdout = -1;

	if ((p = malloc(sizeof(buf))) == NULL) {
		fprintf(stderr, "%s\n", syserrmsg(buf, sizeof(buf),
		  "malloc failed", errno));
		goto cleanup;
	}
	if ((fdin = open(filein, O_RDONLY)) == -1) {
		fprintf(stderr, "%s\n", syserrmsg(buf, sizeof(buf),
		  "open (input) failed", errno));
		goto cleanup;
	}
	if ((fdout = open(fileout, O_WRONLY)) == -1) {
		fprintf(stderr, "%s\n", syserrmsg(buf, sizeof(buf),
		  "open (output) failed", errno));
		goto cleanup;
	}
	return true;

cleanup:
	free(p);
	if (fdin != -1)
		(void)close(fdin);
	if (fdout != -1)
		(void)close(fdout);
	return false;
/*[cleanup1]*/
}
	if ((p = malloc(sizeof(buf))) == NULL) {
		fprintf(stderr, "%s\n", syserrmsg(buf, sizeof(buf),
		  "malloc failed", errno));
		return false;
	}
	if ((fdin = open(filein, O_RDONLY)) == -1) {
		fprintf(stderr, "%s\n", syserrmsg(buf, sizeof(buf),
		  "open (input) failed", errno));
		free(p);
		return false;
	}
	if ((fdout = open(fileout, O_WRONLY)) == -1) {
		fprintf(stderr, "%s\n", syserrmsg(buf, sizeof(buf),
		  "open (output) failed", errno));
		(void)close(fdin);
		free(p);
		return false;
	}
/*[]*/
{
/*[getaddrinfo]*/
struct addrinfo *infop;

if ((r = getaddrinfo("localhost", "80", NULL, &infop)) != 0) {
	fprintf(stderr, "Got error code %d from getaddrinfo\n", r);
	exit(EXIT_FAILURE);
}
/*[read-errnowrong1]*/
}
amt = read(fd, buf, numbyte);
if (errno != 0) { /* wrong! */
	fprintf(stderr, "Read failed! errno = %d\n", errno);
	exit(EXIT_FAILURE);
}
/*[]*/
/*[read-errnowrong2]*/
errno = 0;
amt = read(fd, buf, numbyte);
if (errno != 0) { /* wrong! */
	fprintf(stderr, "Read failed! errno = %d\n", errno);
	exit(EXIT_FAILURE);
}
/*[]*/
/*[read-errsymbol]*/
if ((amt = read(fd, buf, numbyte)) == -1) {
	fprintf(stderr, "Read failed!: %s (errno = %d; %s)\n",
	  strerror(errno), errno, errsymbol(errno));
	exit(EXIT_FAILURE);
}
/*[]*/
/*[read-perror]*/
if ((amt = read(fd, buf, numbyte)) == -1) {
	perror("Read failed!");
	exit(EXIT_FAILURE);
}
/*[]*/
#if 0
/*[read-errno]*/
#include <errno.h>

if ((amt = read(fd, buf, numbyte)) == -1) {
	fprintf(stderr, "Read failed! errno = %d\n", errno);
	exit(EXIT_FAILURE);
}
/*[]*/
#endif
/*[read1]*/
if ((amt = read(fd, buf, numbyte)) == -1) {
	fprintf(stderr, "Read failed!\n");
	exit(EXIT_FAILURE);
}
/*[]*/
}
		exit(EXIT_SUCCESS);

}
