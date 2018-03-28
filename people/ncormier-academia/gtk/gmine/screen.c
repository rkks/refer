/*
** screen.c for  in /u/ept2/cormie_n/lang/c/gmine
**
** Made by nicolas cormier
** Login   <cormie_n@epita.fr>
**
** Started on  Fri Mar  4 10:51:01 2005 nicolas cormier
** Last update Fri Mar  4 14:08:30 2005 nicolas cormier
*/

#include <gtk/gtk.h>

#include "gmine_struct.h"
#include "gmine_func_def.h"

static  void	refresh_button(t_gmine *gmine, int x, int y, int x_max)
{
  t_button	*cur;

  cur = (gmine->buttons + ((y * x_max) + x));
  gtk_toggle_button_set_active((GtkToggleButton *) cur->widget, 0);
  if (cur->pressed == 0)
    return;
  gtk_toggle_button_set_active((GtkToggleButton *) cur->widget, 1);
}

void	refresh_screen(t_gmine *gmine)
{
  int	count;
  int	count_b;

  chk_win(gmine);
  for (count = 0; count < gmine->y; count++)
    for (count_b = 0; count_b < gmine->x; count_b++)
      refresh_button(gmine, count_b, count, gmine->x);
  refresh_text(gmine);
}
