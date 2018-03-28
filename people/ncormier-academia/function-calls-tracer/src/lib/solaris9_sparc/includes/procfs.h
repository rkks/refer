/*
** procfs.h for  in /u/ept3/cormie_n/lang/c/ftrace/src/lib/solaris9_sparc
**
** Made by nicolas cormier
** Login   <cormie_n@epita.fr>
**
** Started on  Wed Apr  5 18:52:21 2006 nicolas cormier
** Last update Tue Apr 25 11:42:34 2006 nicolas cormier
*/

#ifndef __PROCFS_H__
#define __PROCFS_H__

# include <procfs.h>

# define MAX_PROC_NAME_SIZE sizeof("/proc/99999/lwp/8096/lstatus")

# define    IS_SYS_EXEC(w)  (w == SYS_exec || w == SYS_execve)

struct procfs_fd_s
{
  int	ctl;
  int	status;
  int	was;
  int	ras;
  int	gwin;
};
typedef struct procfs_fd_s  procfs_fd_t;

struct	writeregs_procfs_s
{
  unsigned long	func;
  prgregset_t	regs;
};
typedef struct writeregs_procfs_s  writeregs_procfs_t;


struct	pcsfault_s
{
  long		ctl;
  fltset_t	s_fltset;
};
typedef struct pcsfault_s  pcsfault_t;

struct	pcsentry_s
{
  long		ctl;
  sysset_t	sysset;
};
typedef struct pcsentry_s	pcsentry_t;
typedef struct pcsentry_s	pcsexit_t;


#endif /* __PROCFS_H__ */
