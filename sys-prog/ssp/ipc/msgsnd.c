#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "ssp.h"

int main (int argc, char **argv)
{
	int qid;
	key_t key;
	size_t msgsz;
	long msgtyp;
	struct msgbuf *buf;

	if (argc != 4)
		err_quit ("Usage: msgsnd path type size");

	msgtyp = atol (argv [2]);
	msgsz = atol (argv [3]);

	if ((key = ftok (argv [1], 0x42)) == -1)
		err_msg ("ftok failed");
	if ((qid = msgget (key, MSG_W)) == -1)
		err_msg ("msgget failed");

	if ((buf = malloc (sizeof (long) + msgsz)) == NULL)
		err_msg ("malloc failed");

	buf -> mtype = msgtyp;

	if (msgsnd (qid, buf, msgsz, 0) == -1)
		err_msg ("msgsnd failed");

	return (0);
}
