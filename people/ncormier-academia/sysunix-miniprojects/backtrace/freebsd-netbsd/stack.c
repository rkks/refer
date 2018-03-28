/*
** stack.c for  in /home/nico/lang/c/backtrace
**
** Made by nicolas
** Login   <nico@epita.fr>
**
** Started on  Mon Jan 16 14:55:35 2006 nicolas
** Last update Tue Jan 24 19:53:10 2006 nicolas cormier
*/

#include <sys/types.h>
#include <errno.h>
#include <string.h>

#include "backtrace.h"
#include "elffprot.h"

/*
**
** Backtrace recursiv function.
**
*/
void	recur_stack_call_trace(struct reg* cur_reg, int count)
{
  int		ret;
  caddr_t	cur = (caddr_t) cur_reg->r_ebp;
  caddr_t	eip, ebp;

  eip = (caddr_t) cur_reg->r_eip;
  ebp = (caddr_t) cur_reg->r_ebp;

  if (readmem_frm_exec((long) cur, sizeof(int), (char*) &ret) == -1)
    errexit("backtrace: error during the reading of the stack, %s", strerror(errno));

  cur_reg->r_ebp = ret;
  if ((caddr_t) ret <= ebp)
    return;

  if (readmem_frm_exec((long)(cur + sizeof(int)), sizeof(int), (char*) &ret) == -1)
    errexit("backtrace: error during the reading of the stack, %s", strerror(errno));

  if (ret == 0)
    return;

  print_addr_info((caddr_t) ret, count);
  recur_stack_call_trace(cur_reg, count + 1);
}
