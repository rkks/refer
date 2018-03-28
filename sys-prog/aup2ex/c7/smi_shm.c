/*
	SMI - System V shared memory
	AUP2, Sec. 7.13.3

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
#include "smi_shm.h"
#include <sys/sem.h>
#include <sys/shm.h>
/*[define]*/
#define SEMI_READ	0
#define SEMI_WRITE	1
#define SEMI_POST	1
#define SEMI_WAIT	-1
/*[SMIQ_SHM]*/
typedef struct {
	SMIENTITY sq_entity;	      /* entity */
	int sq_semid_server;	      /* server sem */
	int sq_semid_client;          /* client sem (client only) */
	int sq_shmid_server;          /* server shm ID */
	int sq_shmid_client;          /* client shm ID (client only) */
	struct smi_msg *msg_server;   /* ptr to server shm */
	struct smi_msg *msg_client;   /* ptr to client shm (client only) */
	char sq_name[SERVER_NAME_MAX];/* server name */
	struct client_id sq_client;   /* client identification (server only)*/
} SMIQ_SHM;
/*[]*/
#ifndef BSD_DERIVED
union semun { /* not in sys/sem.h, except in FreeBSD and Darwin */
	int val;
	struct semid_ds *buf;
	unsigned short *array;
};
#endif

/*[mkshm_name_server]*/
static void mkshm_name_server(const SMIQ_SHM *p, char *shmname,
  size_t shmname_max)
{
	snprintf(shmname, shmname_max, "/tmp/smishm-%s", p->sq_name);
}
/*[op_semi]*/
static int op_semi(int semid, int sem_num, int sem_op)
{
	struct sembuf sbuf;
	int r;

	sbuf.sem_num = sem_num;
	sbuf.sem_op = sem_op;
	sbuf.sem_flg = 0;
	ec_neg1( r = semop(semid, &sbuf, 1) )
	return r;

EC_CLEANUP_BGN
	return -1;
EC_CLEANUP_END
}
/*[init_semi]*/
static int init_semi(int semid)
{
	union semun arg;
	int r;

	arg.val = 0;
	semctl(semid, SEMI_WRITE, SETVAL, arg);
	semctl(semid, SEMI_READ, SETVAL, arg);
	/* Following call will set otime, allowing clients to proceed. */
	ec_neg1( r = op_semi(semid, SEMI_WRITE, SEMI_POST) )
	return r;

EC_CLEANUP_BGN
	return -1;
EC_CLEANUP_END
}
/*[]*/
/*
	Following function used only by server.
*/
/*[getaddr]*/
#define MAX_ATTACHED 20

static void *getaddr(int shmid)
{
	static struct {
		int shmid;
		void *addr;
		long time;
	} mapped[MAX_ATTACHED];
	static long clock;
	int i, avail, oldest, r;
	static bool first = true;

	if (first) {
		first = false;
		for (i = 0; i < MAX_ATTACHED; i++)
			mapped[i].shmid = -1;
	}
	if (shmid == -1) { /* final cleanup */
		for (i = 0; i < MAX_ATTACHED; i++)
			if (mapped[i].shmid != -1) {
				(void)shmdt(mapped[i].addr);
				mapped[i].shmid = -1;
			}
		return NULL;
	}
	clock++;
	avail = -1;
	for (i = 0; i < MAX_ATTACHED; i++) {
		if (mapped[i].shmid == shmid) {
			mapped[i].time = clock;
			return mapped[i].addr;
		}
		if (mapped[i].shmid == -1 && avail == -1)
			avail = i;
	}
	if (avail == -1) { /* all fds in use; find oldest */
		oldest = -1;
		for (i = 0; i < MAX_ATTACHED; i++)
			if (oldest == -1 || mapped[i].time < oldest) {
				oldest = mapped[i].time;
				avail = i;
			}
		r = shmdt(mapped[avail].addr);
		ec_neg1( r )
	}
	mapped[avail].addr = shmat(shmid, NULL, 0);
	ec_neg1( mapped[avail].addr )
	mapped[avail].time = clock;
	mapped[avail].shmid = shmid;
	return mapped[avail].addr;

EC_CLEANUP_BGN
	return NULL;
EC_CLEANUP_END
}
/*[]*/
/* cleanup needs to be more complete than just free(p) */
/*[smi_open_shm]*/
SMIQ *smi_open_shm(const char *name, SMIENTITY entity, size_t msgsize)
{
	SMIQ_SHM *p = NULL;
	char shmname[FILENAME_MAX];
	int i;
	key_t key;

	ec_null( p = calloc(1, sizeof(SMIQ_SHM)) )
	p->sq_entity = entity;
	if (strlen(name) >= SERVER_NAME_MAX) {
		errno = ENAMETOOLONG;
		EC_FAIL
	}
	strcpy(p->sq_name, name);
	mkshm_name_server(p, shmname, sizeof(shmname));
	(void)close(open(shmname, O_WRONLY | O_CREAT, 0));
	ec_neg1( key = ftok(shmname, 1) )
	if (p->sq_entity == SMI_SERVER) {
		if ((p->sq_semid_server = semget(key, 2, PERM_FILE)) != -1)
			(void)shmctl(p->sq_semid_server, IPC_RMID, NULL);
		ec_neg1( p->sq_semid_server = semget(key, 2,
		  PERM_FILE | IPC_CREAT) )
		p->sq_semid_client = -1;
		if ((p->sq_shmid_server = shmget(key, 0, PERM_FILE)) != -1)
			(void)shmctl(p->sq_shmid_server, IPC_RMID, NULL);
		ec_neg1( p->sq_shmid_server = shmget(key, msgsize,
		  PERM_FILE | IPC_CREAT) )
		p->sq_shmid_client = -1;
		ec_neg1( init_semi(p->sq_semid_server) )
	}
	else {
		ec_neg1( p->sq_semid_server = semget(key, 2, PERM_FILE) )
		ec_neg1( p->sq_semid_client = semget(IPC_PRIVATE, 2,
		  PERM_FILE | IPC_CREAT) )
		ec_neg1( p->sq_shmid_server = shmget(key, msgsize, PERM_FILE) )
		ec_neg1( p->sq_shmid_client = shmget(IPC_PRIVATE, msgsize,
		  PERM_FILE | IPC_CREAT) )
		ec_neg1( p->msg_client = shmat(p->sq_shmid_client, NULL, 0) )
		ec_neg1( init_semi(p->sq_semid_client) )
/*[]*/
		/*
			Wait until server has initialized its semaphores.

			Two problems with this loop:
				1. Wait of 1 sec or more messes up performance testing.
				2. Some systems (e.g., FreeBSD) seem not to update otime.

			Solutions:
				1. Mode that skips the loop.
				2. Loop for at most 10 sec. If it hasn't happened by then,
				   it most likely never will.
		*/
/*[smi_open_shm2]*/
		for (i = 0; !smi_client_nowait && i < 10; i++) {
			union semun arg;
			struct semid_ds ds;

			arg.buf = &ds;
			ec_neg1( semctl(p->sq_semid_server, SEMI_WRITE, IPC_STAT,
			  arg) )
			if (ds.sem_otime > 0)
				break;
			sleep(1);
		}
	}
	ec_neg1( p->msg_server = shmat(p->sq_shmid_server, NULL, 0) )
	return (SMIQ *)p;

EC_CLEANUP_BGN
	free(p);
	return NULL;
EC_CLEANUP_END
}
/*[smi_close_shm]*/
bool smi_close_shm(SMIQ *sqp)
{
	SMIQ_SHM *p = (SMIQ_SHM *)sqp;

	if (p->sq_entity == SMI_SERVER) {
		char shmname[FILENAME_MAX];

		(void)getaddr(-1);
#ifdef DARWIN /* not in book */
{
		union semun dummy;
		ec_neg1( semctl(p->sq_semid_server, 0, IPC_RMID, dummy) )
}
#else
		ec_neg1( semctl(p->sq_semid_server, 0, IPC_RMID) )
#endif
		(void)shmdt(p->msg_server);
		(void)shmctl(p->sq_shmid_server, IPC_RMID, NULL);
		mkshm_name_server(p, shmname, sizeof(shmname));
		(void)unlink(shmname);
	}
	else {
		/* following call to semctl causes msg_time to hang on last client with FreeBSD */
#ifndef FREEBSD
#ifdef DARWIN /* not in book */
{
		union semun dummy;
		ec_neg1( semctl(p->sq_semid_client, 0, IPC_RMID, dummy) )
}
#else
		ec_neg1( semctl(p->sq_semid_client, 0, IPC_RMID) )
#endif
#endif
		(void)shmdt(p->msg_server);
		(void)shmdt(p->msg_client);
		(void)shmctl(p->sq_shmid_client, IPC_RMID, NULL);
	}
	free(p);
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}
/*[]*/
/*[smi_send_getaddr_shm]*/
bool smi_send_getaddr_shm(SMIQ *sqp, struct client_id *client, void **addr)
{
	SMIQ_SHM *p = (SMIQ_SHM *)sqp;
	int semid_receiver;

	if (p->sq_entity == SMI_SERVER) {
		semid_receiver = client->c_id2;
		p->sq_client = *client;
	}
	else
		semid_receiver = p->sq_semid_server;
	ec_neg1( op_semi(semid_receiver, SEMI_WRITE, SEMI_WAIT) )
	if (p->sq_entity == SMI_SERVER)
		ec_null( *addr = getaddr(client->c_id1) )
	else {
		*addr = p->msg_server;
		((struct smi_msg *)*addr)->smi_client.c_id1 = p->sq_shmid_client;
		((struct smi_msg *)*addr)->smi_client.c_id2 = p->sq_semid_client;
	}
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}
/*[smi_send_release_shm]*/
bool smi_send_release_shm(SMIQ *sqp)
{
	SMIQ_SHM *p = (SMIQ_SHM *)sqp;
	int semid_receiver;

	if (p->sq_entity == SMI_SERVER)
		semid_receiver = p->sq_client.c_id2;
	else
		semid_receiver = p->sq_semid_server;
	ec_neg1( op_semi(semid_receiver, SEMI_READ, SEMI_POST) )
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}
/*[smi_receive_getaddr_shm]*/
bool smi_receive_getaddr_shm(SMIQ *sqp, void **addr)
{
	SMIQ_SHM *p = (SMIQ_SHM *)sqp;
	int semid_receiver;

	if (p->sq_entity == SMI_SERVER)
		semid_receiver = p->sq_semid_server;
	else
		semid_receiver = p->sq_semid_client;
	ec_neg1( op_semi(semid_receiver, SEMI_READ, SEMI_WAIT) )
	if (p->sq_entity == SMI_SERVER)
		*addr = p->msg_server;
	else
		*addr = p->msg_client;
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}
/*[smi_receive_release_shm]*/
bool smi_receive_release_shm(SMIQ *sqp)
{
	SMIQ_SHM *p = (SMIQ_SHM *)sqp;
	int semid_receiver;

	if (p->sq_entity == SMI_SERVER)
		semid_receiver = p->sq_semid_server;
	else
		semid_receiver = p->sq_semid_client;
	ec_neg1( op_semi(semid_receiver, SEMI_WRITE, SEMI_POST) )
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}
/*[]*/
