/*
** bin.h for  in /home/nico/lang/c/ftrace/src
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
** 
** Started on  Wed Mar 29 19:24:14 2006 nicolas
** Last update Mon Apr 10 11:53:41 2006 nicolas
*/

#ifndef __BIN_H__
# define __BIN_H__

# include "api/includes/types/bin.h"
# include "api/includes/types/prim_types.h"
# include "api/includes/types/proc.h"
# include "api/includes/types/depend.h"

/*
** Types
*/

struct bin_s
{
  bin_obj_t*	obj;
  syms_t*	syms;
  debugs_t*	debugs;
};
typedef struct bin_s  bin_t;

/*
** Contains each binary load and tracee's binary load.
*/
struct bin_handler_s
{
  bin_t*	tracee;
  list_t*	depends;
};
typedef struct bin_handler_s  bin_handler_t;


/*
** Prototypes
*/

bin_handler_t*	bin_create_handler(char*);
bin_t*		bin_find(bin_handler_t*, addr_t);
int		bin_refresh_depends_list(depends_t*, bin_handler_t*);
int		bin_find_addr_frm_label(bin_handler_t* binh, char* label, addr_t* retaddr);
#endif /* __BIN_H__ */
