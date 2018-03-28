#include <stdio.h>
#include <unistd.h>
#include <sys/sendfile.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "ssp.h"

static char header_buf [LINE_LEN];
static char trailer_buf [LINE_LEN];

int main (int argc, char **argv)
{
	struct stat buf;
	int out_fd;
	ssize_t res;
	size_t xferred;
	sendfilevec_t vec [3];

	if (argc != 3)	
		err_quit ("Usage: sendfilev source_file dest_file");

	vec [0].sfv_fd = SFV_FD_SELF;
	vec [0].sfv_flag = 0;
	vec [0].sfv_off = (off_t) header_buf;
	vec [0].sfv_len = snprintf (header_buf, LINE_LEN, "*** Header ***\n");

	if ((vec [1].sfv_fd = open (argv [1], O_RDONLY)) == -1)
		err_msg ("Can't open %s", argv [1]);
	if (fstat (vec [1].sfv_fd, &buf) == -1)
		err_msg ("Can't stat %s", argv [1]);
	vec [1].sfv_flag = 0;
	vec [1].sfv_off = 0;
	vec [1].sfv_len = (buf.st_size > 100) ? 100 : buf.st_size;

	vec [2].sfv_fd = SFV_FD_SELF;
	vec [2].sfv_flag = 0;
	vec [2].sfv_off = (off_t) trailer_buf;
	vec [2].sfv_len = snprintf (trailer_buf, LINE_LEN, "\n*** Trailer ***\n");

	if ((out_fd = open (argv [2], O_WRONLY | O_TRUNC | O_CREAT,
		FILE_PERMS)) == -1) {
		err_msg ("Can't open %s", argv [2]);
	}

	if ((res = sendfilev (out_fd, vec, 3, &xferred)) == -1)
		err_msg ("sendfilev failed");
	else
		printf ("Wrote %d bytes\n", res);

	close (vec [1].sfv_fd);
	close (out_fd);

	return (0);
}
