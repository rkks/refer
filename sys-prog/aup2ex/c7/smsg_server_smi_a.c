/*
	SMI-based server
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

/* Server */

/*[main]*/
int main(void)
{
	SMIQ *sqp;
	struct smi_msg *msg_in, *msg_out;
	int i;

	printf("server started\n");
	ec_null( sqp = smi_open(SERVER_NAME, SMI_SERVER, DATA_SIZE) )
	while (true) {
		ec_false( smi_receive_getaddr(sqp, (void **)&msg_in) )
		ec_false( smi_send_getaddr(sqp, &msg_in->smi_client,
		  (void **)&msg_out) )
		for (i = 0; msg_in->smi_data[i] != '\0'; i++)
			msg_out->smi_data[i] = toupper(msg_in->smi_data[i]);
		msg_out->smi_data[i] = '\0';
		ec_false( smi_receive_release(sqp) )
		ec_false( smi_send_release(sqp) )
	}
	/* never actually get here */
	ec_false( smi_close(sqp) )
	exit(EXIT_SUCCESS);

EC_CLEANUP_BGN
	exit(EXIT_FAILURE);
EC_CLEANUP_END
}
