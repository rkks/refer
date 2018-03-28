#ifndef _SMI_FIFO_H_
#define _SMI_FIFO_H_

#include "smi.h"

/*
	Simple Messaging Interface - fifo version
*/

SMIQ *smi_open_fifo(const char *name, SMIENTITY entity, size_t msgsize);
bool smi_close_fifo(SMIQ *sqp);
bool smi_send_getaddr_fifo(SMIQ *sqp, struct client_id *client,
  void **addr);
bool smi_send_release_fifo(SMIQ *sqp);
bool smi_receive_getaddr_fifo(SMIQ *sqp, void **addr);
bool smi_receive_release_fifo(SMIQ *sqp);

#endif /* _SMI_FIFO_H_ */
