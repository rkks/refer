/*
** gmine_struct.h for  in /u/ept2/cormie_n/lang/c/gmine
**
** Made by nicolas cormier
** Login   <cormie_n@epita.fr>
**
** Started on  Mon Feb  7 16:00:34 2005 nicolas cormier
** Last update Wed Mar  9 15:35:12 2005 nicolas cormier
*/

#ifndef _GMINE_STRUCT_H_
#define _GMINE_STRUCT_H_

typedef struct	s_pixmap
{
  GdkPixmap	*pixmap;
  GdkBitmap	*mask;
}		t_pixmap;

typedef struct	s_undo
{
  int		x;
  int		y;
  int		act;
}		t_undo;

typedef struct	s_option
{
  GtkWidget	*case_x;
  GtkWidget	*case_y;
  GtkWidget	*nb_mine;
  GtkWidget	*nb_flags;
  GtkWidget	*window;
}		t_option;

typedef struct	s_button
{
  int		x;
  int		y;
  int		etat;
  int		mined;
  int		pressed;
  int		next_mine;
  GtkWidget	*widget;
  GtkWidget	*pixwid;
  GtkWidget	*label;
}		t_button;

typedef struct	s_gmine
{
  int		x;
  int		y;
  int		nb_mine;
  int		nb_flags;
  int		cur_nb_flags;
  int		dead;
  GtkWidget	*window;
  GtkWidget	*table;
  GtkWidget	*text;
  t_button	smiley;
  t_button	*buttons;
  t_pixmap	*flag;
  t_pixmap	*flag_bad;
  t_pixmap	*question;
  t_pixmap	*bomb;
  t_pixmap	*smiley_def;
  t_pixmap	*smiley_lose;
  t_pixmap	*smiley_win;
  t_pixmap	*smiley_worried;
  t_undo	*undo;
  char		timer[1024];
}		t_gmine;

#endif /* _GMINE_STRUCT_H_ */
