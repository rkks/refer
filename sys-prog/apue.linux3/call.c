#include	"call.h"
#include	<sys/uio.h>		/* struct iovec */

/* Place the call by sending the "args" to the calling server,
 * and reading a file descriptor back. */

int
call(const char *args)
{
	int				csfd, len;
	struct iovec	iov[2];

					/* create connection to conn server */
	if ( (csfd = cli_conn(CS_CALL)) < 0)
		err_sys("cli_conn error");

	iov[0].iov_base = CL_CALL " ";
	iov[0].iov_len  = strlen(CL_CALL) + 1;
	iov[1].iov_base = (char *) args;
	iov[1].iov_len  = strlen(args) + 1;
							/* null at end of args always sent */
	len = iov[0].iov_len + iov[1].iov_len;
	if (writev(csfd, &iov[0], 2) != len)
		err_sys("writev error");

					/* read back descriptor */
					/* returned errors handled by write() */
	return( recv_fd(csfd, write) );
}
