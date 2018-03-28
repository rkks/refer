#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include "../netmalloc_common.h"

int main()
{
  int fd = open("/dev/netmalloc", O_RDWR);
  unsigned id = 0;
  void* vaddr = 0;
#if 1
  /* Netmalloc() */
  {
    struct netmalloc_args args;
    args.action = 0; /* NETMALLOC */
    args.size = 1024;
    args.o_vaddr = &vaddr;
    args.o_id = &id;
    syscall(210, &args);
    void* toto = mmap(0, getpagesize(), PROT_READ|PROT_WRITE, 0, fd, 0);
    printf("+++> %x\n", toto);
    sleep(5);
    memcpy(toto, "TOTO", sizeof("TOTO"));
  }
  return 0;
  /* Netdetach */
  {
    struct netmalloc_args args;
    args.action = 3; /* NETDETACH */
    args.vaddr = vaddr;
    syscall(210, &args);
    mmap(0, getpagesize(), PROT_READ|PROT_WRITE, 0, fd, 0);
  }
#else
  /* Netattach() */
 {
    struct netmalloc_args args;
    args.action = 2; /* NETATTACH */
    args.o_vaddr = &vaddr;
    args.id = 0;
    syscall(210, &args);
 }
  /* Netdetach */
  {
    struct netmalloc_args args;
    args.action = 3; /* NETDETACH */
    args.vaddr = vaddr;
    syscall(210, &args);
  }
#endif
  close(fd);
}
