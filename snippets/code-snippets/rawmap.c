/**
 * Example of raw memory access in Linux.
 * Maps the low BIOS area into memory then writes to file.
 * 
 * Written by Tyler Montbriand.  Freeware.
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/mman.h>

#include <stdio.h>
#include <stdlib.h>

// File descriptor for /dev/mem
static int memfd=-1;
static int pagesize=-1;

unsigned long align_addresses(unsigned long *start, unsigned long *len)
{
        unsigned long offset=(*start)%pagesize;
        unsigned long extra;

        fprintf(stderr, "0x%08x[0x%08x] =>", *start, *len);
        (*len)+=offset;
        (*start)-=offset;

        extra=(*len)%pagesize;
        if(extra)
                (*len)+=pagesize-extra;


        fprintf(stderr, "0x%08x[0x%08x]\n", *start, *len);
        return(offset);
}

volatile unsigned char *rawmap(unsigned long start, unsigned long len)
{
        volatile unsigned char *mem;
        unsigned int offset=align_addresses(&start, &len);

        mem=mmap(NULL, len, PROT_READ|PROT_WRITE, MAP_SHARED, memfd, start);
        if(mem == MAP_FAILED)
                return(NULL);

        fprintf(stderr, "Mapped %p\n", mem);

        return(mem + offset);
}

int rawunmap(volatile unsigned char *ptr, unsigned long len)
{
        unsigned long start=(unsigned long)ptr;
        unsigned long offset=align_addresses(&start, &len);

        return(munmap(ptr-offset, len));
}

// PC BIOS start area
#define ROM_START       0x000f0000
// PC BIOS last valid address
#define ROM_END         0x000fffff
// Length of memory segment
#define ROM_LEN         ((ROM_END - ROM_START)+1)

int main(void)
{
  volatile unsigned char *mem=NULL;

  pagesize=getpagesize();

  memfd=open("/dev/mem", O_SYNC|O_RDWR); // O_SYNC necessary for raw access
  if(memfd<0)
    goto MAIN_ERR;

  mem=rawmap(ROM_START, ROM_LEN);
  if(mem == NULL)
  {
    fprintf(stderr, "Couldn't map 0x%08x-0x%08x\n", ROM_START, ROM_END);
    goto MAIN_ERR;
  }

  fprintf(stderr, "All mapped.\n");

  // Print the raw PC BIOS to standard output
  write(1, mem, ROM_LEN);

  rawunmap(mem, ROM_LEN);

  close(memfd);

  return(0);

MAIN_ERR:
  if(mem != NULL)
    rawunmap(mem, ROM_LEN);

  if(memfd >= 0)
    close(memfd);

  return(1);
}

