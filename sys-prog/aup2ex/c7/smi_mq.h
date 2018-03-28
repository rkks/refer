#ifndef _SMI_MQ_H_
#define _SMI_MQ_H_

/* simplified test to avoid testing _POSIX_VERSION */
/* fails to handle undefined case properly, but version independent */
/* However, Linux (and probably other systems) don't handle the
   undefined case correctly anyway! */
#if defined(_POSIX_MESSAGE_PASSING) && _POSIX_MESSAGE_PASSING != -1
#define COMPILES_POSIX_MESSAGE_PASSING
#endif

#include "smi.h"

/*
	Simple Messaging Interface - POSIX msg version
*/
SMIQ *smi_open_mq(const char *name, SMIENTITY entity, size_t msgsize);
bool smi_close_mq(SMIQ *sqp);
bool smi_send_getaddr_mq(SMIQ *sqp, struct client_id *client,
  void **addr);
bool smi_send_release_mq(SMIQ *sqp);
bool smi_receive_getaddr_mq(SMIQ *sqp, void **addr);
bool smi_receive_release_mq(SMIQ *sqp);

#endif /* _SMI_MQ_H_ */
