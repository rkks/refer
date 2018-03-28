#include <stdio.h>
#include <unistd.h>
#include <sys/msg.h>
#include "ssp.h"

int main (int argc, char **argv)
{
	int qid;
	key_t key;

	if (argc != 2)
		err_quit ("Usage: msgrmq path");

	if ((key = ftok (argv [1], 0x42)) == -1)
		err_msg ("ftok failed");
	if ((qid = msgget (key, 0)) == -1)
		err_msg ("msgget failed");

	if (msgctl (qid, IPC_RMID, NULL) == -1)
		err_msg ("msgctl failed");

	return (0);
}
