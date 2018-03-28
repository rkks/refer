#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <door.h>
#include <pthread.h>
#include "ssp.h"

static void server_proc (void *cookie, char *argp, size_t arg_size,
	door_desc_t *dp, uint_t n_desc);
static void server_proc_cleanup (void *arg);

int main (int argc, char **argv)
{
	int door_fd;
	int tmp_fd;

	if (argc != 2)
		err_quit ("Usage: term_door_srv3 path");

	if ((door_fd = door_create (server_proc, NULL, 0)) == -1)
		err_msg ("door_create failed");

	unlink (argv [1]);
	if ((tmp_fd = creat (argv [1], FILE_PERMS)) == -1)
		err_msg ("creat failed");
	close (tmp_fd);
	if (fattach (door_fd, argv [1]) == -1)
		err_msg ("fattach failed");

	for (;;)
		pause ();
}

static void server_proc (void *cookie, char *argp, size_t arg_size,
	door_desc_t *dp, uint_t n_desc)
{
	long arg;
	long res;
	int prev_state;

	if (pthread_setcancelstate (PTHREAD_CANCEL_ENABLE, &prev_state) != 0)
		err_msg ("pthread_setcancelstate failed");
	pthread_cleanup_push (server_proc_cleanup, NULL);

	arg = *((long *) argp);
	res = arg * arg;

	sleep (5);
	pthread_cleanup_pop (0);
	if (pthread_setcancelstate (prev_state, NULL) != 0)
		err_msg ("pthread_setcancelstate failed");

	if (door_return ((char *) &res, sizeof (long), NULL, 0) == -1)
		err_msg ("door_return failed");
}

static void server_proc_cleanup (void *arg)
{
	printf ("Server proc cancelled, thread ID = %d\n", pthread_self ());
}
