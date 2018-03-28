/*
** my_script.h for  in /u/ept2/cormie_n/lang/c/script
**
** Made by nicolas cormier
** Login   <cormie_n@epita.fr>
**
** Started on  Fri Feb  4 12:10:42 2005 nicolas cormier
** Last update Fri Feb  4 14:50:18 2005 nicolas cormier
*/

#ifndef __MY_SCRIPT_H
#define __MY_SCRIPT_H

#define	DEF_FILE	"typescript";

#define MASTER		0
#define SLAVE		1

#define IS_CHILD	0

/* UTIL_C */
int	m_login_tty(int fd);
#ifndef __alpha
void	m_openpty(int *amaster, int *aslave, struct termios *termp, struct winsize *winp);
#endif

/* END_C */
void	m_end(void);
void	restore_term(void);
void	close_openfile(void);

#include <stdio.h>
/* CHILD_C */
void	fork_child(int *pty, FILE *fscript);

/* TERM_C */
void	inhib_rterm(struct termios *term);
#ifndef __alpha
void	init_vterm(int *pty, struct termios *term, struct winsize *win);
#endif

#endif /* __MY_SCRIPT_H */
