#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "ssp.h"

int main (int argc, char **argv)
{
	int c;
	int qid;
	key_t key;
	int msgflg;
	int err_flag;

	msgflg = MSG_R | MSG_W | (MSG_R >> 3) | IPC_CREAT;
	opterr = 0;
	err_flag = 0;

	while ((c = getopt (argc, argv, "e")) != EOF) {
		switch (c) {
			case 'e':
				msgflg |= IPC_EXCL;
				break;

			default:
				err_flag++;
				break;
		}
	}

	if (optind != argc - 1)
		err_flag++;

	if (err_flag)
		err_quit ("Usage: msgcreat [-e] path");

	if ((key = ftok (argv [optind], 0x42)) == -1)
		err_msg ("ftok failed");
	if ((qid = msgget (key, msgflg)) == -1)
		err_msg ("msgget failed");

	printf ("Message queue ID = %d\n", qid);

	return (0);
}
