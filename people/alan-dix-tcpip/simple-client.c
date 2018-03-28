#include <stdio.h>
#include <string.h>
#include "sock.h"


#define	PORTNO	6789
#define HOST    "gamma"

#define BUFF_LEN 200
char buff[BUFF_LEN];
int buf_len = BUFF_LEN-1;     /*  allow room for terminating '\0'  */

main(argc,argv)
    int argc;
    char **argv;
{
    int  server_fd;
  /*  request connection to server  */
    server_fd = inet_connect(HOST,PORTNO);
	/*  waits for server to accept   	*/
	/*  returns NULL on failure  	      */
	/*  host is server¹s machine  	    */
    if ( server_fd < 0 ) { perror("socket"); exit(1); }
    printf("You can send now\n");
 
/*  talk to server  */
    for(;;) {
      /*  our turn first  */
        printf("speak: ");
        if ( gets(buff) == NULL ) {    /* user typed end of file  */
            close(server_fd);
            printf("bye bye\n");
            break;
            }
        write(server_fd,buff,strlen(buff);

      /*  wait for server¹s message  */
        len = read(server_fd,buff,buf_len); 
        if (len == 0) {
            printf("client finished the conversation\n");
            break;
            }
        buff[len] = '\0';
        printf("server says: %s\n",buff);
    }
    exit(0);
}

