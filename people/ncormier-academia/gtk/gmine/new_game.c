/*
** new_game.c for  in /u/ept2/cormie_n/lang/c/gmine
**
** Made by nicolas cormier
** Login   <cormie_n@epita.fr>
**
** Started on  Mon Mar  7 10:34:43 2005 nicolas cormier
** Last update Wed Mar  9 15:49:23 2005 nicolas cormier
*/

#include <stdlib.h>
#include <gtk/gtk.h>

#include "gmine_struct.h"
#include "gmine_func_def.h"

extern t_gmine	*gl_gmine;

void		rinit_game(void)
{
  t_gmine	*gmine;
  t_button	*button;
  int		x;
  int		y;
  int		mined;
  int		next_mine;

  rinit_timer(gl_gmine);
  gmine = gl_gmine;
  for (y = 0; y < gmine->y; y++)
    for (x = 0; x < gmine->x; x++)
      {
	button = (gmine->buttons + ((y * gmine->x) + x));
	if (button->label)
	  gtk_container_remove(GTK_CONTAINER(button->widget), button->label);
	if (button->pixwid)
	  {
	    gtk_container_remove(GTK_CONTAINER(button->widget), button->pixwid);
	    gtk_widget_destroy(button->pixwid);
	  }
	mined = button->mined;
	next_mine = button->next_mine;
	init_button_struct(gmine, x, y, gmine->x);
	button->mined = mined;
	button->next_mine = next_mine;
      }
  gmine->dead = 0;
  gmine->cur_nb_flags = 0;
  free(gmine->undo);
  gmine->undo = NULL;
  apply_this_pixmap(&(gmine->smiley), gmine->smiley_worried);
  refresh_screen(gmine);
}

void		new_game(void)
{
  t_gmine	*gmine;
  t_button	*button;
  int		x;
  int		y;

  rinit_timer(gl_gmine);
  gmine = gl_gmine;
  for (y = 0; y < gmine->y; y++)
    for (x = 0; x < gmine->x; x++)
      {
	button = (gmine->buttons + ((y * gmine->x) + x));
	if (button->label)
	  gtk_container_remove(GTK_CONTAINER(button->widget), button->label);
	if (button->pixwid)
	  {
	    gtk_container_remove(GTK_CONTAINER(button->widget), button->pixwid);
	    gtk_widget_destroy(button->pixwid);
	  }
	init_button_struct(gmine, x, y, gmine->x);
      }
  init_mines(gmine);
  gmine->dead = 0;
  gmine->cur_nb_flags = 0;
  free(gmine->undo);
  gmine->undo = NULL;
  apply_this_pixmap(&(gmine->smiley), gmine->smiley_worried);
  refresh_screen(gmine);
}
