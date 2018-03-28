/*
** mine_init.c for  in /home/nico/gmine
**
** Made by nicolas
** Login   <nico@epita.fr>
**
** Started on  Tue Feb  8 18:50:41 2005 nicolas
** Last update Wed Mar  9 14:59:32 2005 nicolas cormier
*/

#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "gmine_struct.h"
#include "gmine_func_def.h"

static const int	gl_xy[] =
  {
    -1, +0,
    +1, +0,
    +0, -1,
    +0, +1,
    -1, -1,
    -1, +1,
    +1, -1,
    +1, +1
  };

static void	init_next_mines(t_gmine *gmine, int randx, int randy)
{
  int		count;

  for (count = 0; count < 16; count += 2)
    if ((randx + gl_xy[count]) < gmine->x &&
	(randx + gl_xy[count]) >= 0 &&
	(randy + gl_xy[count + 1]) < gmine->y &&
	(randy + gl_xy[count + 1]) >= 0)
      (gmine->buttons + ((randy + gl_xy[count + 1]) * gmine->x)
       + randx + gl_xy[count])->next_mine += 1;
}

void	init_mines(t_gmine *gmine)
{
  int	nb_mine;
  int	randx;
  int	randy;

  srandom(time(0) * getpid());
  for (nb_mine = gmine->nb_mine; nb_mine;)
    {
      randx = random() % gmine->x;
      randy = random() % gmine->y;
      if ((gmine->buttons + (randy * gmine->x) + randx)->mined != 1)
	{
	  (gmine->buttons + (randy * gmine->x) + randx)->mined = 1;
	  init_next_mines(gmine, randx, randy);
	  nb_mine--;
	}
    }
}
