/*
** timer.c for  in /u/ept2/cormie_n/lang/c/gmine
**
** Made by nicolas cormier
** Login   <cormie_n@epita.fr>
**
** Started on  Wed Mar  9 15:19:06 2005 nicolas cormier
** Last update Wed Mar  9 15:50:00 2005 nicolas cormier
*/

#include <time.h>
#include <gtk/gtk.h>

#include "gmine_struct.h"
#include "gmine_func_def.h"

void	rinit_timer(t_gmine *gmine)
{
  gmine->dead = 1;
  m_timer(gmine);
}

int		m_timer(t_gmine *gmine)
{
  static int	init = -1;
  static time_t	start;
  time_t	cur;

  if (gmine->dead == 1)
    {
      init = -1;
      return (1);
    }
  if (init == -1)
    {
      init = 1;
      time(&start);
    }
  time(&cur);
  cur = cur - start - 3600;
  strftime(gmine->timer, sizeof(gmine->timer), "%H:%M:%S"
           , localtime(&cur));
  refresh_text(gmine);
  return (1);
}
