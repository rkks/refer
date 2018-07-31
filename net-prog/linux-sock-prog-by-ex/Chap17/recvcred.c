/* recvcred.c
 *
 * Send a file descriptor :
 */
#include "common.h"

/*
 * Receive Data & Credentials :
 *
 * ARGUMENTS:
 *  s       Socket to read from
 *  credp   Ptr to receiving area for cred.
 *  buf     Ptr to receiving buffer for data
 *  bufsiz  Maximum # of bytes for buffer
 *  addr    Ptr to buffer to receive peer
 *          address (or NULL)
 *  alen    Ptr to Maximum byte length
 *          (updated with actual length
 *          upon return.)
 *
 * RETURNS:
 *  >=0     Data bytes read
 *  -1      Failed: check errno
 *
 * NOTES:
 *  The value -1 is returned with errno set
 *  to ENOENT, if data is returned without
 *  any credentials.
 */
int
recv_cred(
  int s,                          /* Socket */
  struct ucred *credp, /* Credential buffer */
  void *buf,       /* Receiving Data buffer */
  unsigned bufsiz,   /* Recv. Data buf size */
  void *addr,      /* Received Peer address */
  socklen_t *alen) {  /* Ptr to addr length */

    int z;
    struct msghdr msgh;   /* Message header */
    struct iovec iov[1];      /* I/O vector */
    struct cmsghdr *cmsgp = NULL;
    char mbuf[CMSG_SPACE(sizeof *credp)];

    /*
     * Zero out message areas :
     */
    memset(&msgh,0,sizeof msgh);
    memset(mbuf,0,sizeof mbuf);
	
    /*
     * Establish datagram address (if any) :
     */
    msgh.msg_name = addr;
    msgh.msg_namelen = alen ? *alen : 0;

    /*
     * Point to our I/O vector of 1 element:
     */
    msgh.msg_iov = iov;
    msgh.msg_iovlen = 1;

    /*
     * Initialize our 1 I/O element vector :
     */
    iov[0].iov_base = buf;
    iov[0].iov_len = bufsiz;

    /*
     * Initialize control structure :
     */
    msgh.msg_control = mbuf;
    msgh.msg_controllen = sizeof mbuf;

    /*
     * Receive a message :
     */
    do  {
        z = recvmsg(s,&msgh,0);
    } while ( z == -1 && errno == EINTR );

    if ( z == -1 )
        return -1;  /* Failed: check errno */

    /*
     * If ptr alen is non-NULL, return the
     * returned address length (datagrams):
     */
    if ( alen )
        *alen = msgh.msg_namelen;

    /*
     * Walk the list of control messages:
     */
    for ( cmsgp = CMSG_FIRSTHDR(&msgh);
        cmsgp != NULL;
        cmsgp = CMSG_NXTHDR(&msgh,cmsgp) ) {

        if ( cmsgp->cmsg_level == SOL_SOCKET
        && cmsgp->cmsg_type == SCM_CREDENTIALS ) {

            /*
             * Pass back credentials struct:
             */
            *credp = *
                (struct ucred *) CMSG_DATA(cmsgp);

            return z; /* # of data bytes read */
        }
    }

    /*
     * There were no credentials found. An error
     * is returned here, since this applicaton
     * insists on getting credentials.
     */
    errno = ENOENT;
    return -1;
}
