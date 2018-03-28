/*
** text.c for  in /u/ept2/cormie_n/lang/c/gmine
**
** Made by nicolas cormier
** Login   <cormie_n@epita.fr>
**
** Started on  Fri Mar  4 10:17:17 2005 nicolas cormier
** Last update Wed Mar  9 15:50:39 2005 nicolas cormier
*/

#include <stdio.h>
#include <gtk/gtk.h>

#include "gmine_struct.h"
#include "gmine_func_def.h"

void	init_text(t_gmine *gmine)
{
  char	buf[BUFSIZ];

  sprintf(buf, "%d/%d flags               00:00:00",
	  gmine->cur_nb_flags, gmine->nb_flags, gmine->timer);
  gmine->text = gtk_label_new(buf);
  gtk_table_attach_defaults(GTK_TABLE(gmine->table),
			    gmine->text,
			    0, gmine->x, gmine->y + 4, gmine->y + 5);
}

void	refresh_text(t_gmine *gmine)
{
  char	buf[BUFSIZ];

  sprintf(buf, "%d/%d flags               %s",
	  gmine->cur_nb_flags, gmine->nb_flags, gmine->timer);
  gtk_label_set_text(GTK_LABEL(gmine->text), buf);
  gtk_widget_show(gmine->text);
}
