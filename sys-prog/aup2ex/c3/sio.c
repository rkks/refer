/*
	sio (used with feed)
	AUP2, Sec. 3.9.8

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

/*
	FREEBSD seems to support AIO, even though it doesn't claim to with _POSIX_ASYNCHRONOUS_IO. We don't test that symbol so we can run this example on FREEBSD. DARWIN doesn't support it.
*/
#ifdef DARWIN
int main(void)
{
	printf("DARWIN doesn't support AIO.\n");
	exit(EXIT_SUCCESS);
}
#else /* DARWIN */
#include <aio.h>
/*[synchronous]*/
#define PATH "/aup/c3/datafile.txt"
#define FREQ 8000

static void synchronous(void)
{
	int fd, count = 0;
	ssize_t nread;
	char buf1[512], buf2[512];

	ec_neg1( fd = open(PATH, O_RDONLY) )
	timestart();
	while (true) {
		ec_neg1( nread = read(fd, buf1, sizeof(buf1)) )
		if (nread == 0)
			break;
		if (count % FREQ == 0)
			ec_neg1( read(STDIN_FILENO, buf2, sizeof(buf2)) )
		count++;
	}
	timestop("synchronous");
	printf("read %d blocks\n", count);
	return;

EC_CLEANUP_BGN
	EC_FLUSH("synchronous")
EC_CLEANUP_END
}
/*[asynchronous]*/
static void asynchronous(void)
{
	int fd, count = 0;
	ssize_t nread;
	char buf1[512], buf2[512];
	struct aiocb cb;
	const struct aiocb *list[1] = { &cb };

	memset(&cb, 0, sizeof(cb));
	cb.aio_fildes = STDIN_FILENO;
	cb.aio_buf = buf2;
	cb.aio_nbytes = sizeof(buf2);
	cb.aio_sigevent.sigev_notify = SIGEV_NONE;
	ec_neg1( fd = open(PATH, O_RDONLY) )
	timestart();
	while (true) {
		ec_neg1( nread = read(fd, buf1, sizeof(buf1)) )
		if (nread == 0)
			break;
		if (count % FREQ == 0) {
			if (count > 1) {
				ec_neg1( aio_suspend(list, 1, NULL) )
				ec_rv( aio_error(&cb) )
			}
			ec_neg1( aio_read(&cb) )
		}
		count++;
	}
	timestop("asynchronous");
	printf("read %d blocks\n", count);
	return;

EC_CLEANUP_BGN
	EC_FLUSH("asynchronous")
EC_CLEANUP_END
}
/*[]*/
int main(void)
{
	synchronous();
	asynchronous();
	exit(EXIT_SUCCESS);
}

#endif /* DARWIN */
