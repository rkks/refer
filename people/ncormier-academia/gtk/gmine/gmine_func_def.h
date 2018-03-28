/*
** gmine_func_def.h for  in /u/ept2/cormie_n/lang/c/gmine
**
** Made by nicolas cormier
** Login   <cormie_n@epita.fr>
**
** Started on  Mon Feb  7 15:57:02 2005 nicolas cormier
** Last update Wed Mar  9 15:49:40 2005 nicolas cormier
*/

#ifndef _GMINE_FUNC_DEF_H_
#define _GMINE_FUNC_DEF_H_

#include "gmine_struct.h"

/* PIXMAP_C */
void	init_pixmap(t_gmine *gmine);
void	apply_pixmap(t_button *button);

/* INIT_GTK_C */
void	init_gtk(int argc, char **argv, t_gmine *gmine);
void	init_main_window(t_gmine *gmine);

/* BUTTONS_C */
void	init_buttons(t_gmine *gmine);
void	init_button_struct(t_gmine *gmine, int x, int y, int x_max);
void	hello_button(GtkWidget *widget, GdkEventButton *event, gpointer data);
/* INIT_TABLE_C */
void	init_table(t_gmine *gmine);

/* MINES_C */
int	unhide_mines(t_gmine *gmine, t_button *button);

/* MINE_INIT_C */
void	init_mines(t_gmine *gmine);

/* MENU_C */
void	init_menu(t_gmine *gmine);

/* PIXMAP_C */
void	attach_label(t_button *button);
void	apply_pixmap_bomb(t_gmine *gmine, t_button *button);
void	apply_this_pixmap(t_button *button, t_pixmap *cur);
void	init_pixmap(t_gmine *gmine);

/* UNHIDE_GAME_C */
void	boom(t_gmine *gmine, t_pixmap *smiley, int type);

/* TEXT_C */
void	init_text(t_gmine *gmine);
void	refresh_text(t_gmine *gmine);

/* SCREEN_C */
void	refresh_screen(t_gmine *gmine);

/* GAME_C */
void	chk_win(t_gmine *gmine);

/* NEW_GAME_C */
void	new_game(void);

/* SAVE_GAME_C */
void	*save_cur_gmine(t_gmine *gmine);
void	save_game(void);
void	load_game(void);

/* UNDO_C */
void	undo_game(void);

/* OPTION_C */
void	option_window(void);

/* OPTION_VALID_C */
gint	valid_event(GtkWidget *widget, GdkEvent *event, gpointer data);

/* ERROR_C */
void	gtk_error(char *strerr);

/* UNDO_C */
void	add_undo_act(t_gmine *gmine, int act, int x, int y);
void	undo_game(void);

/* FLAG_C */
void	flag_manager(t_gmine *gmine, t_button *button);

/* NEW_GAME_C */
void	rinit_game(void);
void	new_game(void);

/* TIMER_C */
int	m_timer(t_gmine *gmine);
void	rinit_timer(t_gmine *gmine);

#endif /* _GMINE_FUNC_DEF_H_ */
