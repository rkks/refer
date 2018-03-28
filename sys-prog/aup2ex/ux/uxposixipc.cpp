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
	Calls mq_open.
*/
void PosixMsg::open(const char *name, int flags, mode_t perms, struct mq_attr *attr)
{
#if _POSIX_MESSAGE_PASSING > 0
	if ((mqd = ::mq_open(name, flags, perms, attr)) == (mqd_t)-1)
		throw Error(errno);
#else
	throw Error(ENOSYS);
#endif
}

/**
	Calls mq_close.
*/
void PosixMsg::close(void)
{
#if _POSIX_MESSAGE_PASSING > 0
	if (::mq_close(mqd) == -1)
		throw Error(errno);
#else
	throw Error(ENOSYS);
#endif
}

/**
	Calls mq_unlink. Could be static, as it doesn't refer to the mqd.
*/
void PosixMsg::unlink(const char *name)
{
#if _POSIX_MESSAGE_PASSING > 0
	if (::mq_unlink(name) == -1)
		throw Error(errno);
#else
	throw Error(ENOSYS);
#endif
}

/**
	Calls mq_send.
*/
void PosixMsg::send(const char *msg, size_t msgsize, unsigned priority)
{
#if _POSIX_MESSAGE_PASSING > 0
	if (::mq_send(mqd, msg, msgsize, priority) == -1)
		throw Error(errno);
#else
	throw Error(ENOSYS);
#endif
}

/**
	Calls mq_receive.
*/
ssize_t PosixMsg::receive(char *msg, size_t msgsize, unsigned *priorityp)
{
#if _POSIX_MESSAGE_PASSING > 0
	ssize_t n;

	if ((n = ::mq_receive(mqd, msg, msgsize, priorityp)) == -1)
		throw Error(errno);
	return n;
#else
	throw Error(ENOSYS);
#endif
}

/**
	Calls mq_timedsend.
*/
void PosixMsg::timedsend(const char *msg, size_t msgsize, unsigned priority, const struct timespec *tmout)
{
#if _POSIX_MESSAGE_PASSING > 0 && _POSIX_TIMEOUTS > 0
	if (::mq_timedsend(mqd, msg, msgsize, priority, tmout) == -1)
		throw Error(errno);
#else
	throw Error(ENOSYS);
#endif
}

/**
	Calls mq_timedreceive.
*/
ssize_t PosixMsg::timedreceive(char *msg, size_t msgsize, unsigned *priorityp, const struct timespec *tmout)
{
#if _POSIX_MESSAGE_PASSING > 0 && _POSIX_TIMEOUTS > 0
	ssize_t n;

	if ((n = ::mq_timedreceive(mqd, msg, msgsize, priorityp, tmout)) == -1)
		throw Error(errno);
	return n;
#else
	throw Error(ENOSYS);
#endif
}

/**
	Calls mq_notify.
*/
void PosixMsg::notify(const struct sigevent *ep)
{
#if _POSIX_MESSAGE_PASSING > 0
	if (::mq_notify(mqd, ep) == -1)
		throw Error(errno);
#else
	throw Error(ENOSYS);
#endif
}

/**
	Calls mq_getattr.
*/
void PosixMsg::getattr(struct mq_attr *attr)
{
#if _POSIX_MESSAGE_PASSING > 0
	if (::mq_getattr(mqd, attr) == -1)
		throw Error(errno);
#else
	throw Error(ENOSYS);
#endif
}

/**
	Calls mq_setattr.
*/
void PosixMsg::setattr(const struct mq_attr *attr, struct mq_attr *oldattr)
{
#if _POSIX_MESSAGE_PASSING > 0
	if (::mq_setattr(mqd, attr, oldattr) == -1)
		throw Error(errno);
#else
	throw Error(ENOSYS);
#endif
}

/**
	Calls shm_open.
*/
void PosixShm::open(const char *name, int flags, mode_t perms)
{
#if _POSIX_SHARED_MEMORY_OBJECTS > 0 && !defined(LINUXs)
	if ((fd = ::shm_open(name, flags, perms)) == -1)
		throw Error(errno);
#else
	throw Error(ENOSYS);
#endif
}

/**
	Calls shm_unlink. Could be static, as it doesn't refer to the fd.
*/
void PosixShm::unlink(const char *name)
{
#if _POSIX_SHARED_MEMORY_OBJECTS > 0 && !defined(LINUXs)
	if (::shm_unlink(name) == -1)
		throw Error(errno);
#else
	throw Error(ENOSYS);
#endif
}

/**
	Calls mmap. Arguments are in a different order to take advantage of defaults, and fd
	argument comes from the class.
*/
void *PosixShm::mmap(size_t len, void *addr, int prot, int flags, off_t off)
{
	void *p;

	if ((p = ::mmap(addr, len, prot, flags, fd, off)) == MAP_FAILED)
		throw Error(errno);
	return p;
}

/**
	Calls munmap. Could be static, as it doesn't refer to the fd.
*/
void PosixShm::munmap(void *addr, size_t len)
{
	if (::munmap(addr, len) == -1)
		throw Error(errno);
}

/**
	Calls sem_open.
*/
void PosixSem::open(const char *name, int flags, mode_t perms, unsigned value)
{
#if _POSIX_SEMAPHORES > 0
	if ((sem = ::sem_open(name, flags, perms, value)) == SEM_FAILED)
		throw Error(errno);
#else
	throw Error(ENOSYS);
#endif
}

/**
	Calls sem_close.
*/
void PosixSem::close(void)
{
#if _POSIX_SEMAPHORES > 0
	if (::sem_close(sem) == -1)
		throw Error(errno);
#else
	throw Error(ENOSYS);
#endif
}

/**
	Calls sem_destroy.
*/
void PosixSem::destroy(void)
{
#if _POSIX_SEMAPHORES > 0
	if (::sem_destroy(sem) == -1)
		throw Error(errno);
#else
	throw Error(ENOSYS);
#endif
}	

/**
	Calls sem_getvalue.
*/
void PosixSem::getvalue(int *valuep)
{
#if _POSIX_SEMAPHORES > 0
	if (::sem_getvalue(sem, valuep) == -1)
		throw Error(errno);
#else
	throw Error(ENOSYS);
#endif
}

/**
	Calls sem_init.
*/
void PosixSem::init(int pshared, unsigned value)
{
#if _POSIX_SEMAPHORES > 0
	if (::sem_init(sem, pshared, value) == -1)
		throw Error(errno);
#else
	throw Error(ENOSYS);
#endif
}

/**
	Calls sem_post.
*/
void PosixSem::post(void)
{
#if _POSIX_SEMAPHORES > 0
	if (::sem_post(sem) == -1)
		throw Error(errno);
#else
	throw Error(ENOSYS);
#endif
}

/**
	Calls sem_timedwait.
*/
void PosixSem::timedwait(const struct timespec *time)
{
#if _POSIX_SEMAPHORES > 0 && _POSIX_TIMEOUTS > 0
	if (::sem_timedwait(sem, time) == -1)
		throw Error(errno);
#else
	throw Error(ENOSYS);
#endif
}

/**
	Calls sem_trywait.
*/
void PosixSem::trywait(void)
{
#if _POSIX_SEMAPHORES > 0
	if (::sem_trywait(sem) == -1)
		throw Error(errno);
#else
	throw Error(ENOSYS);
#endif
}

/**
	Calls sem_unlink. Could be static, as it doesn't use sem.
*/
void PosixSem::unlink(const char *name)
{
#if _POSIX_SEMAPHORES > 0
	if (::sem_unlink(name) == -1)
		throw Error(errno);
#else
	throw Error(ENOSYS);
#endif
}

/**
	Calls sem_wait.
*/
void PosixSem::wait(void)
{
#if _POSIX_SEMAPHORES > 0
	if (::sem_wait(sem) == -1)
		throw Error(errno);
#else
	throw Error(ENOSYS);
#endif
}
