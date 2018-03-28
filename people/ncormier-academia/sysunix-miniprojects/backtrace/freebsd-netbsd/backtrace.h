/*
** backtrace.h for  in /home/nico/lang/c/backtrace
**
** Made by nicolas
** Login   <nico@epita.fr>
**
** Started on  Wed Jan  4 14:39:38 2006 nicolas
** Last update Mon Jan 23 15:24:19 2006 nicolas cormier
*/

#ifndef __BACKTRACE_H__
#define __BACKTRACE_H__

#include <machine/reg.h>

#ifndef MIN
#define MIN(a,b)                (((a) < (b)) ? (a) : (b))
#endif

/*
** File: backtrace.c
*/
char*	get_bin_path(int act);
void	backtrace(void);
void	print_addr_info(caddr_t addr, int count);

/*
** File: stack.c
*/
void		recur_stack_call_trace(struct reg* cur_reg, int count);

/*
** File: error.c
*/
void		errexit(char *format, ...);

/*
** File: ptrace.c
*/
int		copy_from_son(long addr, int len, char* laddr);
struct reg	ptrace_get_registers(void);
void		restore_data(int idx);
void		restore_alldata(void);
void		restore_bkrpoint(int idx);
void		inject_brkpoints(void);
void		solve_human_brkpoints(void);
int		chk_brkpoints(void);
void		free_brkcont(void);

/*
** File: brkpoints.c
*/
unsigned int	axtoi(char *hex_str);
void		add_rbrkpoints(caddr_t addr);
void		add_brkpoints(char* brkp);
void		del_brkpoints(int idx);
char**		get_humanbrkps(void);
caddr_t*	get_bkrpoints(void);
void		print_brks(void);
void		free_brkpoints(void);

/*
** File: itrace.c
*/
void	tracepid(char* pid);
void	traceson(char* ubin, char **argv);

#define USAGE "backtrace [-h] [-v] {[-p pid] | [-e cmd] | [-c core]} [cmd]"

#endif /* __BACKTRACE_H__ */
