/*
	Functions for User Buffering
	AUP2, Sec. 2.12.2

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
#ifndef _BUFIO_H_
#define _BUFIO_H_

/*[bufio-h]*/
typedef struct {
	int fd;						/* file descriptor */
	char dir;					/* direction: r or w */
	ssize_t total;				/* total chars in buf */
	ssize_t next;				/* next char in buf */
	unsigned char buf[BUFSIZ];	/* buffer */
	bool fdopen;				/* opened with Bfdopen? [not in book] */
	/* Following members not in book. */
	struct timeval timeout;		/* optional timeout (causes select to be used) */
} BUFIO;
/*[]*/

BUFIO *Bopen(const char *path, const char *dir);
BUFIO *Bfdopen(int fd, const char *dir);
int Bgetc(BUFIO *b);
bool Bputc(BUFIO *b, int c);
bool Bclose(BUFIO *b);

/*
	Rest of functions not in book.
*/
bool Bgets(BUFIO *b, char *s, size_t smax);
void Bsettimeout(BUFIO *b, struct timeval *timeout);
#endif /* _BUFIO_H_ */
