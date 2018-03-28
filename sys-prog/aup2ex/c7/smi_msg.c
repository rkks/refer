/*
	SMI - System V messages
	AUP2, Sec. 7.05.3

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
#include "smi_msg.h"
#ifdef HAVE_SYSVMSG
#include <sys/msg.h>
/*[top]*/
typedef struct {
	SMIENTITY sq_entity;	/* entity */
	int sq_qid_server;	/* server identifier */
	int sq_qid_client;	/* client identifier (not used by server) */
	char sq_name[SERVER_NAME_MAX];	/* server name */
	struct client_id sq_client;     /* client ID */
	size_t sq_msgsize; /* msg size */
	struct smi_msg *sq_msg; /* msg buffer */
} SMIQ_MSG;
/*[mkmsg_name_server]*/
static void mkmsg_name_server(const SMIQ_MSG *p, char *msgname,
  size_t msgname_max)
{
	snprintf(msgname, msgname_max, "/tmp/smimsg-%s", p->sq_name);
}
/*[smi_open_msg]*/
SMIQ *smi_open_msg(const char *name, SMIENTITY entity, size_t msgsize)
{
	SMIQ_MSG *p = NULL;
	char msgname[SERVER_NAME_MAX + 100];
	key_t key;

	ec_null( p = calloc(1, sizeof(SMIQ_MSG)) )
	p->sq_msgsize = msgsize + offsetof(struct smi_msg, smi_data);
	ec_null( p->sq_msg = calloc(1, p->sq_msgsize) )
	p->sq_qid_server = p->sq_qid_client = -1;
	p->sq_entity = entity;
	if (strlen(name) >= SERVER_NAME_MAX) {
		errno = ENAMETOOLONG;
		EC_FAIL
	}
	strcpy(p->sq_name, name);
	mkmsg_name_server(p, msgname, sizeof(msgname));
	(void)close(open(msgname, O_WRONLY | O_CREAT, 0));
	ec_neg1( key = ftok(msgname, 1) )
	if (p->sq_entity == SMI_SERVER) {
		if ((p->sq_qid_server = msgget(key, PERM_FILE)) != -1)
			(void)msgctl(p->sq_qid_server, IPC_RMID, NULL);
		ec_neg1( p->sq_qid_server = msgget(key, IPC_CREAT | PERM_FILE) )
	}
	else {
		ec_neg1( p->sq_qid_server = msgget(key, 0) )
		ec_neg1( p->sq_qid_client = msgget(IPC_PRIVATE, PERM_FILE) );
	}
	return (SMIQ *)p;

EC_CLEANUP_BGN
	if (p != NULL)
		(void)smi_close_msg((SMIQ *)p);
	return NULL;
EC_CLEANUP_END
}
/*[smi_close_msg]*/
bool smi_close_msg(SMIQ *sqp)
{
	SMIQ_MSG *p = (SMIQ_MSG *)sqp;

	if (p->sq_entity == SMI_SERVER) {
		char msgname[FILENAME_MAX]; // wrong symbol... need max pathname

		(void)msgctl(p->sq_qid_server, IPC_RMID, NULL);
		mkmsg_name_server(p, msgname, sizeof(msgname));
		(void)unlink(msgname);
	}
	else
		(void)msgctl(p->sq_qid_client, IPC_RMID, NULL);
	free(p->sq_msg);
	free(p);
	return true;
}
/*[smi_send_getaddr_msg]*/
bool smi_send_getaddr_msg(SMIQ *sqp, struct client_id *client,
  void **addr)
{
	SMIQ_MSG *p = (SMIQ_MSG *)sqp;

	if (p->sq_entity == SMI_SERVER)
		p->sq_client = *client;
	*addr = p->sq_msg;
	return true;
}

bool smi_send_release_msg(SMIQ *sqp)
{
	SMIQ_MSG *p = (SMIQ_MSG *)sqp;
	int qid_receiver;

	p->sq_msg->smi_mtype = 1;
	if (p->sq_entity == SMI_SERVER)
		qid_receiver = p->sq_client.c_id1;
	else {
		qid_receiver = p->sq_qid_server;
		p->sq_msg->smi_client.c_id1 = p->sq_qid_client;
	}
	ec_neg1( msgsnd(qid_receiver, p->sq_msg,
	  p->sq_msgsize - sizeof(p->sq_msg->smi_mtype), 0) )
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}
/*[smi_receive_getaddr_msg]*/
bool smi_receive_getaddr_msg(SMIQ *sqp, void **addr)
{
	SMIQ_MSG *p = (SMIQ_MSG *)sqp;
	int qid_receiver;
	ssize_t nrcv;

	if (p->sq_entity == SMI_SERVER)
		qid_receiver = p->sq_qid_server;
	else
		qid_receiver = p->sq_qid_client;
	ec_neg1( nrcv = msgrcv(qid_receiver, p->sq_msg,
	  p->sq_msgsize - sizeof(p->sq_msg->smi_mtype), 0, 0) )
	*addr = p->sq_msg;
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}

bool smi_receive_release_msg(SMIQ *sqp)
{
	return true;
}
/*[]*/

#else /* HAVE_SYSVMSG */
SMIQ *smi_open_msg(const char *name, SMIENTITY entity, size_t msgsize)
{
	errno = ENOSYS;
	return NULL;
}

bool smi_close_msg(SMIQ *sqp)
{
	errno = ENOSYS;
	return false;
}

bool smi_send_getaddr_msg(SMIQ *sqp, struct client_id *client,
  void **addr)
{
	errno = ENOSYS;
	return false;
}

bool smi_send_release_msg(SMIQ *sqp)
{
	errno = ENOSYS;
	return false;
}

bool smi_receive_getaddr_msg(SMIQ *sqp, void **addr)
{
	errno = ENOSYS;
	return false;
}

bool smi_receive_release_msg(SMIQ *sqp)
{
	errno = ENOSYS;
	return false;
}

#endif /* HAVE_SYSVMSG */
