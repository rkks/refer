/*
** error.c for  in /home/nico/instances/gmine
**
** Made by nicolas
** Login   <nico@epita.fr>
**
** Started on  Tue Mar  8 23:14:16 2005 nicolas
** Last update Wed Mar  9 14:14:01 2005 nicolas cormier
*/

#include <gtk/gtk.h>

#include "gmine_struct.h"
#include "gmine_func_def.h"

static gint	ok_event(GtkWidget *widget, GdkEvent *event, gpointer data)
{
  gtk_widget_destroy((GtkWidget *) data);
  return (TRUE);
}

void		gtk_error(char *strerr)
{
  GtkWidget	*window;
  GtkWidget	*label;
  GtkWidget	*button;


  window = gtk_dialog_new();
  button = gtk_button_new_with_label("Ok");
  gtk_signal_connect(GTK_OBJECT(button), "button_release_event",
		     GTK_SIGNAL_FUNC(ok_event), (gpointer) window);
  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(window)->action_area),
		     button, TRUE, TRUE, 0);
  gtk_widget_show(button);
  label = gtk_label_new(strerr);
  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(window)->vbox),
		     label, TRUE, TRUE, 0);
  gtk_widget_show(label);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_widget_show(window);
}
