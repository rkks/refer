#ifndef _SMI_MAP2_H_
#define _SMI_MAP2_H_

#include "smi.h"

SMIQ *smi_open_pshm(const char *name, SMIENTITY entity, size_t msgsize);
bool smi_close_pshm(SMIQ *sqp);
bool smi_send_getaddr_pshm(SMIQ *sqp, struct client_id *client,
  void **addr);
bool smi_send_release_pshm(SMIQ *sqp);
bool smi_receive_getaddr_pshm(SMIQ *sqp, void **addr);
bool smi_receive_release_pshm(SMIQ *sqp);

#endif /* _SMI_MAP2_H_ */
