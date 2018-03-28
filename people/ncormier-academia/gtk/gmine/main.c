/*
** main.c for  in /u/ept2/cormie_n/lang/c/gmine
**
** Made by nicolas cormier
** Login   <cormie_n@epita.fr>
**
** Started on  Sun Feb  6 15:20:40 2005 nicolas cormier
** Last update Wed Mar  9 15:32:42 2005 nicolas cormier
*/

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "gmine_def.h"
#include "gmine_struct.h"
#include "gmine_func_def.h"

t_gmine		*gl_gmine;

static void	init_main(t_gmine *gmine)
{
  gmine->x = X;
  gmine->y = Y;
  gmine->nb_mine = 5;
  gmine->nb_flags = 5;
  gmine->dead = 0;
  gmine->cur_nb_flags = 0;
  gmine->undo = NULL;
}

int		main(int argc, char **argv)
{
  t_gmine	gmine;

  gl_gmine = &gmine;
  init_gtk(argc, argv, &gmine);
  init_main(&gmine);
  init_table(&gmine);
  if ((gmine.buttons = malloc(gmine.x * gmine.y * sizeof(t_button))) == NULL)
    exit(1);
  init_pixmap(&gmine);
  init_menu(&gmine);
  init_buttons(&gmine);
  init_mines(&gmine);
  init_text(&gmine);
  gtk_timeout_add(1000, (GtkFunction) m_timer, &gmine);
  gtk_widget_show_all(gmine.window);
  gtk_main();
  return (0);
}
