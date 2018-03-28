/*
	Simple Semaphore Interface - System V implementation
	AUP2, Sec. 7.09.2

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
#include "SimpleSem.h"
#include <sys/sem.h>
/*[SimpleSemOpen]*/
struct SimpleSem *SimpleSemOpen(const char *name)
{
	struct SimpleSem *sem = NULL;
	key_t key;
	union semun {
	    int val;
	    struct semid_ds *buf;
	    unsigned short *array;
	} arg;
	struct sembuf sop;

	(void)close(open(name, O_WRONLY | O_CREAT, 0));
	ec_neg1( key = ftok(name, 1) )
	ec_null( sem = malloc(sizeof(struct SimpleSem)) )
	if ((sem->sm.sm_semid = semget(key, 1,
	  PERM_FILE | IPC_CREAT | IPC_EXCL)) != -1) {
		arg.val = 0;
		ec_neg1( semctl(sem->sm.sm_semid, 0, SETVAL, arg) )
		sop.sem_num = 0;
		sop.sem_op = 0;
		sop.sem_flg = 0;
		ec_neg1( semop(sem->sm.sm_semid, &sop, 1) )
	}
	else {
		if (errno == EEXIST) {
			while (true)
				if ((sem->sm.sm_semid = semget(key, 1, PERM_FILE)) == -1) {
					if (errno == ENOENT) {
						sleep(1);
						continue;
					}
					else
						EC_FAIL
				}
				else
					break;
#ifndef FREEBSD // bug -- otime not set
			while (true) {
				struct semid_ds buf;

				arg.buf = &buf;
				ec_neg1( semctl(sem->sm.sm_semid, 0, IPC_STAT, arg) )
				if (buf.sem_otime == 0 && false) {
					sleep(1);
					continue;
				}
				else
					break;
			}
#endif
		}
		else
			EC_FAIL
	}
	return sem;

EC_CLEANUP_BGN
	free(sem);
	return NULL;
EC_CLEANUP_END
}
/*[SimpleSemPost]*/
bool SimpleSemPost(struct SimpleSem *sem)
{
	struct sembuf sop;

	sop.sem_num = 0;
	sop.sem_op = 1;
	sop.sem_flg = 0;
	ec_neg1( semop(sem->sm.sm_semid, &sop, 1) )
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}
/*[SimpleSemWait]*/
bool SimpleSemWait(struct SimpleSem *sem)
{
	struct sembuf sop;

	sop.sem_num = 0;
	sop.sem_op = -1;
	sop.sem_flg = 0;
	ec_neg1( semop(sem->sm.sm_semid, &sop, 1) )
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}
/*[SimpleSemClose]*/
bool SimpleSemClose(struct SimpleSem *sem)
{
	free(sem);
	return true;
}
/*[SimpleSemRemove]*/
bool SimpleSemRemove(const char *name)
{
	key_t key;
	int semid;

	if ((key = ftok(name, 1)) == -1) {
		if (errno != ENOENT)
			EC_FAIL
	}
	else {
		if ((semid = semget(key, 1, PERM_FILE)) == -1) {
			if (errno != ENOENT)
				EC_FAIL
		}
		else
			ec_neg1( semctl(semid, 0, IPC_RMID) )
	}
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}
/*[]*/
