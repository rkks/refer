/*
** game.c for  in /u/ept2/cormie_n/lang/c/gmine
**
** Made by nicolas cormier
** Login   <cormie_n@epita.fr>
**
** Started on  Fri Mar  4 10:08:38 2005 nicolas cormier
** Last update Fri Mar  4 13:49:59 2005 nicolas cormier
*/

#include <gtk/gtk.h>

#include "gmine_struct.h"
#include "gmine_func_def.h"
#include "gmine_def.h"

void	chk_win(t_gmine *gmine)
{

  int	count;
  int	count_b;

  if (gmine->dead == 1)
    return;
  for (count = 0; count < gmine->y; count++)
    for (count_b = 0; count_b < gmine->x; count_b++)
      if ((gmine->buttons + ((count * gmine->x) + count_b))->pressed == 0 &&
	  (gmine->buttons + ((count * gmine->x) + count_b))->mined == 0)
	return;
  for (count = 0; count < gmine->y; count++)
    for (count_b = 0; count_b < gmine->x; count_b++)
      if ((gmine->buttons + ((count * gmine->x) + count_b))->mined == 1)
	(gmine->buttons + ((count * gmine->x) + count_b))->etat = 1;
  gmine->dead = 1;
  boom(gmine, gmine->smiley_win, WIN);
}
