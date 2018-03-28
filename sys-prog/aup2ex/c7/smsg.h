/*
	Header for FIFO example
	AUP2, Sec. 7.02.2

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
#ifndef _SMSG_H_
#define _SMSG_H_

/*[hdr]*/
#define SERVER_FIFO_NAME "fifo_server"

struct simple_message {
	pid_t sm_clientpid;
	char sm_data[200];
};
/*[]*/

bool make_fifo_name(pid_t pid, char *name, size_t name_max);

#endif /* _SMSG_H_ */
