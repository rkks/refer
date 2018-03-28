/*
** undo.c for  in /u/ept2/cormie_n/lang/c/gmine
**
** Made by nicolas cormier
** Login   <cormie_n@epita.fr>
**
** Started on  Mon Mar  7 10:43:38 2005 nicolas cormier
** Last update Mon Mar  7 17:47:57 2005 nicolas cormier
*/

#include <stdlib.h>
#include <gtk/gtk.h>

#include "gmine_struct.h"
#include "gmine_func_def.h"

extern t_gmine	*gl_gmine;

void	add_undo_act(t_gmine *gmine, int act, int x, int y)
{
  int	nbr;

  for (nbr = 0; gmine->undo && gmine->undo[nbr].act != -1; nbr++);
  if ((gmine->undo = realloc(gmine->undo, (nbr + 2) * sizeof(t_undo))) == NULL)
    exit(1);
  gmine->undo[nbr].act = act;
  gmine->undo[nbr].x = x;
  gmine->undo[nbr].y = y;
  gmine->undo[nbr + 1].act = -1;
}

void			undo_game(void)
{
  int			nbr;
  t_undo		*undo;
  t_button		*button;
  GdkEventButton	event;

  if (!gl_gmine->undo)
    return;
  for (nbr = 0; gl_gmine->undo[nbr].act != -1; nbr++);
  if ((undo = malloc((nbr + 1) * sizeof(t_undo))) == NULL)
    exit(1);
  memcpy((void *) undo, (void *) gl_gmine->undo, (nbr + 1) * sizeof(t_undo));
  rinit_game();
  for (nbr = 0; undo[nbr + 1].act != -1; nbr++)
  {
    button = gl_gmine->buttons + ((undo[nbr].y * gl_gmine->x) +
				  undo[nbr].x);
    event.button = undo[nbr].act;
    hello_button(button->widget, &event, button);
  }
  free(undo);
}

