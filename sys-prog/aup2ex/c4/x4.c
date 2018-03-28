/*
	Miscellaneous examples for Chap. 4
	AUP2, Chap. 4

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
#include "setblock.h"
#include "../c2/bufio.h"

/* read line from terminal */
/*[getln]*/
bool getln(char *s, ssize_t max, bool *iseof)
{
	ssize_t nread;

	switch (nread = read(STDIN_FILENO, s, max - 1)) {
	case -1:
		EC_FAIL
	case 0:
		*iseof = true;
		return true;
	default:
		if (s[nread - 1] == '\n')
			nread--;
		s[nread] = '\0';
		*iseof = false;
		return true;
	}

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}
/*[]*/

/* read line from anywhere */
/*[getln2]*/
bool getln2(char *s, ssize_t max, bool *iseof)
{
	ssize_t n;
	char c;

	n = 0;
	while (true)
		switch (read(STDIN_FILENO, &c, 1)) {
		case -1:
			EC_FAIL
		case 0:
			s[n] = '\0';
			*iseof = true;
			return true;
		default:
			if (c == '\n') {
				s[n] = '\0';
				*iseof = false;
				return true;
			}
			if (n >= max - 1) {
				errno = E2BIG;
				EC_FAIL
			}
			s[n++] = c;
		}

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}
/*[test_setblock]*/
static void test_setblock(void)
{
	char s[100];
	ssize_t n;
	time_t tstart, tnow;

	ec_neg1( tstart = time(NULL) )
	ec_false( setblock(STDIN_FILENO, false) )
	while (true) {
		ec_neg1( tnow = time(NULL) )
		printf("Waiting for input (%.0f sec.) ...\n",
		  difftime(tnow, tstart));
		switch(n = read(STDIN_FILENO, s, sizeof(s) - 1)) {
		case 0:
			printf("EOF\n");
			break;
		case -1:
			if (errno == EAGAIN) {
				sleep(5);
				continue;
			}
			EC_FAIL
		default:
			if (s[n - 1] == '\n')
				n--;
			s[n] = '\0';
			printf("Read \"%s\"\n", s);
			continue;
		}
		break;
	}
	return;

EC_CLEANUP_BGN
	EC_FLUSH("test_setblock")
EC_CLEANUP_END
}
/*[]*/
static void test_setblock2(void)
{
	char s[100];
	bool iseof;

	ec_false( setblock(STDIN_FILENO, true) )
	while (true) {
		ec_false( getln(s, sizeof(s), &iseof) )
		if (iseof) {
			printf("EOF\n");
			break;
		}
		else
			printf("Read \"%s\"\n", s);
	}
	return;

EC_CLEANUP_BGN
	EC_FLUSH("test_setblock2")
EC_CLEANUP_END
}
/*[]*/
static void test_Bfdopen(void)
{
	BUFIO *stin;
	int c;
/*[Bfdopen-test]*/
	ec_null( stin = Bfdopen(STDIN_FILENO, "r") )
	while ((c = Bgetc(stin)) != -1)
		/* process character */
/*[]*/
		putchar(c);
	if (errno != 0)
		EC_FAIL
	printf("Got EOF\n");
	ec_false( Bclose(stin) )
	return;

EC_CLEANUP_BGN
	(void)Bclose(stin);
	EC_FLUSH("test_Bfdopen")
EC_CLEANUP_END
}
#if 0
#define Sfdopen Sopen
#define refresh exit

void junk(void) /* don't call! */
{
/*4-3*/
	STREAM *stin;
	char c;

	if ((stin = Sfdopen(STDIN_FILENO, "r")) == NULL)
		syserr("Sfdopen");
	c = Sgetc(stin);
/**/

/*4-5*/
	if (!cready())
		refresh();
	c = cget();
/**/
}
#endif

#include <fcntl.h>
/*[cready-cget]*/
#define EMPTY '\0'
static unsigned char cbuf = EMPTY;
typedef enum {CR_READY, CR_NOTREADY, CR_EOF} CR_STATUS;

bool cready(CR_STATUS *statusp)
{
	if (cbuf != EMPTY) {
		*statusp = CR_READY;
		return true;
	}
	setblock(STDIN_FILENO, false);
	switch (read(STDIN_FILENO, &cbuf, 1)) {
	case -1:
		if (errno == EAGAIN) {
			*statusp = CR_NOTREADY;
			return true;
		}
		EC_FAIL
	case 0:
		*statusp = CR_EOF;
		return true;
	case 1:
		return true;
	default: /* "impossible" case */
		errno = 0;
		EC_FAIL
	}

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}

bool cget(CR_STATUS *statusp, int *cp)
{
	if (cbuf != EMPTY) {
		*cp = cbuf;
		cbuf = EMPTY;
		*statusp = CR_READY;
		return true;
	}
	setblock(0, true);
	switch (read(STDIN_FILENO, cp, 1)) {
	case -1:
		EC_FAIL
	case 0:
		*cp = 0;
		*statusp = CR_EOF;
		return true;
	case 1:
		*statusp = CR_READY;
		return true;
	default: /* "impossible" case */
		errno = 0;
		EC_FAIL
	}

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}
/*[]*/


/*[readany]*/
int readany(int fds[], int nfds, int *whichp)
{
	int i;
	unsigned char c;

	for (i = 0; i < nfds; i++)
		setblock(fds[i], false); /* inefficient to do this every time */
	i = 0;
	while (true) {
		if (i >= nfds) {
			sleep(1);
			i = 0;
		}
		c = 0; /* return value for EOF */
		if (read(fds[i], &c, 1) == -1) {
			if (errno == EAGAIN) {
				i++;
				continue;
			}
			EC_FAIL
		}
		*whichp = i;
		return c;
	}

EC_CLEANUP_BGN
	return -1;
EC_CLEANUP_END
}
/*[readany2]*/
int readany2(int fds[], int nfds, int *whichp)
{
	fd_set set_read;
	int i, maxfd = 0;
	unsigned char c;

	FD_ZERO(&set_read);
	for (i = 0; i < nfds; i++) {
		FD_SET(fds[i], &set_read);
		if (fds[i] > maxfd)
			maxfd = fds[i];
	}
	ec_neg1( select(maxfd + 1, &set_read, NULL, NULL, NULL) )
	for (i = 0; i < nfds; i++) {
		if (FD_ISSET(fds[i], &set_read)) {
			c = 0; /* return value for EOF */
			ec_neg1( read(fds[i], &c, 1) )
			*whichp = i;
			return c;
		}
	}
	/* "impossible" to get here */
	errno = 0;
	EC_FAIL

EC_CLEANUP_BGN
	return -1;
EC_CLEANUP_END
}
#ifndef DARWIN
/*[readany3]*/
#include <poll.h>

#define MAXFDS 100

int readany3(int fds[], int nfds, int *whichp)
{
	struct pollfd fdinfo[MAXFDS] = { { 0 } };
	int i;
	unsigned char c;

	if (nfds > MAXFDS) {
		errno = E2BIG;
		EC_FAIL
	}
	for (i = 0; i < nfds; i++) {
		fdinfo[i].fd = fds[i];
		fdinfo[i].events = POLLIN | POLLPRI;
	}
	ec_neg1( poll(fdinfo, nfds, -1) )
	for (i = 0; i < nfds; i++) {
		if (fdinfo[i].revents & (POLLIN | POLLPRI)) {
			c = 0; /* return value for EOF */
			ec_neg1( read(fdinfo[i].fd, &c, 1) )
			*whichp = i;
			return c;
		}
	}
	/* "impossible" to get here */
	errno = 0;
	EC_FAIL

EC_CLEANUP_BGN
	return -1;
EC_CLEANUP_END
}
/*[]*/
#endif /* DARWIN */
/*[readany_test]*/
static void readany_test(void)
{
	int fds[2] = {-1, -1}, which;
	int c;
	bool ok = false;

	ec_neg1( fds[0] = open("/dev/tty", O_RDWR) )
	ec_neg1( fds[1] = open("/dev/pts/3", O_RDWR) )
	while ((c = readany(fds, 2, &which)) > 0)
		printf("Got %c from terminal %d\n", isprint(c) ? c : '?', which);
	ec_neg1( c )
	ok = true;
	EC_CLEANUP

EC_CLEANUP_BGN
	if (fds[0] != -1)
		(void)close(fds[0]);
	if (fds[1] != -1)
		(void)close(fds[1]);
	if (!ok)
		EC_FLUSH("readany_test1")
EC_CLEANUP_END
}
/*[]*/
static void readany_test2(void)
{
	int fds[2] = {-1, -1}, which;
	int c;
	bool ok = false;

	ec_neg1( fds[0] = open("/dev/tty", O_RDWR) )
	ec_neg1( fds[1] = open("/dev/pts/3", O_RDWR) )
	while ((c = readany2(fds, 2, &which)) > 0)
		printf("Got %c from terminal %d\n", isprint(c) ? c : '?', which);
	ec_neg1( c )
	ok = true;
	EC_CLEANUP

EC_CLEANUP_BGN
	if (fds[0] != -1)
		(void)close(fds[0]);
	if (fds[1] != -1)
		(void)close(fds[1]);
	if (!ok)
		EC_FLUSH("readany_test2")
EC_CLEANUP_END
}
#ifndef DARWIN
static void readany_test3(void)
{
	int fds[2] = {-1, -1}, which;
	int c;
	bool ok = false;

	ec_neg1( fds[0] = open("/dev/tty", O_RDWR) )
	ec_neg1( fds[1] = open("/dev/pts/3", O_RDWR) )
	while ((c = readany3(fds, 2, &which)) > 0)
		printf("Got %c from terminal %d\n", isprint(c) ? c : '?', which);
	ec_neg1( c )
	ok = true;
	EC_CLEANUP

EC_CLEANUP_BGN
	if (fds[0] != -1)
		(void)close(fds[0]);
	if (fds[1] != -1)
		(void)close(fds[1]);
	if (!ok)
		EC_FLUSH("readany_test3")
EC_CLEANUP_END
}
#endif /* DARWIN */
int main(void)
{
	char s[100];
#if 1
	int c;
	CR_STATUS status;
	int count = 0;
	while (true) {
		ec_false( cready(&status) )
		switch (status) {
		case CR_READY:
			ec_false( cget(&status, &c) )
			if (status == CR_EOF) {
				printf("EOF from cget\n");
				setblock(STDIN_FILENO, true);
				exit(EXIT_SUCCESS);
			}
			printf("After %d cready() calls, got %c\n", count, c);
			count = 0;
		case CR_NOTREADY:
			count++;
			continue;
		case CR_EOF:
			printf("EOF from cready\n");
			setblock(STDIN_FILENO, true);
			exit(EXIT_SUCCESS);
		}
	}
#if 0 /* AUP1 -- not sure what this is doing */
		if (cready())
			printf("READY\n");
		else
			printf("NOT READY\n");
		if ((c = cget()) == -1) {
			printf("EOF\n");
			exit(EXIT_SUCCESS);
		}
		if (c == 1) {
			setblock(0, true);
			exit(EXIT_SUCCESS);
		}
		printf("%c", c);
#endif

#endif

/*[refresh-example]*/
ec_false( cready(&status) )
if (status == CR_NOTREADY)
	refresh();
ec_false( cget(&status, &c) )
/*[]*/



#if 0
{
	bool iseof;

	setbuf(stdout, NULL);

{
	int fd1 = 1, fd2 = 4;

/*[fd_set-example]*/
fd_set set;

FD_ZERO(&set);
FD_SET(fd1, &set);
FD_SET(fd2, &set);
if (FD_ISSET(fd1, &set)) {
	/* do something with fd1, depending on which set */
}
/*[]*/
}
	//test_Bfdopen();
	//test_setblock();
	//test_setblock2();
	readany_test2();
exit(EXIT_SUCCESS);

	while (1) {
		printf("Type (getln): ");
/*[getln-test]*/
ec_false( getln(s, sizeof(s), &iseof) )
if (iseof)
	printf("EOF\n");
else
	printf("Read: %s\n", s);
/*[]*/
		if (strcmp(s, "quit") == 0)
			break;
		printf("Type (getln2): ");
/*[getln2-test]*/
ec_false( getln2(s, sizeof(s), &iseof) )
if (iseof)
	printf("EOF: %s\n", s);
else
	printf("Read: %s\n", s);
/*[]*/
		if (strcmp(s, "quit") == 0)
			break;
	}
	exit(EXIT_SUCCESS);

}
#endif
EC_CLEANUP_BGN
	setblock(STDIN_FILENO, true);
	exit(EXIT_FAILURE);
EC_CLEANUP_END
}
