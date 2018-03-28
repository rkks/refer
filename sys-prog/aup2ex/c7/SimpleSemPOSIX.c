/*
	Simple Semaphore Interface - POSIX implementation
	AUP2, Sec. 7.10.1

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
	There may exist a POSIX semaphore implementation for these systems,
	but I haven't located it yet. Change next line if you find one.
	and then please email me at aup@basepath.com.
*/
#if !defined(FREEBSD) && !defined(LINUX)

#include "SimpleSem.h"
#include <semaphore.h>

/*[SSPosix]*/
struct SimpleSem *SimpleSemOpen(const char *name)
{
	struct SimpleSem *sem = NULL;

	ec_null( sem = malloc(sizeof(struct SimpleSem)) )
	if ((sem->sm.sm_sem = sem_open(name, O_CREAT, PERM_FILE, 0)) ==
	  SEM_FAILED)
		EC_FAIL
	return sem;

EC_CLEANUP_BGN
	free(sem);
	return NULL;
EC_CLEANUP_END
}

bool SimpleSemPost(struct SimpleSem *sem)
{
	ec_neg1( sem_post(sem->sm.sm_sem) )
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}

bool SimpleSemWait(struct SimpleSem *sem)
{
	ec_neg1( sem_wait(sem->sm.sm_sem) )
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}

bool SimpleSemClose(struct SimpleSem *sem)
{
	ec_neg1( sem_close(sem->sm.sm_sem) )
	free(sem);
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}

bool SimpleSemRemove(const char *name)
{
	if (unlink(name) == -1 && errno != ENOENT)
		EC_FAIL
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}
/*[]*/

#else /* FREEBSD or LINUX */

struct SimpleSem *SimpleSemOpen(const char *name)
{
	errno = ENOSYS;
	return NULL;
}

bool SimpleSemPost(struct SimpleSem *sem)
{
	errno = ENOSYS;
	return false;
}

bool SimpleSemWait(struct SimpleSem *sem)
{
	errno = ENOSYS;
	return false;
}

bool SimpleSemClose(struct SimpleSem *sem)
{
	errno = ENOSYS;
	return false;
}

bool SimpleSemRemove(const char *name)
{
	errno = ENOSYS;
	return false;
}

#endif /* FREEBSD or LINUX */
