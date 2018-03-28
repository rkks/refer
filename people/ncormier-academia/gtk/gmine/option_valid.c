/*
** option_valid.c for  in /home/nico/instances/gmine
**
** Made by nicolas
** Login   <nico@epita.fr>
**
** Started on  Sun Mar  6 10:55:56 2005 nicolas
** Last update Wed Mar  9 15:51:24 2005 nicolas cormier
*/

#include <stdlib.h>
#include <gtk/gtk.h>

#include "gmine_struct.h"
#include "gmine_func_def.h"

extern t_gmine	*gl_gmine;

static int	get_value_from_entry(char *entry)
{
  if (!(*entry >= '0' && *entry <= '9'))
    return (-1);
  return(atoi(entry));
}

static void	reinit_game(t_gmine *gmine)
{
  if ((gl_gmine->buttons = malloc(gl_gmine->x * gl_gmine->y * sizeof(t_button))) == NULL)
    exit(1);
  rinit_timer(gmine);
  gl_gmine->dead = 0;
  gl_gmine->cur_nb_flags = 0;
  gl_gmine->undo = NULL;
  init_main_window(gl_gmine);
  init_table(gl_gmine);
  init_menu(gl_gmine);
  init_buttons(gl_gmine);
  init_mines(gl_gmine);
  init_text(gl_gmine);
}

static int	check_entries(t_option *options)
{
  int		x;
  int		y;
  int		nb_bomb;

  if ((x = get_value_from_entry(gtk_entry_get_text
				(GTK_ENTRY(options->case_x)))) < 10)
    return (gtk_error("  Bad Col value ! (min 10)  "), -1);
  if ((y = get_value_from_entry(gtk_entry_get_text
				(GTK_ENTRY(options->case_y)))) < 10)
    return (gtk_error("  Bad Line value ! (min 10)  "), -1);
  if ((nb_bomb = get_value_from_entry(gtk_entry_get_text
				      (GTK_ENTRY(options->nb_mine)))) <= 0)
    return (gtk_error("  Bad Bomb Nbr !  (min 1) "), -1);
  if (nb_bomb >= (x * y))
    return (gtk_error
	    ("  The number of Bomb must be lesser than column * line!  "), -1);
  return (0);
}

gint	valid_event(GtkWidget *widget, GdkEvent *event, gpointer data)
{
  t_option	*options;

  options = (t_option *) data;
  if (check_entries(options) != 0)
    return (0);
  gtk_widget_destroy(gl_gmine->window);
  free(gl_gmine->buttons);
  free(gl_gmine->undo);
  gl_gmine->x =
    get_value_from_entry(gtk_entry_get_text(GTK_ENTRY(options->case_x)));
  gl_gmine->y =
    get_value_from_entry(gtk_entry_get_text(GTK_ENTRY(options->case_y)));
  gl_gmine->nb_mine =
    get_value_from_entry(gtk_entry_get_text(GTK_ENTRY(options->nb_mine)));
  gl_gmine->nb_flags = gl_gmine->nb_mine;
  reinit_game(gl_gmine);
  gtk_widget_destroy(options->window);
  free(options);
  gtk_widget_show_all(gl_gmine->window);
  refresh_screen(gl_gmine);
  gtk_main();
  return(TRUE);
}
