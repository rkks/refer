/*
** tracee.h for  in /home/nico/lang/c/ftrace/src/includes
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
** 
** Started on  Thu Mar 23 22:46:06 2006 nicolas
** Last update Sun Apr 23 21:41:01 2006 nicolas
*/

#ifndef __TRACEE_H__
# define __TRACEE_H__

# include "api/includes/types/proc.h"

# include "bin.h"

/*
** Tracee status.
*/
# define RUN	0
# define STEP	1
struct tracee_s
{
  proc_obj_t*		proc;
  bin_handler_t*		binh;
  list_t*		brkps_toset;
  brkps_t*		brkps;
  brkps_t*		fbrkps;
  int			status;
};
typedef struct tracee_s  tracee_t;


#endif /* __TRACEE_H__ */
