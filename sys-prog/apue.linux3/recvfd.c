#include	<sys/types.h>
#include	<sys/socket.h>		/* struct msghdr */
#include	<sys/uio.h>			/* struct iovec */
#include	<stddef.h>
#include	"ourhdr.h"

static struct cmsghdr	*cmptr = NULL;		/* malloc'ed first time */
#define	CONTROLLEN	(sizeof(struct cmsghdr) + sizeof(int))
		 /* size of control buffer to send/recv one file descriptor */
/* Receive a file descriptor from another process (a server).
 * In addition, any data received from the server is passed
 * to (*userfunc)(STDERR_FILENO, buf, nbytes).  We have a
 * 2-byte protocol for receiving the fd from send_fd(). */
int
recv_fd(int servfd, ssize_t (*userfunc)(int, const void *, size_t))
{
	int				newfd, nread, status;
	char			*ptr, buf[MAXLINE];
	struct iovec	iov[1];
	struct msghdr	msg;
	newfd = -2;

	status = -1;
	for ( ; ; ) {
		iov[0].iov_base = buf;
		iov[0].iov_len  = sizeof(buf);
		msg.msg_iov     = iov;
		msg.msg_iovlen  = 1;
		msg.msg_name    = NULL;
		msg.msg_namelen = 0;
		if (cmptr == NULL && (cmptr = malloc(CONTROLLEN)) == NULL)
			return(-1);
		msg.msg_control    = (caddr_t) cmptr;
		msg.msg_controllen = CONTROLLEN;

		if ( (nread = recvmsg(servfd, &msg, 0)) < 0)
			err_sys("recvmsg error");
		else if (nread == 0) {
			err_ret("connection closed by server");
			return(-1);
		}
			/* See if this is the final data with null & status.
			   Null must be next to last byte of buffer, status
			   byte is last byte.  Zero status means there must
			   be a file descriptor to receive. */
		for (ptr = buf; ptr < &buf[nread]; ) {
			if (*ptr++ == 0) {
				if (ptr != &buf[nread-1])
					err_dump("message format error");
 				status = *ptr & 255;
 				if (status == 0) {
					if (msg.msg_controllen != CONTROLLEN)
						err_dump("status = 0 but no fd");
					newfd = *(int *)CMSG_DATA(cmptr); /* new descriptor */
				} else
					newfd = -status;
				nread -= 2;
			}
		}
		if (nread > 0)
			if ((*userfunc)(STDERR_FILENO, buf, nread) != nread)
				return(-1);
		if (status >= 0)	/* final data has arrived */
			return(newfd);	/* descriptor, or -status */
	}
}
