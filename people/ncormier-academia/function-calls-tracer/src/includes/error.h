/*
** error.h for  in /home/nico/lang/c/ftrace/src/lib/freebsd_ia32/proc
** 
** Made by nicolas
** Mail   <n.cormier@gmail.com>
** 
** Started on  Sat Mar 25 21:36:43 2006 nicolas
** Last update Wed Apr  5 23:07:27 2006 nicolas
*/

#ifndef __ERROR_H__
# define __ERROR_H__

/*
** error.c
*/
void	errexit(char *format, ...);
char**	__ftrace_errstr(void);
char*	ftrace_strerror(int);

# define ftrace_errstr	(* __ftrace_errstr())
# define EFTRACE	4242


#endif /* __ERROR_H__ */
