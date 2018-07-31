/* common.h
 *
 * Source common to all modules:
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <sys/uio.h>

#ifndef TRUE
#define TRUE    1
#define FALSE   0
#endif

extern void bail(const char *on_what);

extern int recv_fd(int s);

extern int reqport(int port);

extern int send_fd(
    int s,int fd,
    void *addr,socklen_t alen);

extern int recv_cred(
    int s,struct ucred *credp,
    void *buf,unsigned bufsiz,
    void *addr,socklen_t *alen);

