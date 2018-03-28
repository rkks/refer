/*
** option.c for  in /home/nico/instances/gmine
**
** Made by nicolas
** Login   <nico@epita.fr>
**
** Started on  Sat Mar  5 21:02:15 2005 nicolas
** Last update Wed Mar  9 12:36:16 2005 nicolas cormier
*/

#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>

#include "gmine_struct.h"
#include "gmine_func_def.h"

extern t_gmine	*gl_gmine;

static gint	delete_event(GtkWidget *widget, GdkEvent *event, gpointer data)
{
  gtk_widget_destroy(widget);
  return(TRUE);
}

static GtkWidget	*init_entries(char *title, int def, GtkWidget *vbox)
{
  GtkWidget		*ret;
  GtkWidget		*frame;
  char			buf[BUFSIZ];

  frame = gtk_frame_new (title);
  sprintf(buf, "%d", def);
  ret = gtk_entry_new_with_max_length(50);
  gtk_entry_set_text(GTK_ENTRY(ret), buf);
  gtk_entry_select_region(GTK_ENTRY(ret),
			   0, GTK_ENTRY(ret)->text_length);
  gtk_container_add(GTK_CONTAINER(frame), ret);
  gtk_box_pack_start(GTK_BOX(vbox), frame, FALSE, FALSE, 0);
  gtk_widget_show(ret);
  gtk_widget_show(frame);
  return (ret);
}

static void	init_entries_and_button(GtkWidget *window, GtkWidget *vbox)
{
  t_option	*options;
  GtkWidget	*button;

  if ((options = malloc(sizeof(t_option))) == NULL)
    exit(1);
  options->case_x = init_entries("Col. Nbr.", gl_gmine->x, vbox);
  options->case_y = init_entries("Line Nbr.", gl_gmine->y, vbox);
  options->nb_mine = init_entries("Bomb Nbr.", gl_gmine->nb_mine, vbox);
  options->window = window;
  button = gtk_button_new_with_label("Ok");
  gtk_signal_connect(GTK_OBJECT(button), "button_release_event",
		     GTK_SIGNAL_FUNC(valid_event), (gpointer) options);
  gtk_box_pack_start(GTK_BOX(vbox), button, TRUE, TRUE, 0);
  GTK_WIDGET_SET_FLAGS(button, GTK_CAN_DEFAULT);
  gtk_widget_grab_default(button);
  gtk_widget_show (button);
  gtk_widget_show (window);
}

void		option_window(void)
{
  GtkWidget	*window;
  GtkWidget	*vbox;

  window = gtk_window_new(GTK_WINDOW_DIALOG);
  gtk_window_set_default_size((GtkWindow*) window, (gint) 50,
			      (gint) 50);
  gtk_signal_connect(GTK_OBJECT(window), "delete_event",
		     GTK_SIGNAL_FUNC(delete_event), gl_gmine);
  gtk_signal_connect(GTK_OBJECT(window), "delete_event",
		     GTK_SIGNAL_FUNC(delete_event), gl_gmine);
  gtk_container_set_border_width(GTK_CONTAINER(window), 10);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  vbox = gtk_vbox_new(FALSE, 0);
  gtk_container_add(GTK_CONTAINER(window), vbox);
  gtk_widget_show(vbox);
  init_entries_and_button(window, vbox);
}
