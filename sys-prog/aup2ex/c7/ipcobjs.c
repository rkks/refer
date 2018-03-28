/*
	Sys V IPC-object test program
	AUP2, Sec. 7.04 (not in book)

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
#include <sys/msg.h>
#include <sys/sem.h>

#define MQNAME "tempqnamea"
#define MQNAME2 "tempqname2a"

int main(void)
{
	int mqid, semid;
	key_t key;
	int i;
	struct msg {
	    long mtype;	/* message type */
	    char mtext[100000];	/* message text */
	} msg;
	struct msqid_ds msginfo;

	(void)close(open(MQNAME, O_WRONLY | O_CREAT, 0));
	ec_neg1( key = ftok(MQNAME, 1) )
	if ((mqid = msgget(key, 0666)) != -1)
		ec_neg1( msgctl(mqid, IPC_RMID, &msginfo) )
	ec_neg1( mqid = msgget(key, 0666 | IPC_CREAT | IPC_EXCL) )
	ec_neg1( msgctl(mqid, IPC_STAT, &msginfo) )
	printf("bytes = %ld\n", (long)msginfo.msg_qbytes);
	msg.mtype = 1;
	for (i = msginfo.msg_qbytes; i > 0; i--) {
		if (msgsnd(mqid, &msg, i, IPC_NOWAIT) != -1) {
			printf("Size %d worked\n", i);
			exit(EXIT_SUCCESS);
		}
		if (errno != EINVAL) {
			printf("Failed on size %d\n", i);
			EC_FAIL
		}
	}
/*
	for (i = 1; i < 1000000; i++)
		if (msgsnd(mqid, &msg, msginfo.msg_qbytes, IPC_NOWAIT) == -1) {
			if (errno == EAGAIN) {
				printf("queue full -- i = %d\n", i);
				exit(EXIT_SUCCESS);
			}
			else {
				printf("failed on msg #%d\n", i);
				EC_FAIL
			}
		}
*/
	(void)close(open(MQNAME2, O_WRONLY | O_CREAT, 0));
	ec_neg1( key = ftok(MQNAME2, 1) )
	ec_neg1( semid = semget(key, 1, 0666 | IPC_CREAT) )
	ec_neg1( semid = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT) )

	system("ipcs");
	exit(EXIT_SUCCESS);

EC_CLEANUP_BGN
	exit(EXIT_FAILURE);
EC_CLEANUP_END
}
