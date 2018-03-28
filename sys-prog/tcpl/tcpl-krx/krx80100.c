/*
  Andrew Tesker
  ucat.c
  a version of cat using UNIX system access
*/

#include <stdio.h>
#include <fcntl.h>
#define BUFSIZE 1024


int main(int argc, char *argv[])
{
  int fd1;
  void filecopy(int f, int t);
  
  if(argc == 1)
    filecopy(0, 1);

  else {
    while(--argc > 0)
      if(( fd1 = open(*++argv, O_RDONLY, 0)) == -1) {
	printf("unix cat: can't open %s\n", *argv);
	return 1;
      }
      else {
	filecopy(fd1, 1);
	close(fd1);
      }
  }
  
  return 0;
  
}

void filecopy(int from, int to)
{
  int n;
  char buf[BUFSIZE];

  while((n=read(from, buf, BUFSIZE)) > 0 )
    write(to, buf, n);
}
