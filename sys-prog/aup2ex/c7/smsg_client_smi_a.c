/*
	SMI-based client
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
#include "defs.h"
#include "smsg_smi.h"


/* Client */

/*[main]*/
int main(int argc, char *argv[])
{
	SMIQ *sqp;
	struct smi_msg *msg;
	int i;

	char *work[] = {
		"applesauce",
		"tiger",
		"mountain",
		NULL
	};

	printf("client %ld started\n", (long)getpid());
	ec_null( sqp = smi_open(SERVER_NAME, SMI_CLIENT, DATA_SIZE) )
	for (i = 0; work[i] != NULL; i++) {
		ec_false( smi_send_getaddr(sqp, NULL, (void **)&msg) )
		strcpy(msg->smi_data, work[i]);
		ec_false( smi_send_release(sqp) )
		ec_false( smi_receive_getaddr(sqp, (void **)&msg) )
		printf("client %ld: %s --> %s\n", (long)getpid(),
		  work[i], msg->smi_data);
		ec_false( smi_receive_release(sqp) )
	}
	ec_false( smi_close(sqp) )
	printf("Client %ld done\n", (long)getpid());
	exit(EXIT_SUCCESS);

EC_CLEANUP_BGN
	exit(EXIT_FAILURE);
EC_CLEANUP_END
}
