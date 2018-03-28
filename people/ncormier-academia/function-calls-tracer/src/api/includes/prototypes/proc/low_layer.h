/*
** low_layer.h for  in /home/nico/lang/c/ftrace/src/api/includes/prototypes/proc
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
** 
** Started on  Wed Mar 22 17:41:22 2006 nicolas
** Last update Sun Apr  9 19:11:57 2006 nicolas
*/

#ifndef __PROC_LOW_LAYER_H__
# define __PROC_LOW_LAYER_H__

# include "api/includes/types/proc.h"
# include "api/includes/types/prim_types.h"
# include "api/includes/types/regs.h"


proc_obj_t*	proc_new(void);
int		proc_del(proc_obj_t*);

regs_t*		proc_get_registers(proc_obj_t*);
int		proc_set_registers(proc_obj_t*, regs_t*);
int		proc_read(proc_obj_t*, addr_t, unsigned int, char*);
int		proc_write(proc_obj_t*, addr_t, unsigned int, char*);
int		proc_continue(proc_obj_t*, int);
int		proc_continue_addr(proc_obj_t*, addr_t, int);
int		proc_step(proc_obj_t*, int);
int		proc_step_addr(proc_obj_t*, addr_t, int);
int		proc_create(proc_obj_t*, char*, char**);
int		proc_attach(proc_obj_t*, pid_t);
int		proc_detach(proc_obj_t*);
int	        proc_kill(proc_obj_t*);
int		proc_stop(proc_obj_t*);

/* /\* */
/* ** Define Type. (3' arg) */
/* *\/ */
/* #define DB_EXEC		1	/\* break on execute *\/ */
/* #define DB_WRONLY	2	/\* break on write *\/ */
/* #define DB_RDWR		3	/\* break on read or write *\/ */
/* int		proc_set_hardware_breakpoint(proc_obj_t*, brkp_t*, int); */


/*
** proc_wait_debug_event{_nohang} return
*/
# define DEBUG_EVENT		0
# define SIGNAL_EVENT		1
# define EXIT_EVENT		2
# define EXIT_SIGNALED_EVENT	3
int		proc_wait_debug_event(proc_obj_t* obj, int* sigret);
# define NOTHING_EVENT		4
int		proc_wait_debug_event_nohang(proc_obj_t* obj, int* sigret);

#endif /* __PROC_LOW_LAYER_H__ */
