#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h> 

/* for further information on serial programming with termios 
 * see http://en.wikibooks.org/wiki/Serial_Programming/termios
   This code is based on an example on that site. */
 
void usage(char *prog) {
  fprintf(stderr, "usage: %s <dev>\n",prog); 
  fprintf(stderr, "device is /dev/ttyS0 or similar\n"); 
  fprintf(stderr, "tip: GNU screen can read serial devices too, screen <dev>\n"); 
  exit(-1);
}

int main(int argc,char** argv)
{
  char *dev = (argc > 1) ? argv[1] : NULL;
  if (!dev) usage(argv[0]);

  struct termios tio;
  memset(&tio,0,sizeof(tio));
  tio.c_iflag=0;
  tio.c_oflag=0;
  tio.c_cflag=CS8|CREAD|CLOCAL; // 8n1, see termios.h 
  tio.c_lflag=0;
  tio.c_cc[VMIN]=1;
  tio.c_cc[VTIME]=5;
 
  int tty_fd;
  tty_fd=open(argv[1], O_RDWR);      
  cfsetospeed(&tio,B9600);  // 9600 baud
  cfsetispeed(&tio,B9600);  // 9600 baud
  tcsetattr(tty_fd,TCSANOW,&tio);

  char c;
  while (read(tty_fd,&c,1)>0) write(STDOUT_FILENO,&c,1); 
 
  close(tty_fd);
}
