/*
** term.c for  in /u/ept2/cormie_n/lang/c/script
**
** Made by nicolas cormier
** Login   <cormie_n@epita.fr>
**
** Started on  Fri Feb  4 12:22:55 2005 nicolas cormier
** Last update Sun Feb  6 14:28:52 2005 nicolas cormier
*/

#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "my_script.h"

extern struct termios	gl_term_back;
extern int		gl_pty[2];


void	inhib_rterm(struct termios *term)
{
  term->c_lflag &= ~(ICANON | ECHO | ISIG);
  term->c_cc[VMIN] = 1;
  term->c_cc[VTIME] = 0;
  if (tcsetattr(STDIN_FILENO, (int) TCSAFLUSH, term) == -1)
    m_end();
}

void	init_vterm(int *pty, struct termios *term, struct winsize *win)
{
  if (tcgetattr(STDIN_FILENO, term) == -1)
    m_end();
  gl_term_back = *term;
  if (ioctl(STDIN_FILENO, (int) TIOCGWINSZ, win) == -1)
    m_end();
  m_openpty(pty + MASTER, pty + SLAVE, term, win);
  gl_pty[MASTER] = pty[MASTER];
  gl_pty[SLAVE] = pty[SLAVE];
  if (tcsetattr(STDIN_FILENO, (int) TCSAFLUSH, term) == -1)
    m_end();
}
