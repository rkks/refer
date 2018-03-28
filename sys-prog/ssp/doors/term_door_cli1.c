#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <door.h>
#include <signal.h>
#include <errno.h>
#include "ssp.h"

static volatile sig_atomic_t flag;

static void sig_chld (int sig);

int main (int argc, char **argv)
{
	int fd;
	long num;
	long res;
	pid_t pid;
	door_arg_t door_args;

	if (argc != 3)
		err_quit ("Usage: term_door_cli1 path number");

	if ((fd = open (argv [1], O_RDWR)) == -1)
		err_msg ("Can't open door");

	num = atol (argv [2]);

	door_args.data_ptr = (char *) &num;
	door_args.data_size = sizeof (long);
	door_args.desc_ptr = NULL;
	door_args.desc_num = 0;
	door_args.rbuf = (char *) &res;
	door_args.rsize = sizeof (long);

	if (sigset (SIGCHLD, sig_chld) == SIG_ERR)
		err_msg ("sigset failed");

	if ((pid = fork ()) == -1)
		err_msg ("fork failed");
	else if (pid == 0) {
		sleep (2);
		_exit (0);
	}

	for (;;) {
		printf ("Calling door_call\n");
		if (door_call (fd, &door_args) == 0)
			break;
		if ((errno == EINTR) && (flag == 1))
			flag = 0;
		else
			err_msg ("door_call failed");
	}

	printf ("Result = %ld\n", res);

	return (0);
}

static void sig_chld (int sig)
{
	flag = 1;
}
