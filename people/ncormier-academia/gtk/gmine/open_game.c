/*
** open_game.c for  in /home/nico/instances/gmine
**
** Made by nicolas
** Login   <nico@epita.fr>
**
** Started on  Tue Mar  8 19:24:50 2005 nicolas
** Last update Wed Mar  9 14:51:02 2005 nicolas cormier
*/

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "gmine_struct.h"
#include "gmine_func_def.h"

extern t_gmine	*gl_gmine;

static void	init_cur(int fd)
{
  t_gmine	tmp;

  if (read(fd, &tmp, sizeof(t_gmine)) == -1)
    return;
  gtk_widget_destroy(gl_gmine->window);
  free(gl_gmine->buttons);
  free(gl_gmine->undo);
  *gl_gmine = tmp;
  gl_gmine->dead = 0;
  gl_gmine->cur_nb_flags = 0;
  if ((gl_gmine->buttons =
       malloc(gl_gmine->x * gl_gmine->y * sizeof(t_button))) == NULL)
    exit(1);
  init_main_window(gl_gmine);
  init_table(gl_gmine);
  init_menu(gl_gmine);
  init_buttons(gl_gmine);
  init_mines(gl_gmine);
  init_text(gl_gmine);
}

static void	restore_cur(int fd)
{
  t_undo	*undo;
  int		count;

  if (read(fd, &count, sizeof(int)) == -1)
    return;
  if ((gl_gmine->undo = malloc((count + 2) * sizeof(t_undo))) == NULL)
    exit(1);
  if (read(fd, gl_gmine->undo, count * sizeof(t_undo)) == -1)
    return;
  undo = gl_gmine->undo;
  undo += count;
  undo->act = 0;
  undo++;
  undo->act = -1;
  undo_game();
}

static void     load_cur(char *path, GtkWidget *todel)
{
  t_button	button;
  t_button	*ptr;
  int		fd;
  int		count;

  if ((fd = open(path, O_RDONLY, 0644)) == -1)
    {
      gtk_error("  No Such File  ");
      return;
    }
  init_cur(fd);
  ptr = gl_gmine->buttons;
  for (count = 0; count < (gl_gmine->x * gl_gmine->y); count++)
    {
      if (read(fd, &button, sizeof(t_button)) == -1)
	return;
      ptr->mined = button.mined;
      ptr->next_mine = button.next_mine;
      ptr++;
    }
  restore_cur(fd);
  gtk_widget_show_all(gl_gmine->window);
  gtk_widget_destroy(todel);
  refresh_screen(gl_gmine);
  gtk_main();
}

static void	file_ok_sel(GtkWidget *w, GtkFileSelection *fs, gpointer data)
{
  GtkWidget	*filew;

  filew = (GtkWidget *) data;
  load_cur(gtk_file_selection_get_filename(GTK_FILE_SELECTION(filew)), filew);
}

void		load_game(void)
{
   GtkWidget	*filew;

  filew = gtk_file_selection_new ("File selection");
  gtk_signal_connect_after(GTK_OBJECT(GTK_FILE_SELECTION(filew)->ok_button),
		     "button_release_event",
			   (GtkSignalFunc) file_ok_sel, (gpointer) filew);
  gtk_signal_connect_object(GTK_OBJECT(GTK_FILE_SELECTION
					(filew)->cancel_button),
			     "clicked", (GtkSignalFunc) gtk_widget_destroy,
			     GTK_OBJECT(filew));
  gtk_window_set_position(GTK_WINDOW(filew), GTK_WIN_POS_CENTER);
  gtk_widget_show(filew);
}
