/*
** util.c for  in /u/ept2/cormie_n/lang/c/script
**
** Made by nicolas cormier
** Login   <cormie_n@epita.fr>
**
** Started on  Fri Feb  4 12:08:25 2005 nicolas cormier
** Last update Sun Feb  6 14:43:16 2005 nicolas cormier
*/

#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include "my_script.h"

static void	try_group_pty(int *amaster, int *aslave, char *path)
{
  char	*pty_mask;

  for (pty_mask = "0123456789abcdefghijklmnopqrstuv"; *pty_mask; pty_mask++)
    {
      path[5] = 'p';
      path[9] = *pty_mask;
      if ((*amaster = open(path, O_RDWR, 0)) == -1)
	{
	  if (errno == ENOENT)
	    break;
	}
      else
	{
	  path[5] = 't';
	  if ((*aslave = open(path, O_RDWR, 0)) != -1)
	    break;
	  if (close(*amaster) == -1)
	    m_end();
	}
    }
}

static void	get_pty(int *amaster, int *aslave)
{
  char	*path;
  char	*pty_grp_mask;

  path = strdup("/dev/pty??");
  *aslave = -1;
  for (pty_grp_mask = "pqrsPQRS"; *pty_grp_mask; pty_grp_mask++)
    {
      path[8] = *pty_grp_mask;
      try_group_pty(amaster, aslave, path);
      if (*aslave != -1)
	break;
    }
  if (*aslave == -1)
    m_end();
}

void	m_openpty(int *amaster, int *aslave, struct termios *termp, struct winsize *winp)
{
  get_pty(amaster, aslave);
  if (tcsetattr(*aslave, (int) TCSAFLUSH, termp) == -1)
    m_end();
  if (ioctl(*aslave, (int) TIOCSWINSZ, (char *) winp) == -1)
    m_end();
}

int	m_login_tty(int fd)
{
  if (setsid() == -1)
    m_end();
#ifdef TIOCSCTTY
  if (ioctl(fd, (int) TIOCSCTTY, (char *) NULL) == -1)
    m_end();
#endif
  if (dup2(fd, 0) == -1)
    m_end();
  if (dup2(fd, 1) == -1)
    m_end();
  if (dup2(fd, 2) == -1)
    m_end();
  if (close(fd) == -1)
    m_end();
  return (0);
}
