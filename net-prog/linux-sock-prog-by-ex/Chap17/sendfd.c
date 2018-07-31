/* sendfd.c
 *
 * Send a file descriptor :
 */
#include "common.h"

/*
 * Send a file descriptor via socket:
 *
 * ARGUMENTS:
 *  s       Socket to send on
 *  fd      Open file descriptor to send
 *  addr    Ptr to UDP address or NULL
 *  alen    Size of addr or zero
 *
 *
 * RETURNS:
 *  0       Successful
 *  -1      Failed: check errno
 */
int
send_fd(int s,int fd,void *addr,socklen_t alen) {
    int z;
    struct msghdr msgh;  /* Message header */
    struct iovec iov[1];     /* I/O vector */
    struct cmsghdr *cmsgp = NULL;
    char buf[CMSG_SPACE(sizeof fd)];
    int er=0;   /* "No error" code of zero */

    /*
     * Clear message areas :
     */
    memset(&msgh,0,sizeof msgh);
    memset(buf,0,sizeof buf);

    /*
     * Supply socket address (if any) :
     */
    msgh.msg_name = addr;
    msgh.msg_namelen = alen;

    /*
     * Install our I/O vector :
     */
    msgh.msg_iov = iov;
    msgh.msg_iovlen = 1;

    /*
     * Initialize the I/O vector to send
     * the value in "er" (which is zero).
     * This is done because data must be
     * transmitted to send the fd.
     */
    iov[0].iov_base = &er;
    iov[0].iov_len = sizeof er;

    /*
     * Establish control buffer:
     */
    msgh.msg_control = buf;
    msgh.msg_controllen = sizeof buf;

    /*
     * Configure the message to send
     * a file descriptor :
     */
    cmsgp = CMSG_FIRSTHDR(&msgh);
    cmsgp->cmsg_level = SOL_SOCKET;
    cmsgp->cmsg_type = SCM_RIGHTS;
    cmsgp->cmsg_len = CMSG_LEN(sizeof fd);

    /*
     * Install the file descriptor value :
     */
    *((int *)CMSG_DATA(cmsgp)) = fd;
    msgh.msg_controllen = cmsgp->cmsg_len;

    /*
     * Send it to the client process :
     */
    do  {
        z = sendmsg(s,&msgh,0);
    } while ( z == -1 && errno == EINTR );

    return z == -1 ? -1 : 0;
}
