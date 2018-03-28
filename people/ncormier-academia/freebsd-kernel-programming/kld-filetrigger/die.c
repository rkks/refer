/*
** die.c for  in /home/nico/lang/kernel/kld-filetrigger
** 
** Made by nicolas
*/

#ifndef _KERNEL
# include <stdio.h>
# include <stdlib.h>
#else
# include <sys/param.h>
# include <sys/systm.h>
/* DELETE */
# include <sys/param.h>
# include <sys/systm.h>
# include <sys/proc.h>
#endif /* _KERNEL */
#include "die.h"

void	die(const char* msg)
{
#ifndef _KERNEL
  printf("%s\n", msg);
  exit(1);
#else
  int i;
  void* ptr = &i;
  printf("%s\n", msg);
  (void) tsleep(ptr, PCATCH|PWAIT, "idle", 0);
#endif /* _KERNE */
}
