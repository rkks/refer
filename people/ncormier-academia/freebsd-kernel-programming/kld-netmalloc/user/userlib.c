/*
** userlib.c for  in /home/nico/lang/kernel/kld-netmalloc/user
** 
** Made by nicolas
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/queue.h>
#include "userlib.h"
#include "server.h"
#include "die.h"

/* List handler
 */
struct lst_allocation
{
  void*				vaddr;
  unsigned			id;
  unsigned			size;
  LIST_ENTRY(lst_allocation)	entries;
};

static LIST_HEAD(, lst_allocation)	elems;
static int init = 0;

static void	list_initialization(void)
{
  if (init)
    return;
  init = 1;
  LIST_INIT(&elems);
}

static void	insert_allocation(void* vaddr, unsigned id, unsigned size)
{
  if (!init) list_initialization();
  struct lst_allocation* new_alloc = malloc(sizeof(struct lst_allocation));
  if (!new_alloc) die("insert_allocation: allocation failed");
  new_alloc->vaddr = vaddr;
  new_alloc->id = id;
  new_alloc->size = size;
  LIST_INSERT_HEAD(&elems, new_alloc, entries);
}

static struct lst_allocation*	get_allocation_by_vaddr(void* vaddr)
{
  if (!init) list_initialization();
  struct lst_allocation* lst_elem = NULL;
  LIST_FOREACH(lst_elem, &elems, entries)
    {
      if (lst_elem->vaddr == vaddr)
	return lst_elem;
    }
  return NULL;
}

/* Netmalloc functions
 */
void*	netmalloc(unsigned size)
{
  unsigned id = 0;
  int wsync = 0;
  void* ret = NULL;
  /* Netmalloc command*/
  {
    struct lst_excmd* cmd = malloc(sizeof(struct lst_excmd));
    cmd->command = NETMALLOC;
    cmd->data.netmalloc_args.size = size;
    cmd->data.netmalloc_args.host = "127.0.0.1";
    cmd->data.netmalloc_args.port = 4243;
    cmd->data.netmalloc_args.kvaddr = &ret;
    cmd->data.netmalloc_args.id = &id;
    cmd->data.netmalloc_args.waddr = (void*)&wsync;
    LIST_INSERT_HEAD(&get_server()->extern_commands, cmd, excmd_list);
  }
  while (!wsync)
    usleep(500);
  printf("alloc: %x @ %d\n", ret, id);
  insert_allocation(ret, id, size);
  return ret;
}

void	netdetach(void* data)
{
  struct lst_allocation* alloc = get_allocation_by_vaddr(data);
  if (!alloc) die("netdetach: can't find allocation");
  /* Detach allocation */
  {
    struct lst_excmd* cmd = malloc(sizeof(struct lst_excmd));
    cmd->command = NETDETACH;
    cmd->data.netdetach_args.id = alloc->id;
    LIST_INSERT_HEAD(&get_server()->extern_commands, cmd, excmd_list);
  }
}

void*	netattach(unsigned id)
{
  int wsync = 0;
  void* ret = NULL;
  /* Netattach 1024 */
  {
    struct lst_excmd* cmd = malloc(sizeof(struct lst_excmd));
    cmd->command = NETATTACH;
    cmd->data.netattach_args.id = id;
    cmd->data.netattach_args.size = 1024;
    cmd->data.netattach_args.host = "127.0.0.1";
    cmd->data.netattach_args.port = 4243;
    cmd->data.netattach_args.waddr = (void*)&wsync;
    cmd->data.netattach_args.kvaddr = &ret;
    LIST_INSERT_HEAD(&get_server()->extern_commands, cmd, excmd_list);
  }
  while (!wsync)
    usleep(500);
  printf("attach: %x @ %d\n", ret, id);
  insert_allocation(ret, id, 1024);
  return ret;
}

void	netfree(void* data)
{
  /* Netfree */
  {
    struct lst_excmd* cmd = malloc(sizeof(struct lst_excmd));
    cmd->command = NETFREE;
  }
}
