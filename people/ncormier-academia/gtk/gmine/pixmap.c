/*
** pixmap.c for  in /u/ept2/cormie_n/lang/c/gmine
**
** Made by nicolas cormier
** Login   <cormie_n@epita.fr>
**
** Started on  Mon Feb  7 15:55:30 2005 nicolas cormier
** Last update Sat Mar  5 12:11:25 2005 nicolas
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "gmine_struct.h"
#include "gmine_func_def.h"

#include "xpm/flag.xpm"
#include "xpm/flag_bad.xpm"
#include "xpm/mark.xpm"
#include "xpm/mine.xpm"
#include "xpm/smiley_def.xpm"
#include "xpm/smiley_lose.xpm"
#include "xpm/smiley_win.xpm"
#include "xpm/smiley_worried.xpm"

static t_pixmap	*init_this_pixmap(t_gmine *gmine, char *xpm)
{
  t_pixmap	*ret;
  GdkPixmap	*pixmap;
  GdkBitmap	*mask;
  GtkStyle	*style;

  style = gtk_widget_get_style(gmine->window);
  pixmap = gdk_pixmap_create_from_xpm_d(gmine->window->window,  &mask,
					&style->bg[GTK_STATE_NORMAL],
					(gchar **) xpm);
  if ((ret = malloc(sizeof(t_pixmap))) == NULL)
    exit(1);
  ret->pixmap = pixmap;
  ret->mask = mask;
  return (ret);
}

void	init_pixmap(t_gmine *gmine)
{
  gmine->flag = init_this_pixmap(gmine, (char *) flag_xpm);
  gmine->flag_bad = init_this_pixmap(gmine, (char *) flag_bad_xpm);
  gmine->question = init_this_pixmap(gmine, (char *) mark_xpm);
  gmine->bomb = init_this_pixmap(gmine, (char *) mine_xpm);
  gmine->bomb = init_this_pixmap(gmine, (char *) mine_xpm);
  gmine->smiley_def = init_this_pixmap(gmine, (char *) smil_smile_xpm);
  gmine->smiley_lose = init_this_pixmap(gmine, (char *) smil_sad_xpm);
  gmine->smiley_win = init_this_pixmap(gmine, (char *) smiley_win_xpm);
  gmine->smiley_worried = init_this_pixmap(gmine, (char *) smil_worried_xpm);
}
void		attach_label(t_button *button)
{
  char		buf[BUFSIZ];

  sprintf(buf, "%d", button->next_mine);
  button->label = gtk_label_new(buf);
  gtk_container_add(GTK_CONTAINER(button->widget), button->label);
  gtk_widget_show(button->label);
  gtk_widget_show(button->widget);
}

void		apply_pixmap_bomb(t_gmine *gmine, t_button *button)
{
  GtkWidget	*pixmapwid;

  if (button->pixwid)
    {
      gtk_container_remove(GTK_CONTAINER(button->widget), button->pixwid);
      gtk_widget_destroy(button->pixwid);
    }
  if (button->etat == 0)
    pixmapwid = gtk_pixmap_new(gmine->flag->pixmap, gmine->flag->mask);
  else if (button->etat == 1)
    pixmapwid = gtk_pixmap_new(gmine->question->pixmap,
			       gmine->question->mask);
  else
    {
      button->etat = 0;
      button->pixwid = NULL;
      return;
    }
  gtk_container_add(GTK_CONTAINER(button->widget), pixmapwid);
  gtk_widget_show(pixmapwid);
  button->etat++;
  button->pixwid = pixmapwid;
}

void		apply_this_pixmap(t_button *button, t_pixmap *cur)
{
  GtkWidget	*pixmapwid;

  if (button->pixwid)
    {
      gtk_container_remove(GTK_CONTAINER(button->widget), button->pixwid);
      gtk_widget_destroy(button->pixwid);
    }
  pixmapwid = gtk_pixmap_new(cur->pixmap, cur->mask);
  gtk_container_add(GTK_CONTAINER(button->widget), pixmapwid);
  gtk_widget_show(pixmapwid);
  button->pixwid = pixmapwid;
}
