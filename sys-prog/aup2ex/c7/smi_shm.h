#ifndef _SMI_SHM_H_
#define _SMI_SHM_H_

#include "smi.h"

bool smi_client_nowait; /* see smi_open_shm */

/*
	Simple Messaging Interface - System V shm version
*/
SMIQ *smi_open_shm(const char *name, SMIENTITY entity, size_t msgsize);
bool smi_close_shm(SMIQ *sqp);
bool smi_send_getaddr_shm(SMIQ *sqp, struct client_id *client,
  void **addr);
bool smi_send_release_shm(SMIQ *sqp);
bool smi_receive_getaddr_shm(SMIQ *sqp, void **addr);
bool smi_receive_release_shm(SMIQ *sqp);

#endif /* _SMI_SHM_H_ */
