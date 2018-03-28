/*
	Simple Messaging Interface
	AUP2, Sec. 7.03.2

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
#ifndef _SMI_H_
#define _SMI_H_

/*
	Messages must begin with a long mtype.
*/

/*[]*/
struct smi_msg {
	long smi_mtype; /* must be first */
	struct client_id {
		long c_id1;
		long c_id2;
	} smi_client;
	char smi_data[1];
};
/*[]*/
typedef void *SMIQ;
typedef enum {SMI_SERVER, SMI_CLIENT} SMIENTITY;
#define SERVER_NAME_MAX 50

SMIQ *smi_open(const char *name, SMIENTITY entity, size_t msgsize);
bool smi_close(SMIQ *sqp);
bool smi_send_getaddr(SMIQ *sqp, struct client_id *client, void **addr);
bool smi_send_release(SMIQ *sqp);
bool smi_receive_getaddr(SMIQ *sqp, void **addr);
bool smi_receive_release(SMIQ *sqp);
#endif /* _SMI_H_ */
