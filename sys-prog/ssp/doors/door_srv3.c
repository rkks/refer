#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <door.h>
#include <pthread.h>
#include <limits.h>
#include "ssp.h"

static int door_fd = -1;
static pthread_mutex_t door_lock;
static pthread_cond_t door_cv;

static void ssp_create (door_info_t *info);
static void *ssp_thread (void *arg);
static void server_proc (void *cookie, char *argp, size_t arg_size,
	door_desc_t *dp, uint_t n_desc);

int main (int argc, char **argv)
{
	int tmp_fd;

	if (argc != 2)
		err_quit ("Usage: door_srv3 path");

	door_server_create (ssp_create);

	if (pthread_mutex_lock (&door_lock) != 0)
		err_msg ("Can't lock door");
	if ((door_fd = door_create (server_proc, NULL, DOOR_PRIVATE)) == -1)
		err_msg ("door_create failed");
	if (pthread_cond_signal (&door_cv) == -1)
		err_msg ("pthread_cond_signal failed");
	if (pthread_mutex_unlock (&door_lock) != 0)
		err_msg ("Can't unlock door");

	unlink (argv [1]);
	if ((tmp_fd = creat (argv [1], FILE_PERMS)) == -1)
		err_msg ("creat failed");
	close (tmp_fd);
	if (fattach (door_fd, argv [1]) == -1)
		err_msg ("fattach failed");

	for (;;)
		pause ();
}

static void ssp_create (door_info_t *info)
{
	pthread_t thread_id;
	pthread_attr_t attr;

	if (pthread_attr_init (&attr) != 0)
		err_msg ("pthread_attr_init failed");
	if (pthread_attr_setscope (&attr, PTHREAD_SCOPE_SYSTEM) != 0)
		err_msg ("pthread_attr_setscope failed");
	if (pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_DETACHED) != 0)
		err_msg ("pthread_attr_setdetachstate failed");
	if (PTHREAD_STACK_MIN > 8192) {
		if (pthread_attr_setstacksize (&attr, PTHREAD_STACK_MIN) != 0)
			err_msg ("pthread_attr_setstacksize failed");
	}
	else {
		if (pthread_attr_setstacksize (&attr, 8192) != 0)
			err_msg ("pthread_attr_setstacksize failed");
	}

	if (pthread_create (&thread_id, &attr, ssp_thread,
		(void *) info -> di_proc) != 0) {
		err_msg ("pthread_create failed");
	}

	if (pthread_attr_destroy (&attr) != 0)
		err_msg ("pthread_attr_destroy failed");

	printf ("ssp_create: Created server thread %d\n", thread_id);
}

static void *ssp_thread (void *func)
{
	if (pthread_setcancelstate (PTHREAD_CANCEL_DISABLE, NULL) != 0)
		err_msg ("pthread_setcancelstate failed");

	if (pthread_mutex_lock (&door_lock) != 0)
		err_msg ("Can't lock door");
	while (door_fd == -1) {
		if (pthread_cond_wait (&door_cv, &door_lock) != 0)
			 err_msg ("pthread_cond_wait failed");
	}
	if (pthread_mutex_unlock (&door_lock) != 0)
		err_msg ("Can't unlock door");

	if (func == (void *) server_proc) {
		if (door_bind (door_fd) == -1)
			err_msg ("door_bind failed");
		if (door_return (NULL, 0, NULL, 0) == -1)
			err_msg ("door_return failed");
	}
	else
		err_quit ("ssp_thread: %p: Unknown function", func);

	return (NULL);
}

static void server_proc (void *cookie, char *argp, size_t arg_size,
	door_desc_t *dp, uint_t n_desc)
{
	long arg;
	long res;

	arg = *((long *) argp);
	res = arg * arg;

	printf ("Server proc called, thread ID = %d, arg = %ld\n",
		pthread_self (), arg);
	sleep (5);
	printf ("Server proc returning, thread ID = %d, res = %ld\n",
		pthread_self (), res);

	if (door_return ((char *) &res, sizeof (long), NULL, 0) == -1)
		err_msg ("door_return failed");
}
