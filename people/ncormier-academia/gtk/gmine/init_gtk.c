/*
** init_gtk.c for  in /u/ept2/cormie_n/lang/c/gmine
**
** Made by nicolas cormier
** Login   <cormie_n@epita.fr>
**
** Started on  Mon Feb  7 16:08:12 2005 nicolas cormier
** Last update Wed Mar  9 15:02:27 2005 nicolas cormier
*/

#include <gtk/gtk.h>

#include "gmine_func_def.h"

static gint	delete_event(GtkWidget *widget, GdkEvent *event, gpointer data)
{
  gtk_main_quit();
  return (TRUE);
}

static void	destroy(GtkWidget *widget, gpointer data)
{
  gtk_main_quit();
}

void	init_main_window(t_gmine *gmine)
{
  gmine->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size((GtkWindow*) gmine->window, (gint) 250,
			      (gint) 300);
  gtk_signal_connect(GTK_OBJECT(gmine->window), "delete_event",
		     GTK_SIGNAL_FUNC(delete_event), NULL);
  gtk_signal_connect(GTK_OBJECT(gmine->window), "destroy",
		     GTK_SIGNAL_FUNC(destroy), NULL);
  gtk_container_set_border_width(GTK_CONTAINER(gmine->window), 10);
  gtk_window_set_position(GTK_WINDOW(gmine->window), GTK_WIN_POS_CENTER);
  gtk_widget_show(gmine->window);
}

void	init_gtk(int argc, char **argv, t_gmine *gmine)
{
  gtk_init(&argc, &argv);
  gtk_rc_parse("rc/xliquidgtk/gtkrc");
  init_main_window(gmine);
}
