/*
	SMI - sockets
	AUP2, Sec. 8.05

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
#include "smi_skt.h"
#include "../c8/ssi.h"
/*[SMIQ_SKT]*/
typedef struct {
	SMIENTITY sq_entity;
	SSI *sq_ssip;
	struct client_id sq_client;     /* client ID */
	size_t sq_msgsize; /* msg size */
	struct smi_msg *sq_msg; /* msg buffer */
} SMIQ_SKT;
/*[smi_open_skt]*/
SMIQ *smi_open_skt(const char *name, SMIENTITY entity, size_t msgsize)
{
	SMIQ_SKT *p = NULL;

	ec_null(  p = calloc(1, sizeof(SMIQ_SKT)) )
	p->sq_msgsize = msgsize + offsetof(struct smi_msg, smi_data);
	ec_null( p->sq_msg = calloc(1, p->sq_msgsize) )
	p->sq_entity = entity;
	ec_null( p->sq_ssip = ssi_open(name, entity == SMI_SERVER) )
	return (SMIQ *)p;

EC_CLEANUP_BGN
	(void)smi_close_skt((SMIQ *)p);
	return NULL;
EC_CLEANUP_END
}
/*[smi_close_skt]*/
bool smi_close_skt(SMIQ *sqp)
{
	SMIQ_SKT *p = (SMIQ_SKT *)sqp;
	SSI *ssip;

	if (p != NULL) {
		ssip = p->sq_ssip;
		free(p->sq_msg);
		free(p);
		if (ssip != NULL)
			ec_false( ssi_close(ssip) )
	}
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}
/*[smi_send_getaddr_skt]*/
bool smi_send_getaddr_skt(SMIQ *sqp, struct client_id *client,
  void **addr)
{
	SMIQ_SKT *p = (SMIQ_SKT *)sqp;

	if (p->sq_entity == SMI_SERVER)
		p->sq_client = *client;
	*addr = p->sq_msg;
	return true;
}
/*[smi_send_release_skt]*/
bool smi_send_release_skt(SMIQ *sqp)
{
	SMIQ_SKT *p = (SMIQ_SKT *)sqp;
	int fd;

	if (p->sq_entity == SMI_SERVER)
		ec_neg1( fd = p->sq_client.c_id1 )
	else
		ec_neg1( fd = ssi_get_server_fd(p->sq_ssip) )
	ec_neg1( writeall(fd, p->sq_msg, p->sq_msgsize) )
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}
/*[smi_receive_getaddr_skt]*/
bool smi_receive_getaddr_skt(SMIQ *sqp, void **addr)
{
	SMIQ_SKT *p = (SMIQ_SKT *)sqp;
	ssize_t /*nremain,*/ nread;
	int fd;

	*addr = p->sq_msg;
	while (true) {
		if (p->sq_entity == SMI_SERVER)
			ec_neg1( fd = ssi_wait_server(p->sq_ssip) )
		else
			ec_neg1( fd = ssi_get_server_fd(p->sq_ssip) )
		ec_neg1( nread = readall(fd, p->sq_msg, p->sq_msgsize) )
		if (nread == 0) {
			if (p->sq_entity == SMI_SERVER) {
				ec_false( ssi_close_fd(p->sq_ssip, fd) )
				continue;
			}
			else {
				errno = EINVAL; /* book had ENOMSG, but Darwin doesn't have it */
				EC_FAIL
			}
		}
		else
			break;
	}
	if (p->sq_entity == SMI_SERVER)
		p->sq_msg->smi_client.c_id1 = fd;
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}
/*[smi_receive_release_skt]*/
bool smi_receive_release_skt(SMIQ *sqp)
{
	return true;
}
/*[]*/
