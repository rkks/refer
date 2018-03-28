/*
** test.c for  in /home/nico/lang/kernel/kld-netmalloc
** 
** Made by nicolas
*/

#include "../netmalloc_common.h"

int main()
{
  unsigned id = 0;
  void* vaddr = 0;
#if 0
  /* Netmalloc() */
  {
    struct netmalloc_args args;
    args.action = DO_NETMALLOC;
    args.size = 1024;
    args.o_vaddr = &vaddr;
    args.o_id = &id;
    syscall(210, &args);
  }
  /* Netwrite() */
  {
    struct netmalloc_args args;
    args.vaddr = vaddr;
    args.action = DO_NETWRITE;
    args.iobuf = "TATA";
    args.iosize = sizeof("TATA");
    syscall(210, &args);
  }
  /* Netdetach */
  {
    struct netmalloc_args args;
    args.action = DO_NETDETACH;
    args.vaddr = vaddr;
    syscall(210, &args);
  }
#elif 0
  /* Netattach() */
 {
    struct netmalloc_args args;
    args.action = DO_NETATTACH;
    args.o_vaddr = &vaddr;
    args.id = 0;
    syscall(210, &args);
 }
  /* Netwrite() */
  {
    struct netmalloc_args args;
    args.action = DO_NETWRITE;
    args.vaddr = (char*)vaddr + 4;
    args.iobuf = "TITI";
    args.iosize = sizeof("TITI");
    syscall(210, &args);
  }
  /* Netdetach */
  {
    struct netmalloc_args args;
    args.action =  DO_NETDETACH;
    args.vaddr = vaddr;
    syscall(210, &args);
  }
#else
  /* Netattach() */
 {
    struct netmalloc_args args;
    args.action = DO_NETATTACH;
    args.o_vaddr = &vaddr;
    args.id = 0;
    syscall(210, &args);
 }
  /* Netwrite() */
  {
    char buf[256];
    struct netmalloc_args args;
    args.action = DO_NETREAD;
    args.vaddr = (char*)vaddr;
    args.iobuf = buf;
    args.iosize = sizeof(buf);
    syscall(210, &args);
    printf("User buf => %s\n", buf);
  }
  /* Netdetach */
  {
    struct netmalloc_args args;
    args.action =  DO_NETDETACH;
    args.vaddr = vaddr;
    syscall(210, &args);
  }
#endif
}
