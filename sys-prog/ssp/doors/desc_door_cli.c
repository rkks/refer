#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <door.h>
#include "ssp.h"

static char buf [BUF_SIZE];
static char res_buf [BUF_SIZE];

int main (int argc, char **argv)
{
	int fd;
	int door_fd;
	ssize_t n;
	door_arg_t door_args;

	if (argc != 3)
		err_quit ("Usage: desc_door_cli1 door_path file_path");

	if ((door_fd = open (argv [1], O_RDWR)) == -1)
		err_msg ("Can't open door");

	door_args.data_ptr = argv [2];
	door_args.data_size = strlen (argv [2]) + 1;
	door_args.desc_ptr = NULL;
	door_args.desc_num = 0;
	door_args.rbuf = res_buf;
	door_args.rsize = BUF_SIZE;

	if (door_call (door_fd, &door_args) == -1)
		err_msg ("door_call failed");

	if (door_args.data_size != 0)
		err_quit ("%.*s", door_args.data_size, door_args.data_ptr);
	else if (door_args.desc_ptr == NULL)
		err_quit ("desc_ptr is NULL");
	else if (door_args.desc_num != 1)
		err_quit ("desc_num = %d", door_args.desc_num);
	else if (!(door_args.desc_ptr -> d_attributes & DOOR_DESCRIPTOR))
		err_quit ("d_attributes = %X", door_args.desc_ptr -> d_attributes);

	fd = door_args.desc_ptr -> d_data.d_desc.d_descriptor;
	while ((n = read (fd, buf, BUF_SIZE)) > 0)
		if (writen (STDOUT_FILENO, buf, n) == -1)
			err_msg ("writen failed");

	if (n == -1)
		err_msg ("read failed");

	return (0);
}
