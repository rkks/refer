/*
** unhide_game.c for  in /u/ept2/cormie_n/lang/c/gmine
**
** Made by nicolas cormier
** Login   <cormie_n@epita.fr>
**
** Started on  Fri Mar  4 13:27:27 2005 nicolas cormier
** Last update Fri Mar  4 14:04:04 2005 nicolas cormier
*/

#include <gtk/gtk.h>

#include "gmine_struct.h"
#include "gmine_func_def.h"
#include "gmine_def.h"

static void	del_question(t_button *button)
{
  if (button->etat == 2 && button->pixwid)
    {
      gtk_container_remove(GTK_CONTAINER(button->widget),
			   button->pixwid);
      gtk_widget_destroy(button->pixwid);
      button->pixwid = NULL;
    }
}

void	boom(t_gmine *gmine, t_pixmap *smiley, int type)
{
  int		count;

  for (count = 0; count < (gmine->x * gmine->y); count++)
    {
      del_question(gmine->buttons + count);
      (gmine->buttons + count)->pressed = 1;
      if ((gmine->buttons + count)->mined == 0 &&
	  (gmine->buttons + count)->etat == 1 &&
	  type == LOSE)
	apply_this_pixmap((gmine->buttons + count), gmine->flag_bad);
      else if (((gmine->buttons + count)->mined == 1 &&
	       (gmine->buttons + count)->etat == 1)
	       ||
	       ((gmine->buttons + count)->mined == 1 && type == WIN))
	apply_this_pixmap((gmine->buttons + count), gmine->flag);
      else if ((gmine->buttons + count)->mined == 1)
	apply_this_pixmap((gmine->buttons + count), gmine->bomb);
    }
  gmine->dead = 1;
  apply_this_pixmap(&(gmine->smiley), smiley);
}
