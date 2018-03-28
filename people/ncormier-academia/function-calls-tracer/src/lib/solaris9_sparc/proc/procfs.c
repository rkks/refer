/*
** procfs.c for  in /u/ept3/cormie_n/lang/c/ftrace/src
**
** Made by nicolas cormier
** Login   <cormie_n@epita.fr>
**
** Started on  Wed Apr 19 11:00:08 2006 nicolas cormier
** Last update Tue Apr 25 23:25:24 2006 nicolas cormier
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "procfs.h"

#ifndef __DONT_USE_READ_PROCFS__
static int	read_procfs_open(proc_obj_t* obj, char* name, int* fd)
{
  char		bufb[MAX_PROC_NAME_SIZE];
  int		nfd;

  if (sprintf(bufb, "/proc/%d/%s", (int) obj->pid, name) == -1)
    return (-1);
  if (access(bufb, R_OK))
    return (-1);
  nfd = open(bufb, O_RDONLY);
  if (nfd == -1)
    return (-1);
  *fd = nfd;
  return (0);
}

static int	read_procfs(proc_obj_t* obj, char* name, size_t len, char* buf, off_t offset)
{
  int	fd = -1, r = -1;

  if (!strcmp("status", name))
    {
      if (((procfs_fd_t*) obj->luse)->status == -1)
	{
	  if (read_procfs_open(obj, name, &((procfs_fd_t*) obj->luse)->status) == -1)
	    return (-1);
	}
      fd = ((procfs_fd_t*) obj->luse)->status;
    }
  else if (!strcmp("as", name))
    {
      if (((procfs_fd_t*) obj->luse)->ras == -1)
	{
	  if (read_procfs_open(obj, name, &((procfs_fd_t*) obj->luse)->ras) == -1)
	    return (-1);
	}
      fd = ((procfs_fd_t*) obj->luse)->ras;
    }
  else if (!strcmp("lwp/1/gwindows", name))
    {
      if (((procfs_fd_t*) obj->luse)->gwin == -1)
	{
	  if (read_procfs_open(obj, name, &((procfs_fd_t*) obj->luse)->gwin) == -1)
	    return (-1);
	}
      fd = ((procfs_fd_t*) obj->luse)->gwin;
    }
  r = pread(fd, buf, len, offset);
  return (r);
}
#endif /* __DONT_USE_READ_PROCFS__ */

static int	write_procfs_open(proc_obj_t* obj, char* name, int* fd)
{
  char		bufb[MAX_PROC_NAME_SIZE];
  int		nfd;

  if (sprintf(bufb, "/proc/%d/%s", (int) obj->pid, name) == -1)
    return (-1);
  if (access(bufb, W_OK))
    return (-1);
  nfd = open(bufb, O_WRONLY);
  if (nfd == -1)
    return (-1);
  *fd = nfd;
  return (0);
}

static int	write_procfs(proc_obj_t* obj, char* name, size_t len, char* buf, off_t offset)
{
  int	fd = -1, w = -1;

  if (!strcmp("ctl", name))
  {
    if (((procfs_fd_t*) obj->luse)->ctl == -1)
      {
	if (write_procfs_open(obj, name, &((procfs_fd_t*) obj->luse)->ctl) == -1)
	  return (-1);
      }
      fd = ((procfs_fd_t*) obj->luse)->ctl;
  }
  else if (!strcmp("as", name))
  {
    if (((procfs_fd_t*) obj->luse)->was == -1)
      {
	if (write_procfs_open(obj, name, &((procfs_fd_t*) obj->luse)->was) == -1)
	  return (-1);
      }
      fd = ((procfs_fd_t*) obj->luse)->was;
  }

  w = pwrite(fd, buf, len, offset);
  return (w);
}
