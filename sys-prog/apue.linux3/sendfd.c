#include	<sys/types.h>
#include	<sys/socket.h>		/* struct msghdr */
#include	<sys/uio.h>			/* struct iovec */
#include	<errno.h>
#include	<stddef.h>
#include	"ourhdr.h"

static struct cmsghdr	*cmptr = NULL;	/* buffer is malloc'ed first time */
#define	CONTROLLEN	(sizeof(struct cmsghdr) + sizeof(int))
		/* size of control buffer to send/recv one file descriptor */

/* Pass a file descriptor to another process.
 * If fd<0, then -fd is sent back instead as the error status. */

int
send_fd(int clifd, int fd)
{
	struct iovec	iov[1];
	struct msghdr	msg;
	char			buf[2];	/* send_fd()/recv_fd() 2-byte protocol */

	iov[0].iov_base = buf;
	iov[0].iov_len  = 2;
	msg.msg_iov     = iov;
	msg.msg_iovlen  = 1;
	msg.msg_name    = NULL;
	msg.msg_namelen = 0;
	if (fd < 0) {
		msg.msg_control    = NULL;
		msg.msg_controllen = 0;
		buf[1] = -fd;	/* nonzero status means error */
		if (buf[1] == 0)
			buf[1] = 1;	/* -256, etc. would screw up protocol */
	} else {
		if (cmptr == NULL && (cmptr = malloc(CONTROLLEN)) == NULL)
			return(-1);
		cmptr->cmsg_level  = SOL_SOCKET;
		cmptr->cmsg_type   = SCM_RIGHTS;
		cmptr->cmsg_len    = CONTROLLEN;
		msg.msg_control    = (caddr_t) cmptr;
		msg.msg_controllen = CONTROLLEN;
		*(int *)CMSG_DATA(cmptr) = fd;		/* the fd to pass */
		buf[1] = 0;		/* zero status means OK */
	}
	buf[0] = 0;			/* null byte flag to recv_fd() */

	if (sendmsg(clifd, &msg, 0) != 2)
		return(-1);
	return(0);
}
