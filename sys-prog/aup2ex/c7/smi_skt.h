#ifndef _SMI_SKT_H_
#define _SMI_SKT_H_

#include "smi.h"

/*
	Simple Messaging Interface - socket version
*/
SMIQ *smi_open_skt(const char *name, SMIENTITY entity, size_t msgsize);
bool smi_close_skt(SMIQ *sqp);
bool smi_close_fd_skt(SMIQ *sqp, int fd);
bool smi_send_getaddr_skt(SMIQ *sqp, struct client_id *client,
  void **addr);
bool smi_send_release_skt(SMIQ *sqp);
bool smi_receive_getaddr_skt(SMIQ *sqp, void **addr);
bool smi_receive_release_skt(SMIQ *sqp);

#endif /* _SMI_SKT_H_ */
