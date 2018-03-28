#ifndef _SMI_MSG_H_
#define _SMI_MSG_H_

#include "smi.h"

/*
	Following not in book. Note that as of OS X 10.2, Darwin does not have Sys V msgs.
*/
#if (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 4) || defined(FREEBSD)
#define HAVE_SYSVMSG
#endif

/*
	Simple Messaging Interface - System V msg version
*/
SMIQ *smi_open_msg(const char *name, SMIENTITY entity, size_t msgsize);
bool smi_close_msg(SMIQ *sqp);
bool smi_send_getaddr_msg(SMIQ *sqp, struct client_id *client,
  void **addr);
bool smi_send_release_msg(SMIQ *sqp);
bool smi_receive_getaddr_msg(SMIQ *sqp, void **addr);
bool smi_receive_release_msg(SMIQ *sqp);

#endif /* _SMI_MSG_H_ */
