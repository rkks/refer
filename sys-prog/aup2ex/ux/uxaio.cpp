/*
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
#include "ux.hpp"

using namespace Ux;

/**
	Calls aio_cancel.
*/
int Aio::cancel(int fd)
{
#if _POSIX_ASYNCHRONOUS_IO > 0
	int r;

	if ((r = ::aio_cancel(fd, this)) == -1)
		throw Error(errno);
	return r;
#else
	throw Error(ENOSYS);
#endif
}

/**
	Calls aio_error.
*/
int Aio::error(void)
{
#if _POSIX_ASYNCHRONOUS_IO > 0
	return ::aio_error(this);
#else
	throw Error(ENOSYS);
#endif
}

/**
	Calls aio_fsync.
*/
void Aio::sync(int op)
{
#if _POSIX_ASYNCHRONOUS_IO > 0
	if (::aio_fsync(op, this) == -1)
		throw Error(errno);
#else
	throw Error(ENOSYS);
#endif
}

/**
	Calls aio_read.
*/
void Aio::read(void)
{
#if _POSIX_ASYNCHRONOUS_IO > 0
	if (::aio_read(this) == -1)
		throw Error(errno);
#else
	throw Error(ENOSYS);
#endif
}

/**
	Calls aio_return.
*/
ssize_t Aio::return_status(void)
{
#if _POSIX_ASYNCHRONOUS_IO > 0
	ssize_t r;

	if ((r = ::aio_return(this)) == -1)
		throw Error(errno);
	return r;
#else
	throw Error(ENOSYS);
#endif
}

/**
	Calls aio_suspend.
*/
/* static */ void Aio::suspend(const struct aiocb *const list[], int cbcnt,
  const struct timespec *timeout)
{
#if _POSIX_ASYNCHRONOUS_IO > 0
	if (::aio_suspend(list, cbcnt, timeout) == -1)
		throw Error(errno);
#else
	throw Error(ENOSYS);
#endif
}

/**
	Calls aio_write.
*/
void Aio::write(void)
{
#if _POSIX_ASYNCHRONOUS_IO > 0
	if (::aio_write(this) == -1)
		throw Error(errno);
#else
	throw Error(ENOSYS);
#endif
}

/**
	Calls lio_listio.
*/
/* static */ void Aio::listio(int mode, struct aiocb *const list[],
  int cbcnt, struct sigevent *sig)
{
#if _POSIX_ASYNCHRONOUS_IO > 0
	if (::lio_listio(mode, list, cbcnt, sig) == -1)
		throw Error(errno);
#else
	throw Error(ENOSYS);
#endif
}
