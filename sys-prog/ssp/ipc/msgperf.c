#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include "ssp.h"

#define MSGMAX (2048 + sizeof (long))
#define BUFFER_SIZE 2000
#define NUM_ITERS 50000

int main (int argc, char **argv)
{
	int i;
	pid_t pid;
	int qid;
	struct msgbuf *buf;

	if (argc != 1)
		err_quit ("Usage: msgperf");

	if ((qid = msgget (IPC_PRIVATE, MSG_R | MSG_W | IPC_CREAT)) == -1)
		err_msg ("msgget failed");

	if ((buf = malloc (MSGMAX)) == NULL)
		err_msg ("malloc failed");

	buf -> mtype = 1;

	switch (pid = fork ()) {
		case -1:
			err_msg ("fork failed");

		case 0:
			for (;;)
				if (msgrcv (qid, buf, MSGMAX, 1, 0) == -1)
					err_msg ("msgrcv failed");

		default:
			for (i = 0; i < NUM_ITERS; i++)
				if (msgsnd (qid, buf, BUFFER_SIZE, 0) == -1)
					err_msg ("msgsnd failed");

			kill (pid, SIGINT);
			waitpid (pid, NULL, 0);
			break;
	}

	msgctl (qid, IPC_RMID, NULL);

	return (0);
}
