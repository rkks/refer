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
#include "defs.h"
#include "bufio.h"

static BUFIO *Bopen_internal(int fd, const char *path, const char *dir)
{
	BUFIO *b = NULL;
	int flags;

	switch (dir[0]) {
	case 'r':
		flags = O_RDONLY;
		break;
	case 'w':
		flags = O_WRONLY | O_CREAT | O_TRUNC;
		break;
	default:
		errno = EINVAL;
		EC_FAIL
	}
	ec_null( b = calloc(1, sizeof(BUFIO)) )
	b->fdopen = path == NULL;
	if (b->fdopen)
		b->fd = fd;
	else
		b->fd = open(path, flags, PERM_FILE);
	ec_neg1( b->fd )
	b->dir = dir[0];
	return b;

EC_CLEANUP_BGN
	free(b);
	return NULL;
EC_CLEANUP_END
}

BUFIO *Bfdopen(int fd, const char *dir)
{
	return Bopen_internal(fd, NULL, dir);
}

/*[bufio.c]*/
#if 0 /* non-Bfdopen version, as appeared in Chap. 2 of book */
BUFIO *Bopen(const char *path, const char *dir)
{
	BUFIO *b = NULL;
	int flags;

	switch (dir[0]) {
	case 'r':
		flags = O_RDONLY;
		break;
	case 'w':
		flags = O_WRONLY | O_CREAT | O_TRUNC;
		break;
	default:
		errno = EINVAL;
		EC_FAIL
	}
	ec_null( b = calloc(1, sizeof(BUFIO)) )
	ec_neg1( b->fd = open(path, flags, PERM_FILE) )
	b->dir = dir[0];
	return b;

EC_CLEANUP_BGN
	free(b);
	return NULL;
EC_CLEANUP_END
}
#else
BUFIO *Bopen(const char *path, const char *dir)
{
	return Bopen_internal(-1, path, dir);
}
#endif

static bool readbuf(BUFIO *b)
{
	if (b->timeout.tv_sec != 0 || b->timeout.tv_usec != 0) {
		fd_set set;

		FD_ZERO(&set);
		FD_SET(b->fd, &set);
		ec_neg1( select(b->fd + 1, &set, NULL, NULL, &b->timeout) )
		if (!FD_ISSET(b->fd, &set)) {
			errno = ETIMEDOUT;
			return false;
		}
	}
	ec_neg1( b->total = read(b->fd, b->buf, sizeof(b->buf)) )
	if (b->total == 0) {
		errno = 0;
		return false;
	}
	b->next = 0;
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}

static bool writebuf(BUFIO *b)  /* flush buffer */
{
	ssize_t n, total;

	total = 0;
	while (total < b->next) {
		ec_neg1( n = write(b->fd, &b->buf[total], b->next - total) )
		total += n;
	}
	b->next = 0;
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}

int Bgetc(BUFIO *b)
{
	if (b->next >= b->total)
		if (!readbuf(b)) {
			if (errno == 0)
				return -1;
			EC_FAIL
		}
	return b->buf[b->next++];

EC_CLEANUP_BGN
	return -1;
EC_CLEANUP_END
}

bool Bputc(BUFIO *b, int c)
{
	b->buf[b->next++] = c;
	if (b->next >= sizeof(b->buf))
		ec_false( writebuf(b) )
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}

bool Bclose(BUFIO *b)
{
	if (b != NULL) {
		if (b->dir == 'w')
			ec_false( writebuf(b) )
		if (!b->fdopen) /* this line not in book */
			ec_neg1( close(b->fd) )
		free(b);
	}
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}
/*[]*/

/*
	Rest of functions not in book.
*/
bool Bgets(BUFIO *b, char *s, size_t smax)
{
	int c, i;
	bool ok = true;

	for (i = 0; i < smax - 1; i++) {
		switch (c = Bgetc(b)) {
		case '\n':
			break;
		case -1:
			ok = i > 0;
			break;
		default:
			s[i] = (char)c;
			continue;
		}
		break;
	}
	s[i] = '\0';
	return ok;
}

void Bsettimeout(BUFIO *b, struct timeval *timeout)
{
	b->timeout = *timeout;
}
