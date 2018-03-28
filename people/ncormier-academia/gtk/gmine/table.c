/*
** table.c for  in /u/ept2/cormie_n/lang/c/gmine
**
** Made by nicolas cormier
** Login   <cormie_n@epita.fr>
**
** Started on  Mon Feb  7 16:16:50 2005 nicolas cormier
** Last update Thu Mar  3 15:00:28 2005 nicolas cormier
*/

#include <gtk/gtk.h>

#include "gmine_struct.h"
#include "gmine_func_def.h"

void	init_table(t_gmine *gmine)
{
  gmine->table = gtk_table_new (gmine->y + 2, gmine->x, TRUE);
  gtk_container_add(GTK_CONTAINER(gmine->window), gmine->table);
}
