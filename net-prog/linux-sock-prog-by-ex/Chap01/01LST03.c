/* Listing 1.3
 *
 * Client/Server Example Using socketpair(2)
 * and fork(2) :
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>

/*
 * As of RedHat-6.0, these are still not defined :
 */
#ifndef SHUT_WR
#define SHUT_RD     0
#define SHUT_WR     1
#define SHUT_RDWR   2
#endif

/*
 * Main program :
 */
int
main(int argc,char **argv) {
    int z;          /* Status return code */
    int s[2];       /* Pair of sockets */
    char *msgp;     /* A message pointer */
    int mlen;       /* Message length */
    char buf[80];   /* Work buffer */
    pid_t chpid;    /* Child PID */

    /*
     * Create a pair of local sockets :
     */
    z = socketpair(AF_LOCAL,SOCK_STREAM,0,s);

    if ( z == -1 ) {
        fprintf(stderr,"%s: socketpair(2)\n",
            strerror(errno));
        exit(1);
    }

    /*
     * Now fork() into two processes :
     */
    if ( (chpid = fork()) == (pid_t)-1 ) {

        /*
         * Failed to fork into two processes :
         */
        fprintf(stderr,"%s: fork(2)\n",
            strerror(errno));
        exit(1);

    } else if ( chpid == 0 ) {

        /*
         * This is the child process (client) :
         */
        char rxbuf[80]; /* Receive buffer */

        printf("Parent PID is %ld\n",(long)getppid());

        close(s[0]);    /* Server uses s[1] */
        s[0] = -1;      /* Forget this unit */

        /*
         * Form the message and its length :
         */
        msgp = "%A %d-%b-%Y %l:%M %p";
        mlen = strlen(msgp);

        printf("Child sending request '%s'\n",msgp);
        fflush(stdout);

        /*
         * Write a request to the server :
         */
        z = write(s[1],msgp,mlen);

        if ( z < 0 ) {
            fprintf(stderr,"%s: write(2)\n",
                strerror(errno));
            exit(1);
        }

        /*
         * Now indicate that we will not be writing
         * anything further to our socket, by shutting
         * down the write side of the socket :
         */
        if ( shutdown(s[1],SHUT_WR) == -1 ) {
            fprintf(stderr,"%s: shutdown(2)\n",
                strerror(errno));
            exit(1);
        }

        /*
         * Receive the reply from the server :
         */
        z = read(s[1],rxbuf,sizeof rxbuf);
        if ( z < 0 ) {
            fprintf(stderr,"%s: read(2)\n",
                strerror(errno));
            exit(1);
        }

        /*
         * Put a null byte at the end of what we
         * received from the server:
         */
        rxbuf[z] = 0;

        /*
         * Report the results :
         */
        printf("Server returned '%s'\n",rxbuf);
        fflush(stdout);

        close(s[1]);    /* Close our end now */

    } else {

        /*
         * This is the parent process (server) :
         */
        int status; /* Child termination status */
        char txbuf[80];         /* Reply buffer */
        time_t td;       /* Current date & time */

        printf("Child PID is %ld\n",(long)chpid);
        fflush(stdout);

        close(s[1]);    /* Client uses s[1] */
        s[1] = -1;      /* Forget this descriptor */

        /*
         * Wait for a request from the client :
         */
        z = read(s[0],buf,sizeof buf);

        if ( z < 0 ) {
            fprintf(stderr,"%s: read(2)\n",
                strerror(errno));
            exit(1);
        }

        /*
         * Put a null byte at the end of the
         * message we received from the client:
         */
        buf[z] = 0;

        /*
         * Now perform the server function on the
         * received message :
         */
        time(&td);          /* Get current time */

        strftime(txbuf,sizeof txbuf,  /* Buffer */
            buf,                /* Input format */
            localtime(&td));      /* Input time */

        /*
         * Send back the response to client :
         */
        z = write(s[0],txbuf,strlen(txbuf));

        if ( z < 0 ) {
            fprintf(stderr,"%s: write(2)\n",
                strerror(errno));
            exit(1);
        }

        /*
         * Close our end of the socket :
         */
        close(s[0]);

        /*
         * Wait for the child process to exit..
         * See text.
         */
        waitpid(chpid,&status,0);
    }

    return 0;
}
