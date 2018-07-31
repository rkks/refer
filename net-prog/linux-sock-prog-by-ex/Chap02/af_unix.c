/* af_unix.c:
 *
 * AF_UNIX Socket Example :
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>

/*
 * This function reports the error and
 * exits back to the shell :
 */
static void
bail(const char *on_what) {
    perror(on_what);
    exit(1);
}

int
main(int argc,char **argv,char **envp) {
    int z;           /* Status return code */
    int sck_unix;                /* Socket */
    struct sockaddr_un adr_unix;/* AF_UNIX */
    int len_unix;                /* length */
    const char pth_unix[]      /* pathname */
        = "/tmp/my_sock";

    /*
     * Create a AF_UNIX (aka AF_LOCAL) socket:
     */
    sck_unix = socket(AF_UNIX,SOCK_STREAM,0);

    if ( sck_unix == -1 )
        bail("socket()");

    /*
     * Here we remove the pathname for the
     * socket, in case it existed from a
     * prior run.. ignore errors (it might
     * not exist).
     */
    unlink(pth_unix);

    /*
     * Form an AF_UNIX Address :
     */
    memset(&adr_unix,0,sizeof adr_unix);

    adr_unix.sun_family = AF_UNIX;

    strncpy(adr_unix.sun_path,pth_unix,
        sizeof adr_unix.sun_path-1)
        [sizeof adr_unix.sun_path-1] = 0;

    len_unix = SUN_LEN(&adr_unix);

    /*
     * Now bind the address to the socket :
     */
    z = bind(sck_unix,
        (struct sockaddr *)&adr_unix,
        len_unix);

    if ( z == -1 )
        bail("bind()");

    /*
     * Display all of our bound sockets :
     */
    system("netstat -pa --unix 2>/dev/null| "
        "sed -n '/^Active UNIX/,/^Proto/p;"
        "/af_unix/p'");

    /*
     * Close and unlink our socket path :
     */
    close(sck_unix);
    unlink(pth_unix);

    return 0;
}
