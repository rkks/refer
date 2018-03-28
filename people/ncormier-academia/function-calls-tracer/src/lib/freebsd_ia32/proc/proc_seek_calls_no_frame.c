/*
** proc_seek_calls_no_frame.c for  in /home/nico/ftrace/src
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
** 
** Started on  Mon Apr 24 19:57:22 2006 nicolas
** Last update Mon Apr 24 19:57:41 2006 nicolas
*/

static addr_t	plt_start = 0;
static addr_t	plt_end = 0;
static int	addr_is_in_plt(addr_t addr)
{  
  if (addr >= plt_start && addr <= plt_end)
    return (1);
  return (0);
}

/*
** proc_seek_calls statics.
*/
int		proc_trace_extcall = 1;
/*
** Classic Call.
*/
static int	next_is_call = 0;
static addr_t	last = 0;
static addr_t	last_ins = 0;
/*
** Plt.
*/
static addr_t	waiting_for = 0;
static addr_t	solve_plt_add = 0;
static int	waiting_for_rtld = 0;


/*
** Syscall case.
*/
static int	next_is_syscall = 0;
static list_t*	syscall_ret_list = NULL;

static void	is_syscall_check(addr_t cur, addr_t r)
{
  if (next_is_syscall)
    {
      next_is_syscall = 0;
      if ((0x0000FFFF & r) == 0x80CD)
	{
	  syscall_ret_list = list_add(syscall_ret_list, (void*) cur + 2);
	}
    }
}

static int	check_syscall_ret(addr_t cur)
{
  list_t*	list = syscall_ret_list;

  for (; list; list = list->next)
    {
      if ((void*) cur == list->value)
	{
	  syscall_ret_list = list_del(syscall_ret_list, list->value);
	  return (1);
	}

      if (list == syscall_ret_list->prev)
	break;
    }
  return (0);
}

static list_t*	force_ret_list = NULL;
static int	check_force_ret(addr_t cur, addr_t* ioaddr)
{
  list_t*	list = force_ret_list;

  for (; list; list = list->next)
    {
      if (cur == ((force_ret_t*) list->value)->ret)
	{
	  *ioaddr = ((force_ret_t*) list->value)->from;
	  force_ret_list = list_del(force_ret_list, list->value);
	  return (1);
	}

      if (list == force_ret_list->prev)
	break;
    }
  return (0);
}

static int	add_force_ret(addr_t cur, addr_t ret)
{
  force_ret_t*	fret;

  fret = malloc(sizeof(force_ret_t));
  if (fret == NULL)
    return (-1);
  fret->from = cur;
  fret->ret = ret;
  force_ret_list = list_add(force_ret_list, (void*) fret);
  return (0);
}

int	proc_seek_calls(proc_obj_t* obj, addr_t* ioaddr)
{
  regs_t*	regs = NULL;
  int		r, ret = 0;
  addr_t	cur;

  /*
  ** If *ioaddr == 0 no addr is specified, use current eip
  */
  if (!*ioaddr)
    {
      regs = proc_get_registers(obj);
      if (regs == NULL)
	return (-1);
      *ioaddr = regs->r_eip;
    }
  else
    /*
    ** han !
    */
    next_is_call = 1;

  cur = *ioaddr;

  /*
  ** When the user want to trace extern calls
  ** we skip the rtld work.
  */
  if (proc_trace_extcall && addr_is_in_rtld(obj, cur))
    {
      free(regs);
      return (0);
    }

  
  if (waiting_for)
    {
      if (!solve_plt_add && !addr_is_in_rtld(obj, cur) && !addr_is_in_plt(cur))
	{
	  solve_plt_add = cur;
	}
      goto proc_seek_calls_check_waiting_for;
    }


  /*
  ** Read PC instruction.
  */
  if (proc_read(obj, cur, sizeof(int), (char*) &r) == -1)
    goto proc_seek_calls_failed;

/*   printf("%x (%x)  ->  %x %d \n", cur, cur - 0x28074000, r, next_is_call); */


  /*
  ** Extern call.
  ** When user have activated extern call tracing.
  */
  {
    /*
    ** Syscalls handler.
    */
    is_syscall_check(cur, r);
    if (check_syscall_ret(cur))
      {
	/*
	** Check if the syscall suceed.
	** if CF = 1 syscall failed.
	*/
	if (regs->r_eflags & 1)
	  {
	    addr_t	lasteip;
	    /*
	    ** Syscall failed, force a return on last eip.
	    */
	    if (proc_read(obj, regs->r_esp, sizeof(int), (char*) &lasteip) == -1)
	      goto proc_seek_calls_failed;
	    if (add_force_ret(cur, lasteip) == -1)
	      return (-1);
	  }
      }
    else if (check_force_ret(cur, ioaddr))
      {
	/*
	** Check force returns due to a failed syscall.
	** ps: si ca c'est anglais ...
	*/
	ret = RET;
      }
  }


  /*
  ** TODO comment
  **
  ** 0f b7 c0     movzwl %ax,%eax
  ** 89 c2        mov    %eax,%edx
  ** 55           push   %ebp
  */
  if ((0x00FFFFFF & r) == 0xc0b70f && next_is_call == 1)
    {
      addr_t	rb;

      if (proc_read(obj, cur + 3, sizeof(int), (char*) &rb) == -1)
	goto proc_seek_calls_failed;
      if ((0x0000FFFF & rb) == 0xc289)
	{
	  if (proc_read(obj, cur + 5, sizeof(int), (char*) &rb) == -1)
	    goto proc_seek_calls_failed;
	  if ((0x000000FF & rb) == 0x55)
	    {
	      ret = CALL;
	    }
	}
    }
  /*
  ** Call is a local call.
  */
  else if ((0x00FFFFFF & r) == 0xe58955 && next_is_call == 1)
    {
      /*
      ** Wait one step and look if it is a syscall.
      */
      next_is_syscall = 1;
      ret = CALL;
    }
  /*
  ** Call is an extern call (PLT).
  */
  else if (next_is_call == 1 && (
				 (0x0000FFFF & r) == 0x25FF ||
				 (0x0000FFFF & r) == 0xA3FF
				 )
	   )
    {
      /*
      ** Where are in the plt
      **
      ** if the user don't want to trace extern call
      ** we skip every instruction until we find the
      ** extern function's return.
      **
      ** else we skip the rtld instruction and trace
      ** the extern call.
      */
    
      addr_t	plt_0;

      /*
      ** Eip is on the first insctruction of plt[function_called].
      **
      ** ff 25 14 24 05 08       jmp    *0x8052414
      ** 68 c0 00 00 00          push   $0xc0
      ** e9 60 fe ff ff          jmp    80488fc <_init+0x14>
      **
      ** To determine the addr of plt_0 we have to calc the near jump
      ** destination addr.
      */
      if (proc_read(obj, cur + 12, sizeof(addr_t), (char*) &plt_0) == -1)
	goto proc_seek_calls_failed;
      plt_0 += regs->r_eip + 16;
      plt_start = plt_0;
      plt_end = cur + 11;

      if (!proc_trace_extcall)
	waiting_for = last + 5;
      else
	waiting_for_rtld = 1;
    }

  /*
  ** Unset call seek flag.
  */
  next_is_call = 0;


  /*
  ** Pointer function.
  **
  ** 50                      push   %eax
  ** ff 15 30 e4 04 08       call   *0x804e430
  */
  if ((0x000000FF & last_ins) == 0x50 && (0x0000FFFF & r) == 0x15FF)
    {
      next_is_call = 1;
    }
  /*
  ** Call opcode.
  */
  else if ((0x000000FF & r) == 0xE8 || (0x000000FF & r) == 0x9A ||
	   (0x000000FF & r) == 0xE9)
    {
      /*
      ** Wait one step and look if it really is a call.
      */
      next_is_call = 1;
    }



  /*
  ** Return sequence.
  */
  if ((0x000000FF & r) == 0xC3 || (0x000000FF & r) == 0xC2 ||
      (0x000000FF & r) == 0xCB || (0x000000FF & r) == 0xCA)
    {
      ret = RET;
    }


  /*
  ** Force the current instruction to be a call.
  ** (After a plt call)
  **
  ** call extern_func
  ** plt			skip.
  ** (rtld or not)		skip.
  ** extern call	     <- force call here.
  */
  if (waiting_for_rtld)
    {
      if (!addr_is_in_plt(cur))
	{
	  waiting_for_rtld = 0;
	  ret = CALL;
	  /*
	  ** Wait one step and look if it is a syscall.
	  */
	  next_is_syscall = 1;
	}
    }
 

 proc_seek_calls_check_waiting_for:
  /*
  ** Skipping instuction until PC != waiting_for
  ** if PC == waiting_for where are on the extern
  ** call return.
  */
  if (cur == waiting_for)
    {
      waiting_for = 0;
      next_is_call = 0;
      *ioaddr = solve_plt_add;
      solve_plt_add = 0;
      ret = EXTCALL;
    }


  /*
  ** Save last PC.
  ** Save last insctruction.
  */
  last = cur;
  last_ins = r;

  if (regs)
    free(regs);
  return (ret);

 proc_seek_calls_failed:
  if (regs)
    free(regs);
  return (-1);
}
