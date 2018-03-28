#include <stdio.h>
#include <string.h>
#include "sock.h"

#define	PORTNO	6789
#define HOST    "gamma"        /*  needs to be set to server's machine  */

#define BUFF_LEN 200
char buff[BUFF_LEN];
int buf_len = BUFF_LEN-1;     /*  allow room for terminating '\0'  */

main(argc,argv)
    int argc;
    char **argv;
{
    int    port_fd,  client_fd;
    int    len;

 /*  establish port  */
    port_fd = inet_socket(HOST,PORTNO);
    if ( port_fd < 0 ) { perror("socket"); exit(1); }
    printf("start up complete\n");
 
  /*  wait for client to connect  */
    client_fd = sock_accept(port_fd);

	/*  only want one client, so close port_fd  */
    close(port_fd);

  /*  talk to client  */
    for(;;) {
      /*  wait for client¹s message  */
        len = read(client_fd,buff,buf_len); 
        if (len == 0) {
            printf("client finished the conversation\n");
            break;
            }
        buff[len] = '\0';
        printf("client says: %s\n",buff);
      /*  now it¹s our turn  */
        printf("speak: ");
        if ( gets(buff) == NULL ) {    /* user typed end of file  */
            close(client_fd);
            printf("bye bye\n",id);
            break;
            }
        write(client_fd,buff,strlen(buff);
    }
    exit(0);
}
