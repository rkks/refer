/* recvfd.c
 *
 * Receive a file descriptor :
 */
#include "common.h"

/*
 * Receive a file descriptor from the
 * socket.
 *
 * ARGUMENTS:
 *  s       Socket to receive file
 *          descriptor on.
 *
 * RETURNS:
 *  >= 0    Received file descriptor
 *  -1      Failed: See errno
 */
int
recv_fd(int s) {
    int z;
    struct msghdr msgh;  /* Message header */
    struct iovec iov[1];     /* I/O vector */
    struct cmsghdr *cmsgp = NULL;
    char buf[CMSG_SPACE(sizeof(int))];
    char dbuf[80];    /* Small data buffer */

    /*
     * Initialize structures to zero bytes:
     */
    memset(&msgh,0,sizeof msgh);
    memset(buf,0,sizeof buf);

    /*
     * No socket addresses are used here:
     */
    msgh.msg_name = NULL;
    msgh.msg_namelen = 0;

    /*
     * Install our I/O vector :
     */
    msgh.msg_iov = iov;
    msgh.msg_iovlen = 1;

    /*
     * Initilize I/O vector to read data
     * into our dbuf[] array :
     */
    iov[0].iov_base = dbuf;
    iov[0].iov_len = sizeof dbuf;

    /*
     * Load control data into buf[] :
     */
    msgh.msg_control = buf;
    msgh.msg_controllen = sizeof buf;

    /*
     * Receive a message :
     */
    do  {
        z = recvmsg(s,&msgh,0);
    } while ( z == -1 && errno == EINTR );

    if ( z == -1 )
        return -1;    /* Failed: see errno */

    /*
     * Walk the control structure looking for
     * a file descriptor :
     */
    for ( cmsgp = CMSG_FIRSTHDR(&msgh);
        cmsgp != NULL;
        cmsgp = CMSG_NXTHDR(&msgh,cmsgp) ) {

        if ( cmsgp->cmsg_level == SOL_SOCKET
        && cmsgp->cmsg_type == SCM_RIGHTS ) {
            /*
             * File descriptor found :
             */
            return *(int *) CMSG_DATA(cmsgp);
        }
    }

    /*
     * No file descriptor was received:
     * If we received 4 bytes, assume we
     * received an errno value... then
     * set errno from our received data.
     */
    if ( z == sizeof (int) )
        errno = *(int *)dbuf; /* Rcvd errno */
    else
        errno = ENOENT;    /* Default errno */

    return -1; /* Return failure indication */
}
