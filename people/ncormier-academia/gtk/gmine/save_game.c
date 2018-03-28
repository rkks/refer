/*
** save_game.c for  in /u/ept2/cormie_n/lang/c/gmine
**
** Made by nicolas cormier
** Login   <cormie_n@epita.fr>
**
** Started on  Mon Mar  7 10:36:05 2005 nicolas cormier
** Last update Wed Mar  9 15:03:52 2005 nicolas cormier
*/

#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <gtk/gtk.h>

#include "gmine_struct.h"
#include "gmine_func_def.h"

extern t_gmine	*gl_gmine;

static void	save_cur(char *path)
{
  int		fd;
  int		nbr;

  if ((fd = open(path, O_WRONLY | O_TRUNC | O_CREAT, 0644)) == -1)
    return;
  if (write(fd, gl_gmine, sizeof(t_gmine)) == -1)
    return;
  if (write(fd, gl_gmine->buttons,
	    gl_gmine->x * gl_gmine->y * sizeof(t_button))
      == -1)
    return;
  for (nbr = 0; gl_gmine->undo[nbr].act != -1; nbr++)
    ;
  if (write(fd, &nbr,  sizeof(int)) == -1)
    ;
  if (write(fd, gl_gmine->undo,  nbr * sizeof(t_undo))
      == -1)
    return;
  if (close(fd) == -1)
    return;
}

static void	file_ok_sel(GtkWidget *w, GtkFileSelection *fs, gpointer data)
{
  GtkWidget	*filew;

  filew = (GtkWidget *) data;
  save_cur(gtk_file_selection_get_filename(GTK_FILE_SELECTION(filew)));
  gtk_widget_destroy(data);
}

void		save_game(void)
{
  GtkWidget	*filew;

  if (!gl_gmine->undo)
    {
      gtk_error("  Please start game before save !  ");
      return;
    }
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
