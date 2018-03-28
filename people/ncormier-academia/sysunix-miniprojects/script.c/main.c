/*
** my_script.c for  in /u/ept2/cormie_n/lang/c/script
**
** Made by nicolas cormier
** Login   <cormie_n@epita.fr>
**
** Started on  Wed Feb  2 15:35:47 2005 nicolas cormier
** Last update Sun Feb  6 14:40:19 2005 nicolas cormier
*/

#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <sys/signal.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include "my_script.h"

#ifdef __alpha
 pid_t wait3(int *status_location, int options, struct rusage *resource_usage );
#endif

struct termios		gl_term_back;
int			gl_pty[2];
pid_t			gl_father;
FILE			*gl_fscript;
char			*gl_fname;

void	readin_writepty(int master)
{
  int	nbread;
  char	buf[BUFSIZ];

  while ((nbread = read(STDIN_FILENO, buf, BUFSIZ)) > 0)
    if (write(master, buf, nbread) == -1)
      m_end();
}

FILE	*scriptfile(int argc, char **argv)
{
  FILE	*retour;
  char	*fname;
  int	flag_opt;
  int	opt_retour;

  flag_opt = 0;
  while ((opt_retour = getopt(argc, argv, "a")) != -1)
    if (opt_retour == 'a')
      flag_opt = 1;
  argc -= flag_opt;
  argv += flag_opt;
  if (argc > 1)
    fname = argv[1];
  else
    fname = DEF_FILE;
  if ((retour = fopen(fname, flag_opt ? "a" : "w")) == NULL)
    m_end();
  gl_fscript = retour;
  gl_fname = strdup(fname);
  printf("Script started, output file is %s\n", fname);
  return (retour);
}

void	init_gl(void)
{
  gl_fscript = NULL;
  gl_pty[MASTER] = -1;
  gl_pty[SLAVE] = -1;
}

void	child_death(int jesersarien)
{
  int	pid;
  int	status;
  int	error;

  error = 0;
  while ((pid = wait3(&status, WNOHANG, 0)) > 0)
    if (WIFEXITED(status) != 1)
      error = 1;
  if (error != 0)
    m_end();
  restore_term();
  close_openfile();
  printf("Script done, output file is %s\n", gl_fname);
  exit(0);
}

int			main(int argc, char **argv)
{
  FILE			*fscript;
  int			pty[2];
  struct termios	term;
  struct winsize	win;

  gl_father = getpid();
  init_gl();
  fscript = scriptfile(argc, argv);
  init_vterm(pty, &term, &win);
  signal(SIGCHLD, child_death);
  fork_child(pty, fscript);
  if (fclose(fscript) != 0)
    m_end();
  if (close(pty[SLAVE]) == -1)
    m_end();
  gl_pty[SLAVE] = -1;
  gl_fscript = NULL;
  inhib_rterm(&term);
  readin_writepty(pty[MASTER]);
  return (0);
}
