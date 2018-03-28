/*
	SMI - POSIX messages
	AUP2, Sec. 7.07.2

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
#include "smi_mq.h"
#ifdef COMPILES_POSIX_MESSAGE_PASSING
#include <mqueue.h>
#endif

#ifndef COMPILES_POSIX_MESSAGE_PASSING

typedef int mqd_t;
struct mq_attr {
	long mq_flags;
	long mq_maxmsg;
	long mq_msgsize;
	long mq_curmsgs;
};

static int missing_function(const char *s, ...)
{
	errno = ENOSYS;
	return -1;
}

#define mq_open (mqd_t)missing_function
#define mq_close(a) missing_function(NULL)
#define mq_unlink missing_function
#define mq_send(a, b, c, d) missing_function(NULL)
#define mq_receive(a, b, c, d) missing_function(NULL)

#endif
/*[top]*/
#define MAX_CLIENTS 20

typedef struct {
	SMIENTITY sq_entity;            /* entity */
	mqd_t sq_mqd_server;            /* server message-queue descriptor */
	char sq_name[SERVER_NAME_MAX];  /* server name */
	struct client_info { /* Client uses only sq_clients[0] */
		mqd_t cl_mqd;               /* client message-queue descriptor */
		pid_t cl_pid;               /* client process ID */
	} sq_clients[MAX_CLIENTS];
	struct client_id sq_client;     /* client ID */
	size_t sq_msgsize; /* msg size */
	struct smi_msg *sq_msg; /* msg buffer */
} SMIQ_MQ;
/*[make_mq_name]*/
static void make_mq_name_server(const SMIQ_MQ *p, char *mqname,
  size_t mqname_max)
{
	snprintf(mqname, mqname_max, "/smimq-s%s", p->sq_name);
}

static void make_mq_name_client(pid_t pid, char *mqname,
  size_t mqname_max)
{
	snprintf(mqname, mqname_max, "/smimq-c%d", pid);
}
/*[get_client_mqd]*/
static mqd_t get_client_mqd(SMIQ_MQ *p, pid_t pid)
{
	int i, avail = -1;
	char mqname[SERVER_NAME_MAX + 100];

	for (i = 0; i < MAX_CLIENTS; i++) {
		if (p->sq_clients[i].cl_pid == pid)
			return p->sq_clients[i].cl_mqd;
		if (avail == -1 && p->sq_clients[i].cl_pid == 0)
			avail = i;
	}
	errno = ECONNREFUSED;
	ec_neg1( avail )
	p->sq_clients[avail].cl_pid = pid;
	make_mq_name_client(pid, mqname, sizeof(mqname));
	ec_neg1( p->sq_clients[avail].cl_mqd = mq_open(mqname, O_WRONLY) )
	return p->sq_clients[avail].cl_mqd;

EC_CLEANUP_BGN
	return (mqd_t)-1;
EC_CLEANUP_END
}
/*[smi_open_mq]*/
static pid_t my_pid;

SMIQ *smi_open_mq(const char *name, SMIENTITY entity, size_t msgsize)
{
	SMIQ_MQ *p = NULL;
	char mqname[SERVER_NAME_MAX + 100];
	struct mq_attr attr = {0};

	my_pid = getpid();
	ec_null( p = calloc(1, sizeof(SMIQ_MQ)) )
	p->sq_msgsize = msgsize + offsetof(struct smi_msg, smi_data);
	ec_null( p->sq_msg = calloc(1, p->sq_msgsize) )
	p->sq_entity = entity;
	p->sq_mqd_server = p->sq_clients[0].cl_mqd = (mqd_t)-1;
	if (strlen(name) >= SERVER_NAME_MAX) {
		errno = ENAMETOOLONG;
		EC_FAIL
	}
	strcpy(p->sq_name, name);
	make_mq_name_server(p, mqname, sizeof(mqname));
	attr.mq_maxmsg = 100;
	attr.mq_msgsize = p->sq_msgsize;
	if (p->sq_entity == SMI_SERVER) {
		(void)mq_unlink(mqname);
		ec_cmp( errno, ENOSYS )
		ec_neg1( p->sq_mqd_server = mq_open(mqname, O_RDONLY | O_CREAT,
		  PERM_FILE, &attr) )
	}
	else {
		ec_neg1( p->sq_mqd_server = mq_open(mqname, O_WRONLY) )
		make_mq_name_client(my_pid, mqname, sizeof(mqname));
		ec_neg1( p->sq_clients[0].cl_mqd = mq_open(mqname,
		  O_RDONLY | O_CREAT, PERM_FILE, &attr) )
	}
	return (SMIQ *)p;

EC_CLEANUP_BGN
	if (p != NULL)
		(void)smi_close_mq((SMIQ *)p);
	return NULL;
EC_CLEANUP_END
}
/*[smi_close_mq]*/
bool smi_close_mq(SMIQ *sqp)
{
	SMIQ_MQ *p = (SMIQ_MQ *)sqp;
	char msgname[SERVER_NAME_MAX + 100];

	if (p->sq_entity == SMI_SERVER) {
		make_mq_name_server(p, msgname, sizeof(msgname));
		(void)mq_close(p->sq_mqd_server);
		(void)mq_unlink(msgname);
	}
	else {
		make_mq_name_client(my_pid, msgname, sizeof(msgname));
		(void)mq_close(p->sq_mqd_server);
		(void)mq_unlink(msgname);
	}
	free(p->sq_msg);
	free(p);
	return true;
}
/*[smi_send_getaddr_mq]*/
bool smi_send_getaddr_mq(SMIQ *sqp, struct client_id *client,
  void **addr)
{
	SMIQ_MQ *p = (SMIQ_MQ *)sqp;

	if (p->sq_entity == SMI_SERVER)
		p->sq_client = *client;
	*addr = p->sq_msg;
	return true;
}

bool smi_send_release_mq(SMIQ *sqp)
{
	SMIQ_MQ *p = (SMIQ_MQ *)sqp;
	mqd_t mqd_receiver;

	if (p->sq_entity == SMI_SERVER)
		ec_neg1( mqd_receiver = get_client_mqd(p, p->sq_client.c_id1) )
	else {
		mqd_receiver = p->sq_mqd_server;
		p->sq_msg->smi_client.c_id1 = my_pid;
	}
	ec_neg1( mq_send(mqd_receiver, (const char *)p->sq_msg,
	  p->sq_msgsize, 0) )
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}
/*[smi_receive_getaddr_mq]*/
bool smi_receive_getaddr_mq(SMIQ *sqp, void **addr)
{
	SMIQ_MQ *p = (SMIQ_MQ *)sqp;
	mqd_t mqd_receiver;
	ssize_t nrcv;

	if (p->sq_entity == SMI_SERVER)
		mqd_receiver = p->sq_mqd_server;
	else
		mqd_receiver = p->sq_clients[0].cl_mqd;
	ec_neg1( nrcv = mq_receive(mqd_receiver, (char *)p->sq_msg,
	  p->sq_msgsize, NULL) )
	*addr = p->sq_msg;
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}

bool smi_receive_release_mq(SMIQ *sqp)
{
	return true;
}
/*[]*/

