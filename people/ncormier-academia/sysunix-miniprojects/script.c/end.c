/*
** end.c for  in /u/ept2/cormie_n/lang/c/script
**
** Made by nicolas cormier
** Login   <cormie_n@epita.fr>
**
** Started on  Fri Feb  4 12:14:16 2005 nicolas cormier
** Last update Sun Feb  6 14:35:07 2005 nicolas cormier
*/


#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <termios.h>
#include "my_script.h"

extern struct termios	gl_term_back;
extern int		gl_pty[2];
extern pid_t		gl_father;
extern FILE		*gl_fscript;

void	restore_term(void)
{
  if (tcsetattr(STDIN_FILENO, (int) TCSAFLUSH, &gl_term_back) == -1)
    perror("my_script 0");
}

void	close_openfile(void)
{
  if (gl_fscript != NULL)
    if (fclose(gl_fscript) != 0)
      perror("my_script");
  gl_fscript = NULL;
  if (gl_pty[MASTER] != -1)
    if (close(gl_pty[MASTER]) == -1)
     perror("my_script");
  gl_pty[MASTER] = -1;
  if (gl_pty[SLAVE] != -1)
    if (close(gl_pty[SLAVE]) == -1)
      perror("my_script");
  gl_pty[SLAVE] = -1;
}

void	m_end(void)
{
  perror("my_script");
  if (getpid() == gl_father)
    restore_term();
  close_openfile();
  exit(1);
}

