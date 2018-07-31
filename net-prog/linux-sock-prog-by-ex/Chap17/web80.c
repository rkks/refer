/* web80.c :
 *
 * This is an extremely simple web server:
 *
 * This program runs in two modes:
 *
 * 1. Standalone Mode:
 *  $ ./web80 standalone
 *
 *  In this mode, this program functions
 *  as a very simple standalone web server.
 *  However, it must run as root to bind
 *  to the web port 80.
 *
 * 2. Sock Server Mode:
 *  $ ./web80
 *
 *  In this mode, this program contacts
 *  the sockserv server to request a
 *  socket bound to port 80. If sockserv
 *  allows the request, it returns
 *  a port 80 socket. This allows this
 *  program to run without root and
 *  with no setuid requirement.
 */
#include "common.h"

int
main(int argc,char **argv) {
    int z;
    int s;             /* Web Server socket */
    int c;                 /* Client socket */
    int alen;             /* Address length */
    struct sockaddr_in a_web; /* Web Server */
    struct sockaddr_in a_cln;/* Client addr */
    int b = TRUE;       /* For SO_REUSEADDR */
    FILE *rx;                /* Read Stream */
    FILE *tx;               /* Write Stream */
    char getbuf[2048];        /* GET buffer */
    time_t td;       /* Current date & time */

    /*
     * If any arguments are present on the
     * command line, obtain the socket
     * without help from the server (run
     * in simple standalone mode):
     */
    if ( argc > 1 ) {
        /*
         * Standalone mode:
         */
        s = socket(PF_INET,SOCK_STREAM,0);
        if ( s == -1 )
            bail("socket(2)");

        /*
         * Web address on port 80:
         */
        memset(&a_web,0,sizeof a_web);
        a_web.sin_family = AF_INET;
        a_web.sin_port = ntohs(80);
        a_web.sin_addr.s_addr =
            ntohl(INADDR_ANY);

        /*
         * Bind the web server address-
         * we need to be root to succeed
         * at this:
         */
        z = bind(s,
            (struct sockaddr *)&a_web,
            sizeof a_web);
        if ( z == -1 )
            bail("binding port 80");    

        /*
         * Turn on SO_REUSEADDR :
         */
        z = setsockopt(s,SOL_SOCKET,
            SO_REUSEADDR,&b,sizeof b);
        if ( z == -1 )
            bail("setsockopt(2)");

    } else  {
        /*
         * Run in sockserv mode:  Request
         * a socket bound to port 80:
         */
        s = reqport(80);
        if ( s == -1 )
            bail("reqport(80)");
    }

    /*
     * Now make this a listening socket:
     */
    z = listen(s,10);
    if ( z == -1 )
        bail("listen(2)");

    /*
     * Peform a simple, web server loop for
     * demonstration purposes. Here we just
     * accept one line of input text, and
     * ignore it. We provide one simple
     * HTML page back in response:
     */
    for (;;) {
        /*
         * Wait for a connect from browser:
         */
        alen = sizeof a_cln;
        c = accept(s,
            (struct sockaddr *)&a_cln,
            &alen);
        if ( c == -1 ) {
            perror("accept(2)");
            continue;
        }

        /*
         * Create streams for convenience, and
         * just eat any web command provided:
         */
        rx = fdopen(c,"r");
        tx = fdopen(dup(c),"w");
        fgets(getbuf,sizeof getbuf,rx);

        /*
         * Now serve a simple HTML response.
         * This includes this web server's
         * process ID and the current date
         * and time:
         */
        fputs("<HTML>\n"
            "<HEAD>\n"
            "<TITLE>Test Page for this little "
                "web80 server</TITLE>\n"
            "</HEAD>\n"
            "<BODY>\n"
            "<H1>web80 Worked!</H1>\n",tx);

        time(&td);
        fprintf(tx,
            "<H2>From PID %ld @ %s</H2>\n",
            (long)getpid(),
            ctime(&td));

        fputs("</BODY>\n"
            "</HTML>\n",tx);

        fclose(tx);
        fclose(rx);
    }

    return 0;
}
