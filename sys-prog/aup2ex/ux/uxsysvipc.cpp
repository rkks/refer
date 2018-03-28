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
	Calls ftok.
*/
/* static */ key_t SysVIPC::ftok(const char *path, int id)
{
	key_t k;

	if ((k = ::ftok(path, id)) == -1)
		throw Error(errno);
	return k;
}

/**
	Calls msgget.
*/
void SysVMsg::get(key_t key, int flags)
{
	if ((id = ::msgget(key, flags)) == -1)
		throw Error(errno);
}

/**
	Calls msgctl.
*/
void SysVMsg::ctl(int cmd, struct msqid_ds *data)
{
	if (::msgctl(id, cmd, data) == -1)
		throw Error(errno);
}

/**
	Calls msgsnd.
*/
void SysVMsg::snd(const void *msgp, size_t msgsize, int flags)
{
	if (::msgsnd(id, msgp, msgsize, flags) == -1)
		throw Error(errno);
}

/**
	Calls msgrcv.
*/
ssize_t SysVMsg::rcv(void *msgp, size_t mtextsize, long msgtype, int flags)
{
	ssize_t n;

	if ((n = ::msgrcv(id, msgp, mtextsize, msgtype, flags)) == -1)
		throw Error(errno);
	return n;
}

/**
	Calls shmget.
*/
void SysVShm::get(key_t key, size_t size, int flags)
{
	if ((id = ::shmget(key, size, flags)) == -1)
		throw Error(errno);
}

/**
	Calls shmctl.
*/
void SysVShm::ctl(int cmd, struct shmid_ds *data)
{
	if (::shmctl(id, cmd, data) == -1)
		throw Error(errno);
}

/**
	Calls shmat.
*/
void *SysVShm::at(const void *shmaddr, int flags)
{
	void *p;

	if ((p = ::shmat(id, shmaddr, flags)) == (void *)-1)
		throw Error(errno);
	return p;
}

/**
	Calls shmdt. Could be static, as it doesn't use the id.
*/
void SysVShm::dt(const void *shmaddr)
{
	if (::shmdt(shmaddr) == -1)
		throw Error(errno);
}

/**
	Calls semget.
*/
void SysVSem::get(key_t key, int nsems, int flags)
{
	if ((id = ::semget(key, nsems, flags)) == -1)
		throw Error(errno);
}

/**
	Calls semctl.
*/
int SysVSem::ctl(int semnum, int cmd, union semun arg)
{
	int r;

	if ((r = ::semctl(id, semnum, cmd, arg)) == -1)
		throw Error(errno);
	return r;
}

/**
	Calls semop.
*/
void SysVSem::op(struct sembuf *sops, size_t nsops)
{
	if (::semop(id, sops, nsops) == -1)
		throw Error(errno);
}
