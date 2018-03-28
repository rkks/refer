/*
** proc.h for  in /home/nico/ftrace/src
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
** 
** Started on  Sun Apr 23 00:18:19 2006 nicolas
** Last update Sun Apr 23 00:19:35 2006 nicolas
*/

#ifndef __FREEBSD_PROC_H__
# define __FREEBSD_PROC_H__

struct force_ret_s
{
  addr_t	from;
  addr_t	ret;
};
typedef struct force_ret_s  force_ret_t;

#endif /* __FREEBSD_PROC_H__ */
