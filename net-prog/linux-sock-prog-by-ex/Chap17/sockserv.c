/* sockserv.c :
 *
 * This simple server will serve up a socket
 * to a valid recipient:
 */
#include "common.h"
#include <pwd.h>

/*
 * Check user's access :
 *
 * RETURNS:
 *  ptr     To (struct passwd *) if granted.
 *  NULL    Access is to be denied.
 */
static struct passwd *
check_access(
  int port,        /* Port being requested */
  struct ucred *pcred, /* User credentials */
  char **uidlist) { /* List of valid users */
    int x;
    struct passwd *pw; /* User passwd entry */

    /*
     * Look the user's uid # up in the
     * /etc/password file:
     */
    if ( (pw = getpwuid(pcred->uid)) != 0 ) {
        /*
         * Make sure request is coming from
         * one of the acceptable users:
         */
        for ( x=0; uidlist[x]; ++x ) 
            if ( !strcmp(uidlist[x],pw->pw_name) )
                break;
        if ( !uidlist[x] ) 
            pw = 0;       /* Access denied */
    }

    /*
     * Screen the port #. For this demo,
     * only port 80 is permitted.
     */
    if ( port != 80 )
        pw = 0;           /* Access denied */

    return pw;   /* NULL or ptr if granted */
}

/*
 * Access has been granted: send socket
 * to client.
 *
 * ARGUMENTS:
 *  c       Client socket
 *  port    Port requested
 *
 * RETURNS:
 *  0       Success
 *  -1      Failed: check errno
 */
static int
grant_access(int c,int port) {
    int z;
    int fd = -1;           /* New socket fd */
    int b = TRUE;           /* Boolean TRUE */
    struct sockaddr_in addr;/* work address */

    /*
     * Create a new TCP/IP socket:
     */
    fd = socket(PF_INET,SOCK_STREAM,0);
    if ( fd == -1 ) {
        perror("socket(2)");
        goto errxit;
    }

    /*
     * Turn on SO_REUSEADDR :
     */
    z = setsockopt(fd,SOL_SOCKET,
        SO_REUSEADDR,&b,sizeof b);
    if ( z == -1 )
        bail("setsockopt(2)");

    /*
     * Create the address to bind:
     */
    memset(&addr,0,sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_port = ntohs(port);
    addr.sin_addr.s_addr = ntohl(INADDR_ANY);

    /*
     * Bind the requested address:
     */
    z = bind(fd,
        (struct sockaddr *)&addr,
        sizeof addr);
    if ( z == -1 ) {
        fprintf(stderr,"%s: binding port %d\n",
            strerror(errno),port);
        goto errxit;
    }

    /*
     * Send the fd back to the
     * requesting client:
     */
    z = send_fd(c,fd,NULL,0);
    if ( z == -1 ) {
        perror("send_fd()");
        goto errxit;
    }

    close(fd);         /* finished with fd */
    return 0;                   /* Success */

errxit:
    z = errno;               /* Save errno */
    if ( fd )
        close(fd);       /* Release socket */
    errno = z;            /* Restore errno */
    return -1;
}

/*
 * Process a connected client's request:
 */
void
process_client(
  int c,                   /* Client socket */
  char **uidlist     /* List of valid users */
  ) {
    int z;
    int er;         /* Captured errno value */
    int b = TRUE;          /* Boolean: True */
    struct ucred cred;  /* Clnt credentials */
    short port;     /* Port being requested */
    struct passwd *pw; /* User passwd entry */

    /*
     * Now make certain that we can receive
     * credentials on this socket:
     */
    z = setsockopt(c,
        SOL_SOCKET,
        SO_PASSCRED,
        &b,
        sizeof b);
    if ( z )
        bail("setsockopt(2)");

    /*
     * Receive a request with the
     * user credentials:
     */
    z = recv_cred(c,         /* socket */
        &cred, /* Returned credentials */
        &port,      /* Returned port # */
        sizeof port,   /* Size of data */
        NULL, 0); /* no socket address */

    if ( z == -1 )
        perror("recv_cred()");

    /*
     * Now check access. If pw is returned
     * as non-NULL, the request is OK.
     */
    pw = check_access(port,&cred,uidlist);

    if ( pw ) {
        if ( !grant_access(c,port) ) {
            close(c);
            return;   /* request sucessful */
        }
        /* Failed */
        er = errno;      /* Capture reason */
    } else {
        /*
         * Userid was not known, or not in
         * the priviledged list:
         */
        er = EACCES;        /* Perm denied */
    }

    /*
     * Control reaches here if the
     * request failed or is denied:
     *
     * Here we simply send the error
     * code back without a file
     * descriptor. This lack of a fd
     * will be detected by the client.
     */
    do  {
        z = write(c,&er,sizeof er);
    } while ( z == -1 && errno == EINTR );

    if ( z == -1 )
        perror("write(2)");
}

/*
 * Main program :
 */
int
main(int argc,char **argv) {
    int z;
    int s;             /* Server UDP socket */
    int c;                 /* Client socket */
    int alen;             /* Address length */
    struct sockaddr_un a_srvr;  /* Server @ */
    struct sockaddr_un a_clnt;  /* Client @ */

    /*
     * Make sure we have a userid specified:
     */
    if ( argc < 2 ) {
        fputs("Must have at least 1 userid.\n",
            stderr);
        exit(1);
    }

    /*
     * Create a Unix Socket:
     */
    s = socket(PF_LOCAL,SOCK_STREAM,0);
    if ( s == -1 )
        bail("socket(2)");

    /*
     * Create abstract address:
     */
    memset(&a_srvr,0,sizeof a_srvr);
    a_srvr.sun_family = AF_LOCAL;
    strncpy(a_srvr.sun_path,
        "zSOCKET-SERVER",
        sizeof a_srvr.sun_path-1);
    a_srvr.sun_path[0] = 0;

    /*
     * Bind the server address:
     */
    z = bind(s,
        (struct sockaddr *)&a_srvr,
        sizeof a_srvr);
    if ( z == -1 )
        bail("bind(2)");    

    /*
     * Now make this a listening socket:
     */
    z = listen(s,10);
    if ( z == -1 )
        bail("listen(2)");

    /*
     * Now process requests:
     */
    for (;;) {
        /*
         * Wait for a connect:
         */
        alen = sizeof a_clnt;
        c = accept(s,&a_clnt,&alen);
        if ( c == -1 )
            bail("accept(2)");

        /*
         * Process this request:
         */
        process_client(c,argv+1);
        close(c);
    }

    return 0;
}
