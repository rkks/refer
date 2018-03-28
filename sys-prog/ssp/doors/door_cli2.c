#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <door.h>
#include "ssp.h"

int main (int argc, char **argv)
{
	int fd;
	long num;
	long res;
	door_arg_t door_args;

	if (argc != 3)
		err_quit ("Usage: door_cli2 path number");

	if ((fd = open (argv [1], O_RDWR)) == -1)
		err_msg ("Can't open door");

	num = atol (argv [2]);

	door_args.data_ptr = (char *) &num;
	door_args.data_size = sizeof (long);
	door_args.desc_ptr = NULL;
	door_args.desc_num = 0;
	door_args.rbuf = (char *) &res;
	door_args.rsize = sizeof (long) - 1;

	if (door_call (fd, &door_args) == -1)
		err_msg ("door_call failed");

	printf ("Address of res = %p, data_ptr = %p, rbuf = %p, rsize = %d\n",
		&res, door_args.data_ptr, door_args.rbuf, door_args.rsize);
	printf ("Result = %ld\n", *((long *) door_args.data_ptr));

	return (0);
}
