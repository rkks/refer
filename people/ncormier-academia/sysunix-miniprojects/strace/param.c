/*
** param.c for  in /home/nico/lang/c/strace
**
** Made by nicolas
** Login   <nico@epita.fr>
**
** Started on  Wed Dec 28 18:24:32 2005 nicolas
** Last update Tue Jan  3 14:29:38 2006 nicolas cormier
*/

#include <string.h>
#include <stdio.h>
#include <errno.h>
#include "strace.h"

extern int	son;

#ifndef MIN
#define MIN(a,b)                (((a) < (b)) ? (a) : (b))
#endif

void	cpyfrmson(long addr, int len, char* laddr)
{
  int	m;
  long	val;

  if (addr & (sizeof(long) - 1))
    errexit("strace: align");
  while (len)
    {
      errno = 0;
      val = ptrace(PT_READ_D, son, (char *) addr, 0);
      if (errno)
	errexit("strace: ptrace(PT_READ_D), %s", strerror(errno));
      m = MIN((int) sizeof(long), len);
      memcpy(laddr, (const void*) &val, m);
      addr += sizeof(long);
      laddr += m;
      len -= m;
    }
}

void	print_value_frmtab(int param, flags_str_t* tab)
{
  int	i, pipe = 0;

  for (i = 0; tab[i].name; i++)
    {
      if (param== tab[i].val)
	{
	  if (pipe == 1)
	    printf("|");
	  printf("%s", tab[i].name);
	  pipe = 1;
	}
    }
  if (!pipe)
    printf("%d", param);
}

void	print_flags_frmtab(int param, flags_str_t* tab)
{
  int		i, pipe = 0;

  for (i = 0; tab[i].name; i++)
    {
      if ((param & tab[i].val) == tab[i].val)
	{
	  if (pipe == 1)
	    printf("|");
	  printf("%s", tab[i].name);
	  pipe = 1;
	}
    }
  if (!pipe)
    printf("%d", param);
}

#include <vis.h>

void		print_str(int param)
{
  int		ret, i;
  char*		str;
  char		buf[256], bufb[512];
  caddr_t	addr;

  addr = (caddr_t) param;
  for (i = 0; 1; i++)
    {
      errno = 0;
      ret = ptrace(PT_READ_D, son, addr, 0);
      if (errno)
	{
/* 	  if (i && (errno == EPERM || errno == EIO)) */
	    return;
/* 	  errexit("strace: ptrace(PT_READ_D), %s", strerror(errno)); */
	}
      str = (char*) &ret;
      buf[i] = *str;
      if (!*str)
	break;
      addr++;
    }
  strvis(bufb, buf, VIS_CSTYLE);
  printf("\"%s\"", bufb);
}

void	print_void(int param)
{
  param = 0;
  printf("void");
}

#include "ioctl.c"

void	print_ioctl(int param)
{
  printf("%s", ioctlname(param));
}

flags_str_t	lseek_tab[] =
  {
    {SEEK_SET, "SEEK_SET"},
    {SEEK_CUR, "SEEK_CUR"},
    {SEEK_END, "SEEK_END"},
    {-1, NULL}
  };

void   	print_lseek(int param)
{
  print_value_frmtab(param, lseek_tab);
}

#include <signal.h>

flags_str_t	signal_tab[] =
  {
    {SIGHUP, "SIGHUP"},
    {SIGINT, "SIGINT"},
    {SIGQUIT, "SIGQUIT"},
    {SIGILL, "SIGILL"},
    {SIGTRAP, "SIGTRAP"},
    {SIGABRT, "SIGABRT"},
    {SIGEMT, "SIGEMT"},
    {SIGFPE, "SIGFPE"},
    {SIGKILL, "SIGKILL"},
    {SIGBUS, "SIGBUS"},
    {SIGSEGV, "SIGSEGV"},
    {SIGSYS, "SIGSYS"},
    {SIGPIPE, "SIGPIPE"},
    {SIGALRM, "SIGALRM"},
    {SIGTERM, "SIGTERM"},
    {SIGURG , "SIGURG "},
    {SIGSTOP, "SIGSTOP"},
    {SIGTSTP, "SIGTSTP"},
    {SIGCONT, "SIGCONT"},
    {SIGCHLD, "SIGCHLD"},
    {SIGTTIN, "SIGTTIN"},
    {SIGTTOU, "SIGTTOU"},
    {SIGIO, "SIGIO"},
    {SIGXCPU, "SIGXCPU"},
    {SIGXFSZ, "SIGXFSZ"},
    {SIGVTALRM, "SIGVTALRM"},
    {SIGPROF, "SIGPROF"},
    {SIGWINCH, "SIGWINCH"},
    {SIGINFO, "SIGINFO"},
    {SIGUSR1, "SIGUSR1"},
    {SIGUSR2, "SIGUSR2"},
    {-1, NULL}
  };

void	print_signal(int param)
{
  print_value_frmtab(param, signal_tab);
}

#include <sys/mman.h>

flags_str_t	mmapprot_tab[] =
  {
    {PROT_NONE, "PROT_NONE"},
    {PROT_READ, "PROT_READ"},
    {PROT_WRITE, "PROT_WRITE"},
    {PROT_EXEC, "PROT_EXEC"},
    {-1, NULL}
  };

void	print_mmapprot(int param)
{
  print_value_frmtab(param, mmapprot_tab);
}

void	print_filedesc(int param)
{
  if (param == 0)
    printf("STDIN_FILENO");
  else if (param == 1)
    printf("STDOUT_FILENO");
  else if (param == 2)
    printf("STDERR_FILENO");
  else
    printf("%d", param);
}

#include <sys/mman.h>

#ifdef __FreeBSD__
flags_str_t	mmapflags_tab[] =
  {
    {MAP_ANON, "MAP_ANON"},
    {MAP_FIXED, "MAP_FIXED"},
    {MAP_HASSEMAPHORE, "MAP_HASSEMAPHORE"},
    {MAP_NOCORE, "MAP_NOCORE"},
    {MAP_NOSYNC, "MAP_NOSYNC"},
    {MAP_PRIVATE, "MAP_PRIVATE"},
    {MAP_SHARED, "MAP_SHARED"},
    {MAP_STACK, "MAP_STACK"},
    {-1, NULL}
  };
#elif defined(__NetBSD__)
flags_str_t	mmapflags_tab[] =
  {

/*     {MAP_ALIGNED, "MAP_ALIGNED"}, */
    {MAP_ANON, "MAP_ANON"},
    {MAP_FILE, "MAP_FILE"},
    {MAP_FIXED, "MAP_FIXED"},
    {MAP_HASSEMAPHORE, "MAP_HASSEMAPHORE"},
    {MAP_INHERIT, "MAP_INHERIT"},
    {MAP_TRYFIXED, "MAP_TRYFIXED"},
    {MAP_WIRED, "MAP_WIRED"},
    {MAP_PRIVATE, "MAP_PRIVATE"},
    {MAP_SHARED, "MAP_SHARED"},
/*     {MAP_COPY, "MAP_COPY"}, */
    {-1, NULL}
  };
#else
flags_str_t	mmapflags_tab[] =
  {
    {-1, NULL}
  };
#endif

void		print_mmapflags(int param)
{
  print_flags_frmtab(param, mmapflags_tab);
}

#include <unistd.h>

flags_str_t	accessmode_tab[] =
  {
    {R_OK, "R_OK"},
    {W_OK, "W_OK"},
    {X_OK, "X_OK"},
    {F_OK, "F_OK"},
    {-1, NULL}
  };

void		print_accessmode(int param)
{
  print_flags_frmtab(param, accessmode_tab);
}

#include <fcntl.h>

#ifdef __FreeBSD__
flags_str_t	fcntl_tab[] =
  {
    {F_DUPFD, "F_DUPFD"},
    {F_GETFD, "F_GETFD"},
    {F_SETFD, "F_SETFD"},
    {F_GETFL, "F_GETFL"},
    {F_SETFL, "F_SETFL"},
    {F_GETOWN, "F_GETOWN"},
    {F_SETOWN, "F_SETOWN"},
    {O_NONBLOCK, "O_NONBLOCK"},
    {O_APPEND, "O_APPEND"},
    {O_DIRECT, "O_DIRECT"},
    {O_ASYNC, "O_ASYNC"},
    {F_GETLK, "F_GETLK"},
    {F_SETLK, "F_SETLK"},
    {F_SETLKW, "F_SETLKW"},
    {-1, NULL}
  };
#elif defined(__NetBSD__)
flags_str_t	fcntl_tab[] =
  {
    {F_DUPFD, "F_DUPFD"},
    {F_GETFD, "F_GETFD"},
    {F_SETFD, "F_SETFD"},
    {F_GETFL, "F_GETFL"},
    {F_SETFL, "F_SETFL"},
    {F_GETOWN, "F_GETOWN"},
    {F_SETOWN, "F_SETOWN"},
    {F_CLOSEM, "F_CLOSEM"},
    {F_MAXFD, "F_MAXFD"},
    {O_NONBLOCK, "O_NONBLOCK"},
    {O_APPEND, "O_APPEND"},
    {O_ASYNC, "O_ASYNC"},
    {-1, NULL}
  };
#else
flags_str_t	fcntl_tab[] =
  {
    {F_DUPFD, "F_DUPFD"},
    {F_GETFD, "F_GETFD"},
    {F_SETFD, "F_SETFD"},
    {F_GETFL, "F_GETFL"},
    {F_SETFL, "F_SETFL"},
    {F_GETOWN, "F_GETOWN"},
    {F_SETOWN, "F_SETOWN"},
    {-1, NULL}
  };
#endif

void   	print_fcntl(int param)
{
  print_flags_frmtab(param, fcntl_tab);
}

flags_str_t	openmode_tab[] =
  {
    {O_RDONLY, "O_RDONLY"},
    {O_WRONLY, "O_WRONLY"},
    {O_RDWR, "O_RDWR"},
    {O_NONBLOCK, "O_NONBLOCK"},
    {O_APPEND, "O_APPEND"},
    {O_CREAT, "O_CREAT"},
    {O_TRUNC, "O_TRUNC"},
    {O_EXCL, "O_EXCL"},
    {O_SHLOCK, "O_SHLOCK"},
    {O_EXLOCK, "O_EXLOCK"},
#ifdef __FreeBSD__
    {O_DIRECT, "O_DIRECT"},
#endif
    {O_FSYNC, "O_FSYNC"},
    {O_NOFOLLOW, "O_NOFOLLOW"},
    {-1, NULL}
  };

void		print_openmode(int param)
{
  print_flags_frmtab(param, openmode_tab);
}

#include <sys/stat.h>

flags_str_t	st_mode_tab[] =
  {
    {S_IFMT, "S_IFMT"},
    {S_IFIFO, "S_IFIFO"},
    {S_IFCHR, "S_IFCHR"},
    {S_IFDIR, "S_IFDIR"},
    {S_IFBLK, "S_IFBLK"},
    {S_IFREG, "S_IFREG"},
    {S_IFLNK, "S_IFLNK"},
    {S_IFSOCK, "S_IFSOCK"},
    {S_IFWHT, "S_IFWHT"},
    {S_ISUID, "S_ISUID"},
    {S_ISGID, "S_ISGID"},
    {S_ISVTX, "S_ISVTX"},
    {-1, NULL}
  };

flags_int_t	st_chmod_tab[] =
  {
    {S_IRUSR, 400},
    {S_IWUSR, 200},
    {S_IXUSR, 100},
    {S_IRGRP, 40},
    {S_IWGRP, 20},
    {S_IXGRP, 10},
    {S_IROTH, 4},
    {S_IWOTH, 2},
    {S_IXOTH, 1},
    {-1, -1}
  };

void	print_stat(int param)
{
  struct stat	st;
  caddr_t	addr;
  int		i, chmod = 0;

  addr = (caddr_t) param;
  cpyfrmson((long)addr, sizeof(struct stat), (char*) &st);
  printf("{ st_uid=%d, st_gid=%d, st_mode=", st.st_uid, st.st_gid);
  for (i = 0; st_mode_tab[i].name; i++)
    {
      if ((int) (st.st_mode & st_mode_tab[i].val) == st_mode_tab[i].val)
	printf("%s|", st_mode_tab[i].name);
    }

  for (i = 0; st_chmod_tab[i].num != -1; i++)
    {
      if ((int) (st.st_mode & st_chmod_tab[i].val) == st_chmod_tab[i].val)
	chmod += st_chmod_tab[i].num;
    }
  printf("0%d, ... }", chmod);
}

#include <sys/time.h>

void	print_timeval(int param)
{
  struct timeval	tp;
  caddr_t		addr;

  addr = (caddr_t) param;
  cpyfrmson((long)addr, sizeof(struct timeval), (char*) &tp);
  printf("{ tv_sec=%ld, tv_usec=%ld }", (long) tp.tv_sec, (long)tp.tv_usec);
}


#include <sys/statvfs.h>

void	print_statvfs(int param)
{
  struct statvfs	st;
  caddr_t		addr;

  addr = (caddr_t) param;
  cpyfrmson((long)addr, sizeof(struct statvfs), (char*) &st);
  printf("{ f_bavail=%ld, f_files=%ld, ... }", (long) st.f_bavail, (long)st.f_files);
}

void		print_addr(int param)
{
  printf("0x%x", param);
}

pprint_func_t	pprint_func_tab[] =
  {
    {"void", print_void},
    {"char *", print_str},
    {"struct timeval *", print_timeval},
    {"struct stat *", print_stat},
    {"struct nstat *", print_stat},
    {"struct stat43 *", print_stat},
    {"struct stat30 *", print_stat},
    {"struct stat12 *", print_stat},
    {"struct statvfs *", print_statvfs},
    {"strace_openmode", print_openmode},
    {"strace_accessmode", print_accessmode},
    {"strace_ioctl", print_ioctl},
    {"strace_fcntl", print_fcntl},
    {"strace_lseek", print_lseek},
    {"strace_filedesc", print_filedesc},
    {"strace_signal", print_signal},
    {"strace_mmapprot", print_mmapprot},
    {"strace_mmapflags", print_mmapflags},
    {"caddr_t", print_addr},
    {NULL, NULL}
  };

extern int	verbose;

void	print_param(int param, char* param_type)
{
  int	i;

  if (verbose)
    {
      if (!strncmp("const ", param_type, 6))
	param_type +=6;
      for (i = 0; pprint_func_tab[i].type; i++)
	if (!strcmp(param_type, pprint_func_tab[i].type))
	  {
	    pprint_func_tab[i].func(param);
	    return;
	  }
    }
  for(; *param_type; param_type++)
    if (*param_type == '*')
      {
	if (param == 0)
	  printf("NULL");
	else
	  print_addr(param);
	return;
      }
  printf("%d", param);
}
