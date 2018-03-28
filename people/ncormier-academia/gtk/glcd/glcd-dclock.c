/*
** glcd-dclock.c for  in /home/nico/instances/glcd
** 
** Made by nicolas
** Login   <nico@epita.fr>
** 
** Started on  Wed Mar 23 23:35:17 2005 nicolas
** Last update Wed Mar 23 23:52:42 2005 nicolas
*/

#include <gtk/gtk.h>
#include "gtk-dclock.h"

gint	CloseWindow (GtkWidget *widget, gpointer *data)
{
    gtk_main_quit();
    return (FALSE);
}

int	main(int argc, char **argv)
{
  GtkWidget *window;
  GtkWidget *lcd;

  gtk_init(&argc, &argv);
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  gtk_signal_connect(GTK_OBJECT(window), "delete_event", GTK_SIGNAL_FUNC(gtk_main_quit), NULL);
  gtk_container_border_width(GTK_CONTAINER(window), 10);

  lcd = gtk_dclock_new();
  gtk_container_add(GTK_CONTAINER(window), lcd);
  gtk_widget_show(lcd);
  gtk_object_set(GTK_OBJECT(lcd), "GtkLcd::bgcolor", "#000e0e", NULL);
  gtk_object_set(GTK_OBJECT(lcd), "GtkLcd::color", "Green", NULL);

  gtk_widget_show(window);

  gtk_main ();
  return(0);
}
