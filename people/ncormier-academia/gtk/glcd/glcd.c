/*
** toto.c for  in /goinfre
**
** Made by nicolas cormier
** Login   <cormie_n@epita.fr>
**
** Started on  Thu Mar 17 10:54:37 2005 nicolas cormier
** Last update Thu Mar 24 11:28:47 2005 nicolas cormier
*/

#include <stdio.h>
#include <gtk/gtk.h>
#include "gtk-lcd.h"

gint	CloseWindow (GtkWidget *widget, gpointer *data)
{
  gtk_main_quit();
  return(FALSE);
}

int		main(int argc, char **argv)
{
  GtkWidget	*window;
  GtkWidget	*lcd;
  GtkWidget	*vbox;
  char		*get;
  char		*get_b;


  gtk_init(&argc, &argv);
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  gtk_signal_connect(GTK_OBJECT(window), "delete_event", GTK_SIGNAL_FUNC(gtk_main_quit), NULL);
  gtk_container_border_width(GTK_CONTAINER(window), 10);

  vbox = gtk_vbox_new(FALSE, 0);
  gtk_container_add(GTK_CONTAINER(window), vbox);
  gtk_widget_show(vbox);

  lcd = gtk_lcd_new();
  gtk_box_pack_start(GTK_BOX(vbox), lcd, FALSE, FALSE, 0);
  gtk_widget_show(lcd);
  gtk_object_set(GTK_OBJECT(lcd), "GtkLcd::str", "Abcdefghkij", NULL);

  lcd = gtk_lcd_new();
  gtk_box_pack_start(GTK_BOX(vbox), lcd, FALSE, FALSE, 0);
  gtk_widget_show(lcd);
  gtk_object_set(GTK_OBJECT(lcd), "GtkLcd::color", "Green", NULL);
  gtk_object_set(GTK_OBJECT(lcd), "GtkLcd::str", "KlmnopQrst", NULL);

  lcd = gtk_lcd_new();
  gtk_box_pack_start(GTK_BOX(vbox), lcd, FALSE, FALSE, 0);
  gtk_widget_show(lcd);
  gtk_object_set(GTK_OBJECT(lcd), "GtkLcd::color", "White", NULL);
  gtk_object_set(GTK_OBJECT(lcd), "GtkLcd::str", "UvwxyZ", NULL);

  gtk_object_get(GTK_OBJECT(lcd), "GtkLcd::str", &get, NULL);
  gtk_object_get(GTK_OBJECT(lcd), "GtkLcd::color", &get_b, NULL);
  printf("%s est en %s\n", get, get_b);

  lcd = gtk_lcd_new();
  gtk_box_pack_start(GTK_BOX(vbox), lcd, FALSE, FALSE, 0);
  gtk_widget_show(lcd);
  gtk_object_set(GTK_OBJECT(lcd), "GtkLcd::color", "Red", NULL);
  gtk_object_set(GTK_OBJECT(lcd), "GtkLcd::str", "0123456789", NULL);

  lcd = gtk_lcd_new();
  gtk_box_pack_start(GTK_BOX(vbox), lcd, FALSE, FALSE, 0);
  gtk_widget_show(lcd);
  gtk_object_set(GTK_OBJECT(lcd), "GtkLcd::color", "#fc55f1", NULL);
  gtk_object_set(GTK_OBJECT(lcd), "GtkLcd::str", "+=_`*\\/-", NULL);
  gtk_widget_show(window);

  lcd = gtk_lcd_new();
  gtk_box_pack_start(GTK_BOX(vbox), lcd, FALSE, FALSE, 0);
  gtk_widget_show(lcd);
  gtk_object_set(GTK_OBJECT(lcd), "GtkLcd::color", "Blue", NULL);
  gtk_object_set(GTK_OBJECT(lcd), "GtkLcd::str", "<>^|[]:.", NULL);
  gtk_widget_show(window);

  gtk_main ();
  return (0);
}
