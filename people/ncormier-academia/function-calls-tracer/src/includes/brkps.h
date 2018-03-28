/*
** brkps.h for  in /home/nico/lang/c/ftrace/src
**
** Made by nicolas
** Mail   <n.cormier@gmail.com>
**
** Started on  Sun Apr  2 17:25:06 2006 nicolas
** Last update Sun Apr  9 19:12:58 2006 nicolas
*/

#ifndef __BRKPS_H__
# define __BRKPS_H__

# include "api/includes/types/proc.h"
# include "api/includes/types/prim_types.h"

# include "tracee.h"

void	brkps_inject(tracee_t*);
/* void	fbrkps_reinject(tracee_t*); */
void	brkps_try_resolved(tracee_t*);

#endif /* __BRKPS_H__ */
