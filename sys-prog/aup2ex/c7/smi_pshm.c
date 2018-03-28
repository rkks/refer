/*
	SMI - POSIX shared memory
	AUP2, Sec. 7.14.2

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
#include "smi_pshm.h"
/*
	There may exist a POSIX semaphore implementation for these systems,
	but I haven't located it yet. Change next line if you find one.
	and then please email me at aup@basepath.com.
*/
#if !defined(FREEBSD) && !defined(LINUX)

#include <semaphore.h>
#define _P1003_1B_VISIBLE
#include <sys/mman.h>

/*
	Shared memory with separate memory segment for server and all clients.
	Uses POSIX memory-based semaphores and POSIX shared memory.

	Missing is a way for a client to tell the server to make its slot available. It isn't possible for a client's slot to be dropped and then reused, as the memory semaphores have to persist undisturbed.
*/

/* Structure must contain no pointers. */
/*[shared_mem]*/
struct shared_mem {
	sem_t sm_sem_w;
	sem_t sm_sem_r;
	struct smi_msg sm_msg; /* variable size -- must be last */
};
/*[SMIQ_PSHM]*/
#define MAX_CLIENTS 50

typedef struct {
	SMIENTITY sq_entity;            /* entity */
	char sq_name[SERVER_NAME_MAX];	/* server name */
	int sq_srv_fd;					/* server shm file descriptor */
	struct shared_mem *sq_srv_mem;	/* server mapped shm segment */
	struct client {
		pid_t cl_pid;               /* client process ID */
		int cl_fd;                  /* client shm file descriptor */
		struct shared_mem *cl_mem;	/* client mapped shm segment */
	} sq_clients[MAX_CLIENTS];		/* client uses only [0] */
	struct client_id sq_client;     /* client ident. (server only)*/
	size_t sq_msgsize;				/* message size */
} SMIQ_PSHM;
/*[MEM_SIZE]*/
#define MEM_SIZE(s)\
  (sizeof(struct shared_mem) - sizeof(struct smi_msg) + (s))
/*[mkshm_name]*/
static void mkshm_name_server(const SMIQ_PSHM *p, char *shmname,
  size_t shmname_max)
{
	snprintf(shmname, shmname_max, "/smipshm-%s", p->sq_name);
}

static void mkshm_name_client(pid_t pid, char *shmname,
  size_t shmname_max)
{
	snprintf(shmname, shmname_max, "/smipshm-%d", pid);
}
/*[op_semi]*/
#define SEMI_READ		0
#define SEMI_WRITE		1
#define SEMI_DESTROY	2
#define SEMI_POST		1
#define SEMI_WAIT		-1

static int op_semi(struct shared_mem *m, int sem_num, int sem_op)
{
	sem_t *sem_p = NULL;

	if (sem_num == SEMI_WRITE)
		sem_p = &m->sm_sem_w;
	else
		sem_p = &m->sm_sem_r;
	switch (sem_op) {
	case SEMI_WAIT:
		ec_neg1( sem_wait(sem_p) )
		break;
	case SEMI_POST:
		ec_neg1( sem_post(sem_p) )
		break;
	case SEMI_DESTROY:
		ec_neg1( sem_destroy(sem_p) )
	}
	return 0;

EC_CLEANUP_BGN
	return -1;
EC_CLEANUP_END
}
/*[get_client]*/
static struct client *get_client(SMIQ_PSHM *p, pid_t pid)
{
	int i, avail = -1;
	char shmname[SERVER_NAME_MAX + 50];

	for (i = 0; i < MAX_CLIENTS; i++) {
		if (p->sq_clients[i].cl_pid == pid)
			return &p->sq_clients[i];
		if (p->sq_clients[i].cl_pid == 0 && avail == -1)
			avail = i;
	}
	if (avail == -1) {
		errno = EADDRNOTAVAIL;
		EC_FAIL
	}
	p->sq_clients[avail].cl_pid = pid;
	mkshm_name_client(pid, shmname, sizeof(shmname));
	ec_neg1( p->sq_clients[avail].cl_fd = shm_open(shmname, O_RDWR,
	  PERM_FILE) )
	p->sq_clients[avail].cl_mem = mmap(NULL, MEM_SIZE(p->sq_msgsize),
	  PROT_READ | PROT_WRITE, MAP_SHARED, p->sq_clients[avail].cl_fd,
	  0);
	ec_cmp(p->sq_clients[avail].cl_mem, MAP_FAILED)
	return &p->sq_clients[avail];

EC_CLEANUP_BGN
	return NULL;
EC_CLEANUP_END
}
/*[]*/
/* cleanup needs to be more complete than just free(p) */
/*[smi_open_pshm]*/
SMIQ *smi_open_pshm(const char *name, SMIENTITY entity, size_t msgsize)
{
	SMIQ_PSHM *p = NULL;
	char shmname[SERVER_NAME_MAX + 50];

	ec_null( p = calloc(1, sizeof(SMIQ_PSHM)) )
	p->sq_entity = entity;
	p->sq_msgsize = msgsize;
	if (strlen(name) >= SERVER_NAME_MAX) {
		errno = ENAMETOOLONG;
		EC_FAIL
	}
	strcpy(p->sq_name, name);
	mkshm_name_server(p, shmname, sizeof(shmname));
	if (p->sq_entity == SMI_SERVER) {
		if ((p->sq_srv_fd = shm_open(shmname, O_RDWR, PERM_FILE)) != -1) {
			(void)shm_unlink(shmname);
			(void)close(p->sq_srv_fd);
		}
		ec_neg1( p->sq_srv_fd = shm_open(shmname, O_RDWR | O_CREAT,
		  PERM_FILE) )
		ec_neg1( ftruncate(p->sq_srv_fd, MEM_SIZE(msgsize)) )
		p->sq_srv_mem = mmap(NULL, MEM_SIZE(msgsize),
		  PROT_READ | PROT_WRITE, MAP_SHARED, p->sq_srv_fd, 0);
		ec_cmp(p->sq_srv_mem, MAP_FAILED)
		ec_neg1( sem_init(&p->sq_srv_mem->sm_sem_w, true, 1) )
		ec_neg1( sem_init(&p->sq_srv_mem->sm_sem_r, true, 0) )
	}
	else {
		ec_neg1( p->sq_srv_fd = shm_open(shmname, O_RDWR, PERM_FILE) )
		p->sq_srv_mem = mmap(NULL, MEM_SIZE(msgsize),
		  PROT_READ | PROT_WRITE, MAP_SHARED, p->sq_srv_fd, 0);
		ec_cmp(p->sq_srv_mem, MAP_FAILED)
		mkshm_name_client(getpid(), shmname, sizeof(shmname));
		if ((p->sq_clients[0].cl_fd = shm_open(shmname, O_RDWR, PERM_FILE))
		  != -1) {
			(void)shm_unlink(shmname);
			(void)close(p->sq_clients[0].cl_fd);
		}
		ec_neg1( p->sq_clients[0].cl_fd = shm_open(shmname,
		  O_RDWR | O_CREAT, PERM_FILE) )
		ec_neg1( ftruncate(p->sq_clients[0].cl_fd, MEM_SIZE(msgsize)) )
		p->sq_clients[0].cl_mem = mmap(NULL, MEM_SIZE(msgsize),
		  PROT_READ | PROT_WRITE, MAP_SHARED, p->sq_clients[0].cl_fd, 0);
		ec_cmp(p->sq_clients[0].cl_mem, MAP_FAILED)
		ec_neg1( sem_init(&p->sq_clients[0].cl_mem->sm_sem_w, true,
		  1) ) // not sure if 1 is correct
		ec_neg1( sem_init(&p->sq_clients[0].cl_mem->sm_sem_r, true,
		  0) )
	}
	return (SMIQ *)p;

EC_CLEANUP_BGN
	if (p != NULL)
		(void)smi_close_pshm((SMIQ *)p);
	return NULL;
EC_CLEANUP_END
}
/*[smi_close_pshm]*/
bool smi_close_pshm(SMIQ *sqp)
{
	SMIQ_PSHM *p = (SMIQ_PSHM *)sqp;
	int i;
	char shmname[SERVER_NAME_MAX + 50];

	if (p->sq_entity == SMI_SERVER) {
		(void)op_semi(p->sq_srv_mem, SEMI_WRITE, SEMI_DESTROY);
		(void)op_semi(p->sq_srv_mem, SEMI_READ, SEMI_DESTROY);
	}
	(void)munmap(p->sq_srv_mem, MEM_SIZE(p->sq_msgsize));
	if (p->sq_srv_fd > 0)
		(void)close(p->sq_srv_fd);
	if (p->sq_entity == SMI_SERVER) {
		mkshm_name_server(p, shmname, sizeof(shmname));
		(void)shm_unlink(shmname);
	}
	for (i = 0; i < MAX_CLIENTS; i++)
		if (p->sq_clients[i].cl_mem != NULL) {
			if (p->sq_entity == SMI_CLIENT) {
				(void)op_semi(p->sq_clients[i].cl_mem, SEMI_WRITE,
				  SEMI_DESTROY);
				(void)op_semi(p->sq_clients[i].cl_mem, SEMI_READ,
				  SEMI_DESTROY);
			}
			(void)munmap(p->sq_clients[i].cl_mem, MEM_SIZE(p->sq_msgsize));
			if (p->sq_clients[i].cl_fd > 0)
				(void)close(p->sq_clients[i].cl_fd);
			if (p->sq_entity == SMI_CLIENT) {
				mkshm_name_client(p->sq_clients[i].cl_pid, shmname,
				  sizeof(shmname));
				(void)shm_unlink(shmname);
			}
		}
	free(p);
	return true;
}
/*[]*/
/*[smi_send_getaddr_pshm]*/
bool smi_send_getaddr_pshm(SMIQ *sqp, struct client_id *client,
  void **addr)
{
	SMIQ_PSHM *p = (SMIQ_PSHM *)sqp;
	struct client *cp;
	struct shared_mem *sm;

	if (p->sq_entity == SMI_SERVER) {
		p->sq_client = *client;
		ec_null( cp = get_client(p, client->c_id1) )
		sm = cp->cl_mem;
	}
	else
		sm = p->sq_srv_mem;
	ec_neg1( op_semi(sm, SEMI_WRITE, SEMI_WAIT) )
	if (p->sq_entity == SMI_CLIENT)
		sm->sm_msg.smi_client.c_id1 = getpid();
	*addr = &sm->sm_msg;
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}
/*[smi_send_release_pshm]*/
bool smi_send_release_pshm(SMIQ *sqp)
{
	SMIQ_PSHM *p = (SMIQ_PSHM *)sqp;
	struct client *cp;
	struct shared_mem *sm;

	if (p->sq_entity == SMI_SERVER) {
		ec_null( cp = get_client(p, p->sq_client.c_id1) )
		sm = cp->cl_mem;
	}
	else
		sm = p->sq_srv_mem;
	ec_neg1( op_semi(sm, SEMI_READ, SEMI_POST) )
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}
/*[smi_receive_getaddr_pshm]*/
bool smi_receive_getaddr_pshm(SMIQ *sqp, void **addr)
{
	SMIQ_PSHM *p = (SMIQ_PSHM *)sqp;
	struct shared_mem *sm;

	if (p->sq_entity == SMI_SERVER)
		sm = p->sq_srv_mem;
	else
		sm = p->sq_clients[0].cl_mem;
	ec_neg1( op_semi(sm, SEMI_READ, SEMI_WAIT) )
	*addr = &sm->sm_msg;
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}
/*[smi_receive_release_pshm]*/
bool smi_receive_release_pshm(SMIQ *sqp)
{
	SMIQ_PSHM *p = (SMIQ_PSHM *)sqp;
	struct shared_mem *sm;

	if (p->sq_entity == SMI_SERVER)
		sm = p->sq_srv_mem;
	else
		sm = p->sq_clients[0].cl_mem;
	ec_neg1( op_semi(sm, SEMI_WRITE, SEMI_POST) )
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}
/*[]*/

#else /* FREEBSD or LINUX */

SMIQ *smi_open_pshm(const char *name, SMIENTITY entity, size_t msgsize)
{
	errno = ENOSYS;
	return NULL;
}

bool smi_close_pshm(SMIQ *sqp)
{
	return true;
}

bool smi_send_getaddr_pshm(SMIQ *sqp, struct client_id *client,
  void **addr)
{
	errno = ENOSYS;
	return false;
}

bool smi_send_release_pshm(SMIQ *sqp)
{
	errno = ENOSYS;
	return false;
}

bool smi_receive_getaddr_pshm(SMIQ *sqp, void **addr)
{
	errno = ENOSYS;
	return false;
}

bool smi_receive_release_pshm(SMIQ *sqp)
{
	errno = ENOSYS;
	return false;
}

#endif /* FREEBSD or LINUX */
