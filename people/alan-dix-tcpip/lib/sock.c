/****************************************/
/*                                      */
/*      sock.c                          */
/*                                      */
/*      Alan Dix                        */
/*                                      */
/*      package up socket garbage       */
/*                                      */
/*      5th January 1990                */
/*                                      */
/****************************************/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include "sock.h"

int  parse_network_args( argc, argv, pipename, hostname, portno, errmess )
    int *argc;
    char **argv;
    char **pipename;
    char **hostname;
    int *portno;
    char **errmess;

{
    char **from = &argv[1],
         **to   = &argv[1];
    if (pipename) *pipename = NULL;
    if (hostname) *hostname = NULL;
    if (portno) *portno = -1;
    while ( *from ) {
        if ( strcmp(*from,"-unix") == 0 ) {
	    from ++;
	    if ( pipename==NULL ) {
		*errmess = "UNIX sockets not allowed";
		return 0;
		}
	    if ( !*from ) {
		*errmess = "-unix requires pipe name";
		return 0;
		}
	    *pipename = *from;
            from ++;
	    *argc -= 2;;
            }
        else  if ( strcmp(*from,"-host") == 0 ) {
	    from ++;
	    if ( hostname==NULL ) {
		*errmess = "INTERNET sockets not allowed";
		return 0;
		}
	    if ( !*from ) {
		*errmess = "-host requires host name";
		return 0;
		}
	    *hostname = *from;
            from ++;
	    *argc -= 2;;
            }
        else  if ( strcmp(*from,"-port") == 0 ) {
	    from ++;
	    if ( portno==NULL ) {
		*errmess = "cannot set INTERNET port number";
		return 0;
		}
	    if ( !*from || !isdigit(**from) ) {
		*errmess = "-port requires port number";
		return 0;
		}
	    *portno = atoi(*from);
            from ++;
	    *argc -= 2;;
            }
        else {
	    if ( to != from ) *to = *from;
	    }
        }
    return 1;
}


int  unix_socket(name)
    char * name;
{
    int     sd, code;
    struct  sockaddr bind_addr;
    bind_addr.sa_family = AF_UNIX;
    strncpy(bind_addr.sa_data,name,14);
    sd = socket(AF_UNIX, SOCK_STREAM,0);
    if ( sd < 0 ) return sd;
    code = bind(sd, &bind_addr, sizeof(u_short) + strlen(name) );
    if ( code < 0 ) { close(sd); return code; }
    code = listen(sd, 1);
    if ( code < 0 ) { close(sd); return code; }
    return sd;
}

int  unix_connect(name)
    char * name;
{
    int     sd, code;
    struct  sockaddr bind_addr;
    bind_addr.sa_family = AF_UNIX;
    strncpy(bind_addr.sa_data,name,14);
    sd = socket(AF_UNIX, SOCK_STREAM,0);
    if ( sd < 0 ) return sd;
    code = connect(sd, &bind_addr, sizeof(u_short) + strlen(name) );
    if ( code < 0 ) { close(sd); return code; }
    return sd;
}

static void check_inet_host(hostname)
    char * hostname;
{
    char real_hostname[32];
    gethostname(real_hostname,32);
    if ( strncmp(hostname,real_hostname) == 0 ) return;
    mess( "program running on %s, must run on host %s\n",
                          real_hostname,         hostname  );
    exit(1);
}

int  inet_socket(hostname,portno)
    char * hostname;
    int    portno;
{
    int     sd, code;
    struct  sockaddr_in bind_addr;
    check_inet_host(hostname);
                     /* do this to by-pass bug in INET sockets*/
    bind_addr.sin_family = AF_INET;
    bind_addr.sin_addr.s_addr = 0;
    bzero(bind_addr.sin_zero, 8);
    bind_addr.sin_port = portno;
    sd = socket(AF_INET, SOCK_STREAM,0);
    if ( sd < 0 ) return sd;
    code = bind(sd, &bind_addr, sizeof(bind_addr) );
    if ( code < 0 ) { close(sd); return code; }
    code = listen(sd, 1);
    if ( code < 0 ) { close(sd); return code; }
    return sd;
}

int  inet_connect(hostname,portno)
    char * hostname;
    int    portno;
{
    int     sd, code;
    struct  sockaddr_in bind_addr;
    struct hostent *host;

    host = gethostbyname(hostname);
    if (host == NULL ) return -1;
    bind_addr.sin_family = PF_INET;
    bind_addr.sin_addr = *((struct in_addr *) (host->h_addr));
    bind_addr.sin_port = portno;
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if ( sd < 0 ) return sd;
    code = connect(sd, &bind_addr, sizeof(bind_addr) );
    if ( code < 0 ) { close(sd); return code; }
    return sd;
}

int  sock_accept(sock)
    int sock;
{
    int     sd;
    struct  sockaddr bind_addr;
    int len=sizeof(bind_addr);
    sd = accept(sock, &bind_addr, &len);
    return sd;
}

