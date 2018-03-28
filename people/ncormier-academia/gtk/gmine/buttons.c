/*
** buttons.c for  in /u/ept2/cormie_n/lang/c/gmine
**
** Made by nicolas cormier
** Login   <cormie_n@epita.fr>
**
** Started on  Mon Feb  7 16:13:07 2005 nicolas cormier
** Last update Wed Mar  9 14:13:50 2005 nicolas cormier
*/

#include <gtk/gtk.h>

#include "gmine_struct.h"
#include "gmine_func_def.h"

extern t_gmine	*gl_gmine;

void		hello_button(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
  t_button	*button;

  button = (t_button *) data;
  if (button->pressed == 1 || gl_gmine->dead == 1)
    {
      refresh_screen(gl_gmine);
      return;
    }
  add_undo_act(gl_gmine, event->button, button->x, button->y);
  if (event->button == 3)
    flag_manager(gl_gmine, button);
  else if (!button->pixwid)
    {
      unhide_mines(gl_gmine, button);
      gtk_toggle_button_set_active((GtkToggleButton *) widget, 0);
    }
  refresh_screen(gl_gmine);
}

void	init_button_struct(t_gmine *gmine, int x, int y, int x_max)
{
  (gmine->buttons + ((y * x_max) + x))->pixwid = NULL;
  (gmine->buttons + ((y * x_max) + x))->label = NULL;
  (gmine->buttons + ((y * x_max) + x))->etat = 0;
  (gmine->buttons + ((y * x_max) + x))->mined = 0;
  (gmine->buttons + ((y * x_max) + x))->pressed = 0;
  (gmine->buttons + ((y * x_max) + x))->next_mine = 0;
  (gmine->buttons + ((y * x_max) + x))->x = x;
  (gmine->buttons + ((y * x_max) + x))->y = y;
}

static void	add_button(t_gmine *gmine, int x, int y, int x_max)
{
  (gmine->buttons + ((y * x_max) + x))->widget = gtk_toggle_button_new();
  init_button_struct(gmine, x, y, x_max);
  gtk_signal_connect_after(GTK_OBJECT((gmine->buttons + ((y * x_max) + x))->widget),
		     "button_release_event",
		     GTK_SIGNAL_FUNC(hello_button),
		     (gpointer) (gmine->buttons + ((y * x_max) + x)));
  gtk_table_attach_defaults(GTK_TABLE(gmine->table),
			    (gmine->buttons + ((y * x_max) + x))->widget
			    , x, x + 1, y + 4, y + 5);
}

void	init_buttons(t_gmine *gmine)
{
  int	count;
  int	count_b;

  for (count = 0; count < gmine->y; count++)
    for (count_b = 0; count_b < gmine->x; count_b++)
      add_button(gmine, count_b, count, gmine->x);
}
