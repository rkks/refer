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
#ifndef _UXSYSVIPC_HPP_
#define _UXSYSVIPC_HPP_

#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/sem.h>

namespace Ux {

/**
	\ingroup Ux
*/
class SysVIPC : public Base {
protected:
	int id;

public:
	SysVIPC(int n = -1)
		: id(n)
		{  }
	static key_t ftok(const char *path, int id);
	operator int()
		{ return id; }
};

/**
	\ingroup Ux
*/
class SysVMsg : public SysVIPC {
public:
	void	get(key_t key, int flags = IPC_CREAT | PERM_FILE);
	void	ctl(int cmd, struct msqid_ds *data = NULL);
	void	snd(const void *msgp, size_t msgsize, int flags = 0);
	ssize_t	rcv(void *msgp, size_t mtextsize, long msgtype = 0, int flags = 0);
};

/**
	\ingroup Ux
*/
class SysVShm : public SysVIPC {
public:
	void	get(key_t key, size_t size, int flags = IPC_CREAT | PERM_FILE);
	void	ctl(int cmd, struct shmid_ds *data = NULL);
	void *	at(const void *shmaddr = NULL, int flags = 0);
	void	dt(const void *shmaddr);
};

/**
	\ingroup Ux
	Union defined by SUS, but not in any standard header.
*/
union semun {
	int val;
	struct semid_ds *buf;
	unsigned short *array;
};

/**
	\ingroup Ux
*/
class SysVSem : public SysVIPC {
public:
	void	get(key_t key, int nsems, int flags = IPC_CREAT | PERM_FILE);
	int		ctl(int semnum, int cmd, union semun arg);
	void	op(struct sembuf *sops, size_t nsops);
};

} // namespace

#endif // _UXSYSVIPC_HPP_
