/*
** netmalloc_common.c for  in /home/nico/lang/kernel/kld-netmalloc
** 
** Made by nicolas
*/

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/proc.h>
#include <sys/malloc.h>
#include <sys/module.h>
#include <sys/queue.h>
#include <sys/proc.h>
#include <vm/vm.h>
#include <vm/vm_page.h>
#include <vm/vm_map.h>
#include <vm/vm_object.h>

#include "netmalloc_common.h"
#include "server.h"
#include "die.h"
#include "kernel-dependant.h"
#include "vm.h"

/* Private types definition
 */
struct lst_allocation
{
  vm_offset_t			paddr;
  unsigned			id;
  unsigned			size;
  LIST_ENTRY(lst_allocation)	entries;
};

/* Private vars
 */
static LIST_HEAD(, lst_allocation)	elems;
static int				init = 0;

/* Forward declarations
 */
static void			list_initialization(void);
static void			insert_allocation(vm_offset_t paddr, unsigned id, unsigned size);
static struct lst_allocation*	get_allocation_by_vaddr(vm_offset_t paddr);

/* Public functions
 */
int	netmalloc_action(struct thread* th, struct netmalloc_args* args, void** kaddr)
{
  switch (args->action)
    {
    case DO_NETMALLOC:
      {
	int dummy;
	void* waddr = &dummy;
	unsigned o_id = 0;
	void* o_kvaddr = NULL;
	struct lst_excmd* cmd = malloc(sizeof(struct lst_excmd));
	if (!cmd) return -1;
	cmd->command = NETMALLOC;
	cmd->data.netmalloc_args.size = args->size;
	cmd->data.netmalloc_args.host = "127.0.0.1";
	cmd->data.netmalloc_args.port = 4243;
	cmd->data.netmalloc_args.kvaddr = &o_kvaddr;
	cmd->data.netmalloc_args.id = &o_id;
	cmd->data.netmalloc_args.waddr = waddr;
	LIST_INSERT_HEAD(&get_server()->extern_commands, cmd, excmd_list);
	(void) tsleep(waddr, PCATCH|PWAIT, "idle", 0);
	*args->o_vaddr = o_kvaddr;
	*args->o_id = o_id;
	printf("alloc: %x @ %d\n", (unsigned)*args->o_vaddr, *args->o_id);
	insert_allocation(vtophys(o_kvaddr), *args->o_id, args->size);
	*kaddr = o_kvaddr;
      }
      break;
    case DO_NETATTACH:
      {
	int dummy;
	void* waddr = &dummy;
	void* o_kvaddr = NULL;
	struct lst_excmd* cmd = malloc(sizeof(struct lst_excmd));
	if (!cmd) return -1;
	cmd->command = NETATTACH;
	cmd->data.netattach_args.id = args->id;
	cmd->data.netattach_args.size = 1024;
	cmd->data.netattach_args.host = "127.0.0.1";
	cmd->data.netattach_args.port = 4243;
	cmd->data.netattach_args.waddr = waddr;
	cmd->data.netattach_args.kvaddr = &o_kvaddr;
	LIST_INSERT_HEAD(&get_server()->extern_commands, cmd, excmd_list);
	(void) tsleep(waddr, PCATCH|PWAIT, "idle", 0);
	*args->o_vaddr = o_kvaddr;
	printf("attach: %x @ %d\n", (unsigned)*args->o_vaddr, args->id);
	insert_allocation(vtophys(o_kvaddr), args->id, 1024);
	*kaddr = o_kvaddr;
      }
      break;
    case DO_NETDETACH:
      {
	vm_offset_t paddr = vtophys(args->vaddr);
	struct lst_allocation* alloc = get_allocation_by_vaddr(paddr);
	if (!alloc) die("netdetach: can't find allocation");
	struct lst_excmd* cmd = malloc(sizeof(struct lst_excmd));
	cmd->command = NETDETACH;
	cmd->data.netdetach_args.id = alloc->id;
	LIST_INSERT_HEAD(&get_server()->extern_commands, cmd, excmd_list);
      }
      break;
    case DO_NETWRITE:
      {
	(void) copyin(args->iobuf, args->vaddr, args->iosize);
      }
      break;
    case DO_NETREAD:
      {
	(void) copyout(args->vaddr, args->iobuf, args->iosize);
      }
      break;
    }
  return 0;
}

/* List handler
 */
static void	list_initialization(void)
{
  if (init) return;
  init = 1;
  LIST_INIT(&elems);
}

static void	insert_allocation(vm_offset_t paddr, unsigned id, unsigned size)
{
  if (!init) list_initialization();
  struct lst_allocation* new_alloc = malloc(sizeof(struct lst_allocation));
  if (!new_alloc) die("insert_allocation: allocation failed");
  new_alloc->paddr = paddr;
  new_alloc->id = id;
  new_alloc->size = size;
  LIST_INSERT_HEAD(&elems, new_alloc, entries);
}

static struct lst_allocation*	get_allocation_by_vaddr(vm_offset_t paddr)
{
  if (!init) list_initialization();
  struct lst_allocation* lst_elem = NULL;
  LIST_FOREACH(lst_elem, &elems, entries)
    {
      if (lst_elem->paddr == paddr)
	return lst_elem;
    }
  return NULL;
}
