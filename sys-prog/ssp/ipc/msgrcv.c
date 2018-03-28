#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "ssp.h"

#define MSGMAX (2048 + sizeof (long))

int main (int argc, char **argv)
{
	int c;
	int qid;
	key_t key;
	int msgflg;
	int err_flag;
	long msgtyp;
	ssize_t n;
	struct msgbuf *buf;

	msgflg = MSG_R;
	msgtyp = 0;
	opterr = 0;
	err_flag = 0;

	while ((c = getopt (argc, argv, "nt:")) != EOF) {
		switch (c) {
			case 'n':
				msgflg |= IPC_NOWAIT;
				break;

			case 't':
				msgtyp = atol (optarg);
				break;

			default:
				err_flag++;
				break;
		}
	}

	if (optind != argc - 1)
		err_flag++;
	if (err_flag)
		err_quit ("Usage: msgrcv [-n] [-t type] path");

	if ((key = ftok (argv [optind], 0x42)) == -1)
		err_msg ("ftok failed");
	if ((qid = msgget (key, msgflg)) == -1)
		err_msg ("msgget failed");

	if ((buf = malloc (MSGMAX)) == NULL)
		err_msg ("malloc failed");

	if ((n = msgrcv (qid, buf, MSGMAX, msgtyp, msgflg)) == -1)
		err_msg ("msgrcv failed");
	printf ("Read %d bytes, message type = %ld\n", n, buf -> mtype);

	return (0);
}
