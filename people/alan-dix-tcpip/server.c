#include <stdio.h>
#include <string.h>
#include "mess.h"
#include "sock.h"
#include "inform.h"
#include "monitor.h"
#include "line_by_line.h"

#define MAXCLIENTS      10

int   clients = 0;
int   ns[MAXCLIENTS];

int  read_client(nd,id)
    int  nd;
    int  id;
{
    int len, i;
    char buf[256];
    len = read(nd, buf, sizeof(buf)-1);
    if (len == 0) {
        mess("client %d leaving the conversation\n",id);
        inform_input(nd,NULL,NULL);
        ns[id] = 0;
	}
    else {
        char mess_buff[256];			
        buf[len] = '\0';
        mess("client %d says :'%s'\n", id, buf);
        sprintf(mess_buff,"client %d says :'%s'", id, buf);
        for ( i=0; i<clients; i++ ) {
            if ( ns[i] > 0 ) {
                write(ns[i],mess_buff,strlen(mess_buff));
                } 
	    }
	}
    return 0;
}

int  accept_client(sd,id)
    int   sd;
    void *id;
{
    ns[clients] = sock_accept(sd);
    if ( ns[clients] < 0 ) {
        perror("accept");
        return -1;
        }
    inform_input(ns[clients],read_client,clients);
    mess("new client %d\n",clients);
    ++clients;
    if ( clients >= MAXCLIENTS ) {
        mess("server saturated\n");
        inform_input(sd,NULL,NULL);
        }
    return 0;
}

/* the following table is used by the library call 'process_line'  */
/* to despatch user commands, the table has the form:		   */
/*     { id, command_name, command_function, help_string }	   */
/* process_line is called with a line the user has typed and then  */
/* uses the first word in that line to look up in the table,	   */
/* and call the appropriate function additional functions can be   */
/* added to this table as an aid to debugging			   */

struct mon_tab_struct monitor_tab[] = {
    { 0, "exit", inform_done, "close down server"            },
    { 0, 0, 0, 0 }
};

term_line(fd,id,buff)
    int   fd;
    void *id;
    char *buff;
{
    mess("command {%s}\n",buff);
    perform_line(buff);
    return 0;
}

term_eof(fd,id)
    int   fd;
    void *id;
{
    inform_done();
    mess("lost control terminal\n");
    return 0;
}

#define	PORTNO	6789
#define HOST    "gamma"

int port = PORTNO;
char *host = HOST;

main(argc,argv)
    int argc;
    char **argv;
{
    int        sd;
    char *errmess;

    if ( ! parse_network_args(&argc,argv,NULL,&host,&port,&errmess) ) {
	fprintf(stderr,"%s: %s\n",argv[0],errmess);
	exit(1);
	}
    if ( host == NULL ) host = HOST;
    if ( port < 0 ) port = PORTNO;
    sd = inet_socket(host,port);
    if ( sd < 0 ) { perror("socket"); exit(1); }
    inform_line_by_line(0,term_line,term_eof,NULL);
    inform_input(sd,accept_client,NULL);
    mess("start up complete\n");
    inform_loop();
    exit(0);
}
