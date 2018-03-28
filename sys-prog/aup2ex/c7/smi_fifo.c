/*
	SMI - FIFOs
	AUP2, Sec. 7.03.3

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
#include "../c4/setblock.h"
#include "smi_fifo.h"

/* See comment in smi_receive_fifo about poor handling of too many clients. */
/*
	client uses only sq_clients[0] and sq_clients[1] and doesn't use
	sq_fd_server_w
*/
/*[SMIQ_FIFO]*/
#define MAX_CLIENTS 20

typedef struct {
	SMIENTITY sq_entity;            /* entity */
	int sq_fd_server;               /* server read and ... */
	int sq_fd_server_w;             /* ... write file descriptors */
	char sq_name[SERVER_NAME_MAX];  /* server name */
	struct {
		int cl_fd;                  /* client file descriptor */
		pid_t cl_pid;               /* client process ID */
	} sq_clients[MAX_CLIENTS];
	struct client_id sq_client;     /* client ID */
	size_t sq_msgsize;              /* msg size */
	struct smi_msg *sq_msg;         /* msg buffer */
} SMIQ_FIFO;
/*[clients_bgn]*/
static void clients_bgn(SMIQ_FIFO *p)
{
	int i;

	for (i = 0; i < MAX_CLIENTS; i++)
		p->sq_clients[i].cl_fd = -1;
}
/*[clients_find]*/
static int clients_find(SMIQ_FIFO *p, pid_t pid)
{
	int i, avail = -1;

	for (i = 0; i < MAX_CLIENTS; i++) {
		if (p->sq_clients[i].cl_pid == pid)
			return i;
		if (p->sq_clients[i].cl_fd == -1 && avail == -1)
			avail = i;
	}
	if (avail != -1)
		p->sq_clients[avail].cl_pid = pid;
	return avail;
}
/*[clients_close_all]*/
static void clients_close_all(SMIQ_FIFO *p)
{
	int i;

	for (i = 0; i < MAX_CLIENTS; i++)
		if (p->sq_clients[i].cl_fd != -1) {
			(void)close(p->sq_clients[i].cl_fd);
			p->sq_clients[i].cl_fd = -1;
		}
}
/*[clients_end]*/
static void clients_end(SMIQ_FIFO *p)
{
	clients_close_all(p);
}
/*[make_fifo_name_]*/
static void make_fifo_name_server(const SMIQ_FIFO *p, char *fifoname,
  size_t fifoname_max)
{
	snprintf(fifoname, fifoname_max, "/tmp/smififo-%s", p->sq_name);
}

static void make_fifo_name_client(pid_t pid, char *fifoname,
  size_t fifoname_max)
{
	snprintf(fifoname, fifoname_max, "/tmp/smififo%ld", (long)pid);
}
/*[]*/
/*
	Instead of freeing allocated structure on error, another idea is to build local structure and allocate only at end, copying local structure to allocated one.
*/
/*[smi_open_fifo]*/
SMIQ *smi_open_fifo(const char *name, SMIENTITY entity, size_t msgsize)
{
	SMIQ_FIFO *p = NULL;
	char fifoname[SERVER_NAME_MAX + 50];

	ec_null( p = calloc(1, sizeof(SMIQ_FIFO)) )
	p->sq_msgsize = msgsize + offsetof(struct smi_msg, smi_data);
	ec_null( p->sq_msg = calloc(1, p->sq_msgsize) )
	p->sq_entity = entity;
	if (strlen(name) >= SERVER_NAME_MAX) {
		errno = ENAMETOOLONG;
		EC_FAIL
	}
	strcpy(p->sq_name, name);
	make_fifo_name_server(p, fifoname, sizeof(fifoname));
	if (p->sq_entity == SMI_SERVER) {
		clients_bgn(p);
		if (mkfifo(fifoname, PERM_FILE) == -1 && errno != EEXIST)
			EC_FAIL
		ec_neg1( p->sq_fd_server = open(fifoname, O_RDONLY) )
		ec_neg1( p->sq_fd_server_w = open(fifoname, O_WRONLY) )
	}
	else {
		ec_neg1( p->sq_fd_server = open(fifoname, O_WRONLY) )
		make_fifo_name_client(getpid(), fifoname, sizeof(fifoname));
		(void)unlink(fifoname);
		ec_neg1( mkfifo(fifoname, PERM_FILE) )
		ec_neg1( p->sq_clients[0].cl_fd =
		  open(fifoname, O_RDONLY | O_NONBLOCK) )
		ec_false( setblock(p->sq_clients[0].cl_fd, true) )
		ec_neg1( p->sq_clients[1].cl_fd = open(fifoname, O_WRONLY) )
	}
	return (SMIQ *)p;

EC_CLEANUP_BGN
	if (p != NULL) {
		free(p->sq_msg);
		free(p);
	}
	return NULL;
EC_CLEANUP_END
}
/*[smi_close_fifo]*/
bool smi_close_fifo(SMIQ *sqp)
{
	SMIQ_FIFO *p = (SMIQ_FIFO *)sqp;

	clients_end(p);
	(void)close(p->sq_fd_server);
	/*
		Leaving server FIFO around allows future sq_clients to be started before server.
	*/
	if (p->sq_entity == SMI_CLIENT) {
		char fifoname[SERVER_NAME_MAX + 50];

		make_fifo_name_client(getpid(), fifoname, sizeof(fifoname));
		(void)unlink(fifoname);
	}
	else
		(void)close(p->sq_fd_server_w);
	free(p->sq_msg);
	free(p);
	return true;
}
/*[]*/
/* Assumes whole message can be sent at once. */
/*[smi_send_getaddr_fifo]*/
bool smi_send_getaddr_fifo(SMIQ *sqp, struct client_id *client,
  void **addr)
{
	SMIQ_FIFO *p = (SMIQ_FIFO *)sqp;

	if (p->sq_entity == SMI_SERVER)
		p->sq_client = *client;
	*addr = p->sq_msg;
	return true;
}
/*[smi_send_release_fifo]*/
bool smi_send_release_fifo(SMIQ *sqp)
{
	SMIQ_FIFO *p = (SMIQ_FIFO *)sqp;
	ssize_t nwrite;

	if (p->sq_entity == SMI_SERVER) {
		int nclient = clients_find(p, p->sq_client.c_id1);
		if (nclient == -1 || p->sq_clients[nclient].cl_fd == -1) {
			errno = EADDRNOTAVAIL;
			EC_FAIL
		}
		ec_neg1( nwrite = write(p->sq_clients[nclient].cl_fd, p->sq_msg,
		  p->sq_msgsize) )
	}
	else {
		p->sq_msg->smi_client.c_id1 = (long)getpid();
		ec_neg1( nwrite = write(p->sq_fd_server, p->sq_msg,
		  p->sq_msgsize) )
	}
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}
/*[smi_receive_getaddr_fifo]*/
bool smi_receive_getaddr_fifo(SMIQ *sqp, void **addr)
{
	SMIQ_FIFO *p = (SMIQ_FIFO *)sqp;
	ssize_t nread;

	if (p->sq_entity == SMI_SERVER) {
		int nclient;
		char fifoname[SERVER_NAME_MAX + 50];

		while (true) {
			ec_neg1( nread = read(p->sq_fd_server, p->sq_msg,
			  p->sq_msgsize) )
			if (nread == 0) {
				errno = ENETDOWN;
				EC_FAIL
			}
			if (nread < offsetof(struct smi_msg, smi_data)) {
				errno = E2BIG;
				EC_FAIL
			}
			if ((nclient = clients_find(p,
			  (pid_t)p->sq_msg->smi_client.c_id1)) == -1) {
				continue; /* client not notified */
			}
			if (p->sq_clients[nclient].cl_fd == -1) {
				make_fifo_name_client((pid_t)p->sq_msg->smi_client.c_id1,
				  fifoname, sizeof(fifoname));
				ec_neg1( p->sq_clients[nclient].cl_fd =
				  open(fifoname, O_WRONLY) )
			}
			break;
		}
	}
	else
		ec_neg1( nread = read(p->sq_clients[0].cl_fd, p->sq_msg,
		  p->sq_msgsize) )
	*addr = p->sq_msg;
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}
/*[smi_receive_release_fifo]*/
bool smi_receive_release_fifo(SMIQ *sqp)
{
	return true;
}
/*[]*/
