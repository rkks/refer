/**
 * sparsecat -- utility to write space-saving sparse files, reading from
 * stdin and writing to stdout.  If the output is not a file, it doesn't 
 * try to write sparsely.
 *
 * Written by Tyler Montbriand.  Freeware.
 */

#ifndef _LARGEFILE64_SOURCE
#warning "May need -D_LARGEFILE64_SOURCE to compile"
#endif

#if _XOPEN_SOURCE < 600
#warning "Compile with -D_XOPEN_SOURCE=600 for smarter cache when supported"
#endif

#include <errno.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const char *zero_mem=MAP_FAILED;

int byte_count=0;
volatile int kb_count=0;
int sparseable=1;
int last_sparse=0;
long int sttime;

int cat_file(const int fd_in, const int fd_out)
{
  ssize_t bytes_read;
  char buf[512];

#if _XOPEN_SOURCE >= 600
  posix_fadvise(fd_in, 0, 0, POSIX_FADV_SEQUENTIAL | POSIX_FADV_NOREUSE);
  posix_fadvise(fd_out, 0, 0, POSIX_FADV_SEQUENTIAL | POSIX_FADV_NOREUSE);
#endif

  while((bytes_read=read(fd_in, buf, 512)) > 0)
  {
    const char *pos=buf;
    const char *end=(buf+bytes_read);

    if(sparseable)
    if(memcmp(zero_mem, buf, bytes_read)==0)
    {
      if(lseek64(fd_out, bytes_read, SEEK_CUR) < 0)
      {
        perror("Couldn't seek");
        sparseable=0;
      }
      else
      {
        last_sparse=1;
        continue;
      }

      byte_count+=bytes_read;
      if(byte_count >= 1024)
      {
        kb_count += byte_count / 1024;
        byte_count %= 1024;
      }
    }

    last_sparse=0;

    while(pos < end)
    {
      ssize_t bytes_wrote=write(fd_out, pos, end-pos);
      if(bytes_wrote <=0)
        return(-1);

      pos+=bytes_wrote;
      byte_count+=bytes_wrote;

      if(byte_count >= 1024)
      {
        kb_count += byte_count / 1024;
        byte_count %= 1024;
      }
    }
  }

//  fprintf(stderr, "read returned %d\n", (int)bytes_read);
  if((int)bytes_read < 0)
  {
    perror("Couldn't read");
    return(-1);
  }

  return(0);
}

int main(int argc, char *argv[])
{
  int retval=0;

  sttime=time(NULL);

  {
    int zero_fd=open("/dev/zero", O_RDONLY);
    if(zero_fd<0)
      return(255);
    zero_mem=(const char *)mmap(NULL, getpagesize(), PROT_READ, MAP_SHARED,
      zero_fd, (off_t)0L);
    close(zero_fd);
    if(zero_mem==MAP_FAILED)
    {
      perror("Couldn't map /dev/zero");
      return(255);
    }
  }

//  signal(SIGALRM, alarm_handler);
//  alarm(ALARM_INTERVAL);

  if(argc > 1)
  {
    int n;
    for(n=1; n<argc; n++)
    {
      int fd_in=open(argv[n],O_RDONLY);
      if(fd_in < 0)
      {
        fprintf(stderr,"Couldn't open %s: %s\n", argv[n], strerror(errno));
        continue;
      }

      if(cat_file(fd_in, STDOUT_FILENO)<0)
      {
        fprintf(stderr,"Error cattting file: %s\n",strerror(errno));
        retval=1;
      }

      close(fd_in);
    }  
  }
  else
  {
    if(cat_file(STDIN_FILENO, STDOUT_FILENO)<0)
      retval=1;
  }

  if(last_sparse)
  {
    char c=0;
    lseek64(STDOUT_FILENO, -1, SEEK_CUR);
    write(STDOUT_FILENO, &c, 1);    
  }

  munmap((void *)zero_mem,getpagesize());

  fprintf(stderr, "Finished sparsecat\n");
  return(retval);
}
