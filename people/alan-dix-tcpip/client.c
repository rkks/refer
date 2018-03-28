#include <stdio.h>
#include <string.h>
#include "mess.h"
#include "sock.h"
#include "inform.h"
#include "line_by_line.h"

char in_buff[256], out_buff[256], line_buff[256];

int  line_len = 0;

int sd;  /* socket file descriptor */

read_socket(fd,id)
    int   fd;
    void *id;
{
    int n = read(fd,in_buff,255);
    if ( n==0 ) {
        inform_done();
        fprintf(stderr,"socket closed\n");
        return 0;
        }
    in_buff[n] = '\0';
    printf("received %s\n",in_buff);
    fflush(stdout);
    return 0;
}

term_line(fd,id,buff)
    int   fd;
    void *id;
    char *buff;
{
    mess("sending {%s}\n",buff);
    write(sd,buff,strlen(buff));
    return 0;
}

term_eof(fd,id)
    int   fd;
    void *id;
{
    inform_done();
    mess("client done\n");
    return 0;
}

#define	PORTNO	6789
#define HOST    "gamma"

int port = PORTNO;
char *host = HOST;
char *unix_sock = NULL;

main(argc,argv)
    int argc;
    char **argv;
{
    char *errmess;

#ifdef UNIX_SOCKET
    if ( ! parse_network_args(&argc,&argv,&unix_sock,NULL,NULL,&errmess) ) {
	fprintf(stderr,"%s: %s\n",argv[0],errmess);
	exit(1);
	}
    if ( unix_sock == NULL ) {
        fprintf(stderr,"usage: %s -unix socket-name\n",argv[0]);
        exit(1);
        }
    sd = unix_connect(unix_sock);
#else
    if ( ! parse_network_args(&argc,argv,NULL,&host,&port,&errmess) ) {
	fprintf(stderr,"%s: %s\n",argv[0],errmess);
	exit(1);
	}
    if ( host == NULL ) host = HOST;
    if ( port < 0 ) port = PORTNO;
    sd = inet_connect(host,port);
#endif
    if ( sd < 0 ) { perror("socket"); exit(1); }
    printf("You can send now\n");
    inform_line_by_line(0,term_line,term_eof,NULL);
    inform_input(sd,read_socket,NULL);
    inform_loop();
    exit(0);
}

