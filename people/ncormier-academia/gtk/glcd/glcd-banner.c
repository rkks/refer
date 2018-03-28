/*
** toto.c for  in /goinfre
**
** Made by nicolas cormier
** Login   <cormie_n@epita.fr>
**
** Started on  Thu Mar 17 10:54:37 2005 nicolas cormier
** Last update Wed Mar 23 22:37:11 2005 nicolas cormier
*/

#include <gtk/gtk.h>
#include "gtk-lcd-banner.h"

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

  lcd = gtk_lcd_banner_new();
  gtk_container_add(GTK_CONTAINER(window), lcd);
  gtk_widget_show(lcd);

  gtk_object_set(GTK_OBJECT(lcd), "GtkLcd_Banner::nbr", 8, NULL);
  gtk_object_set(GTK_OBJECT(lcd), "GtkLcd::bgcolor", "#000e0e", NULL);
  gtk_object_set(GTK_OBJECT(lcd), "GtkLcd::color", "Green", NULL);
  gtk_object_set(GTK_OBJECT(lcd), "GtkLcd_Banner::str", "salut toto ", NULL);
  gtk_widget_show(window);

  gtk_main ();
  return(0);
}
