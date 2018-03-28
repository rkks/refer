/*
** flag.c for  in /home/nico/instances/gmine
**
** Made by nicolas
** Login   <nico@epita.fr>
**
** Started on  Sat Mar  5 11:32:37 2005 nicolas
** Last update Wed Mar  9 10:51:21 2005 nicolas cormier
*/

#include <gtk/gtk.h>

#include "gmine_struct.h"
#include "gmine_func_def.h"

void	flag_manager(t_gmine *gmine, t_button *button)
{
  if (gmine->cur_nb_flags == gmine->nb_flags && button->etat == 0)
    button->etat++;
  else if (button->etat == 0)
    gmine->cur_nb_flags++;
  else if (button->etat == 1)
    gmine->cur_nb_flags--;
  apply_pixmap_bomb(gmine, button);
  gtk_widget_show(button->widget);
}
