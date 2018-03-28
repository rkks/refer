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
#ifndef _UXFILE_HPP_
#define _UXFILE_HPP_

#include <sys/uio.h>
#include <utime.h>
#include <poll.h>
#ifdef FREEBSD
typedef unsigned int nfds_t; /* for NuWait_poll */
#endif

#ifndef _POSIX_SYNCHRONIZED_IO
#define O_SYNC 0 // for sync default argument
#endif

namespace Ux {

/**
	\ingroup Ux
*/
class File : public Base {
protected:
	int fd;
	const char *path;
	ssize_t size;

public:
	File(int f = -1, const char *p = NULL, ssize_t s = -1)
		: fd(f), path(p), size(s)
		{ }
	File(const char *p, ssize_t s = -1)
		: fd(-1), path(p), size(s)
		{ }
	void set(int f = -1, char *p = NULL, ssize_t s = -1)
		{
			fd = f;
			path = p;
			size = s;
		}
	void set(char *p, ssize_t s = -1)
		{
			fd = -1;
			path = p;
			size = s;
		}
	ssize_t get_size(void) const
		{ return size; }
	int get_fd(void) const
		{ return fd; }
	operator const char*()
		{ return path; }

	void alloc(ssize_t n = -1);
	void free(void);
	size_t get_max_name(const char *path = "/");

	int			access(int what, bool want_throw = true);
	void		chmod(mode_t mode);
	void		chown(uid_t uid, gid_t gid);
	void		close(void);
	File 		dup(void);
	File 		dup2(int fd2);
	int			fcntl(int op, ...);
	void		fsync(int op = O_SYNC);
	void		lchown(uid_t uid, gid_t gid);
	void		link(const char *newpath);
	void		lockf(int op, off_t len);
	void		lstat(struct stat *buf);
	void		mkfifo(mode_t perms = PERM_FILE);
	void		mknod(mode_t mode, dev_t dev);
	void		mkstemp(void);
	void		open(int flags, mode_t perms = PERM_FILE);
	long		pathconf(int name, long default_val = -1);
	static void	pipe(File pf[2]);
	static int	poll(struct pollfd fdinfo[], nfds_t nfds, int timeout = -1);
	static int	pselect(int nfds, fd_set *readset, fd_set *writeset = NULL, fd_set *errorset = NULL, const struct timespec *timeout = NULL, const sigset_t *sigmask = NULL);
	ssize_t		read(void *buf, size_t nbytes, off_t offset = -1);
	ssize_t		readlink(char *buf, size_t bufsize);
	ssize_t		readv(const struct iovec *iov, int iovcnt);
	void		rename(const char *newpath);
	off_t		seek(off_t pos, int whence);
	static int	select(int nfds, fd_set *readset, fd_set *writeset = NULL, fd_set *errorset = NULL, struct timeval *timeout = NULL);
	void		stat(struct stat *buf);
	void		statvfs(struct STATVFS_NAME *buf);
	void		symlink(const char *newpath);
	static void	sync(void);
	void		truncate(off_t length);
	void		unlink(void);
	void		utime(const struct utimbuf *timbuf);
	ssize_t		writev(const struct iovec *iov, int iovcnt);
	ssize_t		write(const void *buf, size_t nbytes, off_t offset = -1);
};

} // namespace

#endif // _UXFILE_HPP_
