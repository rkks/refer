/*
** mine.c for  in /u/ept2/cormie_n/lang/c/gmine
**
** Made by nicolas cormier
** Login   <cormie_n@epita.fr>
**
** Started on  Mon Feb  7 17:05:08 2005 nicolas cormier
** Last update Wed Mar  9 14:56:28 2005 nicolas cormier
*/

#include <stdlib.h>
#include <gtk/gtk.h>

#include "gmine_struct.h"
#include "gmine_func_def.h"
#include "gmine_def.h"

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

static void	show_nbr(t_gmine *gmine, t_button *button)
{
  if (button->pressed == 1)
    return;
  if (button->pixwid)
    {
      gtk_container_remove(GTK_CONTAINER(button->widget), button->pixwid);
      gtk_widget_destroy(button->pixwid);
      button->pixwid = NULL;
    }
  if (button->etat == 1)
    {
      gmine->cur_nb_flags--;
      button->etat = 0;
    }
  attach_label(button);
  button->pressed = 1;
  gtk_toggle_button_set_active((GtkToggleButton *) button->widget, 1);
  return;
}

static int	test_rec(t_gmine *gmine, t_button *button, int count)
{
  return
    (
      (button->x + gl_xy[count]) >= 0 &&
      (button->y + gl_xy[count + 1]) >= 0 &&
      (button->x + gl_xy[count]) < gmine->x &&
      (button->y + gl_xy[count + 1]) < gmine->y &&
      (button + gl_xy[count] + (gmine->x * gl_xy[count + 1]))->mined != 1 &&
      (button + gl_xy[count] + (gmine->x * gl_xy[count + 1]))->pressed != 1
      );
}

int		unhide_mines(t_gmine *gmine, t_button *button)
{
  int		count;

  if (button->etat == 2)
    {
      gtk_container_remove(GTK_CONTAINER(button->widget), button->pixwid);
      gtk_widget_destroy(button->pixwid);
      button->pixwid = NULL;
    }
  if (button->mined == 1)
    return (boom(gmine, gmine->smiley_lose, LOSE), 0);
  if (button->next_mine != 0)
    return (show_nbr(gmine, button), 0);
  button->pressed = 1;
  if (button->pixwid)
    {
      gtk_container_remove(GTK_CONTAINER(button->widget), button->pixwid);
      gtk_widget_destroy(button->pixwid);
      button->pixwid = NULL;
    }
  if (button->etat == 1)
    {
      gmine->cur_nb_flags--;
      button->etat = 0;
    }
  for (count = 0; count < 16; count += 2)
    if (test_rec(gmine, button, count))
	unhide_mines(gmine, (button + gl_xy[count] + (gmine->x * gl_xy[count + 1])));
  return (0);
}
