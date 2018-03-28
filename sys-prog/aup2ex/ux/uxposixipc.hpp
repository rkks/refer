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
#ifndef _UXPOSIXIPC_HPP_
#define _UXPOSIXIPC_HPP_

#if _POSIX_MESSAGE_PASSING > 0
#include <mqueue.h>
#else
typedef void *mqd_t;
struct mq_attr {
	long mq_flags;
	long mq_maxmsg;
	long mq_msgsize;
	long mq_curmsgs;
};
#endif
#if _POSIX_SEMAPHORES > 0
#include <semaphore.h>
#else
typedef int sem_t;
#endif
#include <sys/mman.h>
 
namespace Ux {

/**
	\ingroup Ux
*/
class PosixMsg : public Base {
protected:
	mqd_t mqd;
public:
	PosixMsg(mqd_t m = (mqd_t)-1)
		: mqd(m)
		{ }
	void	close(void);
	void	getattr(struct mq_attr *attr);
	void	notify(const struct sigevent *ep);
	void	open(const char *name, int flags, mode_t perms = PERM_FILE, struct mq_attr *attr = NULL);
	ssize_t	receive(char *msg, size_t msgsize, unsigned *priorityp = NULL);
	void	send(const char *msg, size_t msgsize, unsigned priority = 0);
	void	setattr(const struct mq_attr *attr, struct mq_attr *oldattr = NULL);
	ssize_t	timedreceive(char *msg, size_t msgsize, unsigned *priorityp, const struct timespec *tmout);
	void	timedsend(const char *msg, size_t msgsize, unsigned priority, const struct timespec *tmout);
	void	unlink(const char *name);
};

/**
	\ingroup Ux
*/
class PosixShm : public File {
public:
	void	open(const char *name, int flags, mode_t perms = PERM_FILE);
	void	unlink(const char *name);
	void *	mmap(size_t len, void *addr = NULL, int prot = PROT_READ | PROT_WRITE,
	  int flags = MAP_SHARED, off_t off = 0);
	void	munmap(void *addr, size_t len);
};

/**
	\ingroup Ux
*/
class PosixSem : public Base {
protected:
	sem_t *sem;

public:
	PosixSem(sem_t *s = NULL)
		: sem(s)
		{ }
	void	close(void);
	void	destroy(void);	
	void	getvalue(int *valuep);
	void	init(int pshared = 1, unsigned value = 0);
	void	open(const char *name, int flags, mode_t perms = PERM_FILE, unsigned value = 0);
	void	post(void);
	void	timedwait(const struct timespec *time);
	void	trywait(void);
	void	unlink(const char *name);
	void	wait(void);
};

} // namespace

#endif // _UXPOSIXIPC_HPP_
