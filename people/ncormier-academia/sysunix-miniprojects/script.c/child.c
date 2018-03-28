/*
** child.c for  in /u/ept2/cormie_n/lang/c/script
**
** Made by nicolas cormier
** Login   <cormie_n@epita.fr>
**
** Started on  Fri Feb  4 12:16:15 2005 nicolas cormier
** Last update Sun Feb  6 14:28:42 2005 nicolas cormier
*/

#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <time.h>
#include "my_script.h"

extern int	gl_pty[2];
extern FILE	*gl_fscript;

static void	shell_child(int *pty, FILE *fscript)
{
  char	*shell;

  shell = getenv("SHELL");
  if (shell == NULL)
    shell = "/bin/sh";
  if (close(pty[MASTER]) == -1)
    m_end();
  gl_pty[MASTER] = -1;
  if (fclose(fscript) == -1)
    m_end();
  gl_fscript = NULL;
  if (m_login_tty(pty[SLAVE]) == -1)
    m_end();
  execl(shell, shell, "-i", NULL);
  m_end();
}

static void	print_date(FILE *fscript, char *status)
{
  char		buf_time[BUFSIZ];
  char		buf_print[BUFSIZ];
  time_t	tval;

  if (time(&tval) == -1)
    m_end();
#ifdef __sun
  strcpy(buf_time, ctime(&tval));
#else
 strftime(buf_time, sizeof(buf_time), "%a %b %e %H:%M:%S %Y"
	   , localtime(&tval));
#endif
  if (sprintf(buf_print, "Script %s on %s\n", status, buf_time) == -1)
    m_end();
  if (fwrite(buf_print, 1, strlen(buf_print), fscript) == -1)
    m_end();
}

static void	output_child(int *pty, FILE *fscript)
{
  int	nbread;
  char	buf[BUFSIZ];

  if (close(STDIN_FILENO) == -1)
    m_end();
  if (close(pty[SLAVE]) == -1)
    m_end();
  gl_pty[SLAVE] = -1;
  print_date(fscript, "started");
  while (1)
    {
      nbread = read(pty[MASTER], buf, sizeof(buf));
      if (nbread <= 0)
	break;
      if (write(1, buf, nbread) == -1)
	m_end();
      if (fwrite(buf, 1, nbread, fscript) == -1)
	m_end();
    }
  print_date(fscript, "done");
  close_openfile();
  exit(0);
}

void	fork_child(int *pty, FILE *fscript)
{
  int	child;

  if ((child = fork()) == -1)
    m_end();
  if (child == IS_CHILD)
    {
      if ((child = fork()) == -1)
	m_end();
      if (child == IS_CHILD)
	output_child(pty, fscript);
      else
	shell_child(pty, fscript);
    }
}
