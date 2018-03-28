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
	Calls malloc to allocate the path.
*/
void File::alloc(ssize_t n)
{
	if (n == -1)
		size = get_max_name();
	else
		size = n;
	// use malloc instead of new so errno will be set
	if ((path = (char *)::malloc(size)) == NULL)
		throw Error(errno);
}

/**
	Calls free to free the path.
*/
void File::free(void)
{
	::free((void *)path);
	path = NULL;
	size = -1;
}

/**
	Calls pathconf to get _PC_PATH_MAX.
*/
size_t File::get_max_name(const char *p)
{
	return (size_t)File(p).pathconf(_PC_PATH_MAX, 4096);
}

////////////////////////////////////////////////

/**
	Calls access.
*/
int File::access(int what, bool want_throw)
{
	int r;

	if ((r = ::access(path, what)) == -1 && want_throw)
		throw Error(errno);
	return r;
}

/**
	Calls chmod or fchmod.
*/
void File::chmod(mode_t mode)
{
	int r;

	if (fd == -1)
		r = ::chmod(path, mode);
	else
		r = ::fchmod(fd, mode);
	if (r == -1)
		throw Error(errno);
}

/**
	Calls chown or fchown.
*/
void File::chown(uid_t uid, gid_t gid)
{
	int r;

	if (fd == -1)
		r = ::chown(path, uid, gid);
	else
		r = ::fchown(fd, uid, gid);
	if (r == -1)
		throw Error(errno);
}

/**
	Calls close.
*/
void File::close(void)
{
	if (::close(fd) == -1)
		throw Error(errno);
}

/**
	Calls dup.
*/
File File::dup(void)
{
	int fd2;

	if ((fd2 = ::dup(fd)) == -1)
		throw Error(errno);
	return File(fd2);
}

/**
	Calls dup2.
*/
File File::dup2(int fd2)
{
	if (::dup2(fd, fd2) == -1)
		throw Error(errno);
	return File(fd2);
}

/**
	Calls fcntl. Works only if the single optional argument is an int or pointer.
*/
int File::fcntl(int op, ...)
{
	int n;

	va_list ap;
	va_start(ap, op);
	if ((n = ::fcntl(fd, op, va_arg(ap, intptr_t))) == -1)
		throw Error(errno);
	va_end(ap);
	return n;
}

/**
	Calls fsync or fdatasync.
*/
void File::fsync(int op)
{
	int r;

	switch (op) {
#if _POSIX_SYNCHRONIZED_IO > 0
	case O_SYNC:
#endif
		r = ::fsync(fd);
#if _POSIX_SYNCHRONIZED_IO > 0 && O_SYNC != O_DSYNC
		break;
	case O_DSYNC:
		r = ::fdatasync(fd);
#endif
	}
	if (r == -1)
		throw Error(errno);
}

/**
	Calls lchown.
*/
void File::lchown(uid_t uid, gid_t gid)
{
	if (::lchown(path, uid, gid) == -1)
		throw Error(errno);
}

/**
	Calls link.
*/
void File::link(const char *newpath)
{
	if (::link(path, newpath) == -1)
		throw Error(errno);
}

/**
	Calls lockf.
*/
void File::lockf(int op, off_t len)
{
	if (::lockf(fd, op, len) == -1)
		throw Error(errno);
}

/**
	Calls lstat.
*/
void File::lstat(struct stat *buf)
{
	if (::lstat(path, buf) == -1)
		throw Error(errno);
}

/**
	Calls mkfifo.
*/
void File::mkfifo(mode_t perms)
{
	if (::mkfifo(path, perms) == -1)
		throw Error(errno);
}

/**
	Calls mknod.
*/
void File::mknod(mode_t perms, dev_t dev)
{
	if (::mknod(path, perms, dev) == -1)
		throw Error(errno);
}

/**
	Calls mkstemp.
*/
void File::mkstemp(void)
{
	// overriding const -- user better have supplied buffer!
	if ((fd = ::mkstemp((char *)path)) == -1)
		throw Error(errno);
}

/**
	Calls open.
*/
void File::open(int flags, mode_t perms)
{
	if ((fd = ::open(path, flags, perms)) == -1)
		throw Error(errno);
}

/**
	Calls pathconf or fpathconf, with a default value to be used if the value is unspecified.
*/
long File::pathconf(int name, long default_val)
{
	long r;

	errno = 0;
	if (fd == -1)
		r = ::pathconf(path, name);
	else
		r = ::fpathconf(fd, name);
	if (r == -1) {
		if (errno != 0)
			throw Error(errno);
		else
			r = default_val;
	}
	return r;
}

/**
	Calls pipe.
*/
/* static */ void File::pipe(File pf[2])
{
	int pfd[2];

	if (::pipe(pfd) == -1)
		throw Error(errno);
	pf[0].set(pfd[0]);
	pf[1].set(pfd[1]);
}

/**
	Calls poll.
*/
/* static */ int poll(struct pollfd fdinfo[], nfds_t nfds, int timeout)
{
	int r;

	if ((r = ::poll(fdinfo, nfds, timeout)) == -1)
		throw Error(errno);
	return r;
}

/**
	Calls pselect.
*/
/* static */ int pselect(int nfds, fd_set *readset, fd_set *writeset, fd_set *errorset, const struct timespec *timeout, const sigset_t *sigmask)
{
	int r;

	if ((r = ::pselect(nfds, readset, writeset, errorset, timeout, sigmask)) == -1)
		throw Error(errno);
	return r;
}

/**
	Calls read or pread.
*/
ssize_t File::read(void *buf, size_t nbytes, off_t offset)
{
	ssize_t n;

	if (offset == -1)
		n = ::read(fd, buf, nbytes);
	else
		n = ::pread(fd, buf, nbytes, offset);
	if (n == -1)
		throw Error(errno);
	return n;
}

/**
	Calls readlink.
*/
ssize_t File::readlink(char *buf, size_t bufsize)
{
	ssize_t n;

	if ((n = ::readlink(path, buf, bufsize)) == -1)
		throw Error(errno);
	return n;
}

/**
	Calls readv.
*/
ssize_t File::readv(const struct iovec *iov, int iovcnt)
{
	ssize_t n;

	if ((n = ::readv(fd, iov, iovcnt)) == -1)
		throw Error(errno);
	return n;
}

/**
	Calls rename.
*/
void File::rename(const char *newpath)
{
	if (::rename(path, newpath) == -1)
		throw Error(errno);
}

/**
	Calls lseek.
*/
off_t File::seek(off_t pos, int whence)
{
	off_t p;

	if ((p = ::lseek(fd, pos, whence)) == -1)
		throw Error(errno);
	return p;
}

/**
	Calls select.
*/
/* static */ int select(int nfds, fd_set *readset, fd_set *writeset, fd_set *errorset, struct timeval *timeout)
{
	int r;

	if ((r = ::select(nfds, readset, writeset, errorset, timeout)) == -1)
		throw Error(errno);
	return r;
}

/**
	Calls stat or fstat.
*/
void File::stat(struct stat *buf)
{
	int r;

	if (fd == -1)
		r = ::stat(path, buf);
	else
		r = ::fstat(fd, buf);
	if (r == -1)
		throw Error(errno);
}

/**
	Calls statvfs or fstatvfs, or whatever nonportable substitute has been defined.
*/
void File::statvfs(struct STATVFS_NAME *buf)
{
	int r;

	if (fd == -1)
		r = ::STATVFS_NAME(path, buf);
	else
		r = ::FSTATVFS_NAME(fd, buf);
	if (r == -1)
		throw Error(errno);
}

/**
	Calls symlink.
*/
void File::symlink(const char *newpath)
{
	if (::symlink(path, newpath) == -1)
		throw Error(errno);
}

/**
	Calls sync.
*/
/* static */ void File::sync(void)
{
	::sync();
}

/**
	Calls truncate or ftruncate.
*/
void File::truncate(off_t length)
{
	int r;

	if (fd == -1)
		r = ::truncate(path, length);
	else
		r = ::ftruncate(fd, length);
	if (r == -1)
		throw Error(errno);
}

/**
	Calls unlink.
*/
void File::unlink(void)
{
	if (::unlink(path) == -1)
		throw Error(errno);
}

/**
	Calls utime.
*/
void File::utime(const struct utimbuf *timbuf)
{
	if (::utime(path, timbuf) == -1)
		throw Error(errno);
}

/**
	Calls write or pwrite.
*/
ssize_t File::write(const void *buf, size_t nbytes, off_t offset)
{
	ssize_t n;

	if (offset == -1)
		n = ::write(fd, buf, nbytes);
	else
		n = ::pwrite(fd, buf, nbytes, offset);
	if (n == -1)
		throw Error(errno);
	return n;
}

/**
	Calls writev.
*/
ssize_t File::writev(const struct iovec *iov, int iovcnt)
{
	ssize_t n;

	if ((n = ::writev(fd, iov, iovcnt)) == -1)
		throw Error(errno);
	return n;
}
