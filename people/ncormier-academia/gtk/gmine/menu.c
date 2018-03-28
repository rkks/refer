/*
** menu.c for  in /u/ept2/cormie_n/lang/c/gmine
**
** Made by nicolas cormier
** Login   <cormie_n@epita.fr>
**
** Started on  Thu Mar  3 11:05:30 2005 nicolas cormier
** Last update Wed Mar  9 14:22:31 2005 nicolas cormier
*/

#include <gtk/gtk.h>

#include "gmine_struct.h"
#include "gmine_func_def.h"

extern t_gmine	*gl_gmine;

static void	init_items(t_gmine *gmine, GtkWidget *menu, char *label, void (func)())
{
  GtkWidget	*menu_items;

  menu_items = gtk_menu_item_new_with_label(label);
  gtk_menu_append(GTK_MENU(menu), menu_items);
  gtk_signal_connect_object(GTK_OBJECT(menu_items), "activate",
			    GTK_SIGNAL_FUNC(func),
			    (gpointer) g_strdup(label));
}

static void	smiley_event(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
  new_game();
}

static void	smiley_button(t_gmine *gmine)
{
  gmine->smiley.widget = gtk_button_new();
  gmine->smiley.pixwid = NULL;
  gtk_signal_connect(GTK_OBJECT(gmine->smiley.widget),
		     "button_release_event",
		     GTK_SIGNAL_FUNC(smiley_event),
		     (gpointer) gmine);
  gtk_table_attach_defaults(GTK_TABLE(gmine->table),
			    gmine->smiley.widget,
			    gmine->x / 2 - 1, (gmine->x / 2) + 1, 1, 3);
  apply_this_pixmap(&(gmine->smiley), gmine->smiley_worried);
}

void		init_menu(t_gmine *gmine)
{
  GtkWidget	*menu;
  GtkWidget	*root_menu;
  GtkWidget	*menu_bar;

  menu = gtk_menu_new();
  init_items(gmine, menu, "new game", new_game);
  init_items(gmine, menu, "save", save_game);
  init_items(gmine, menu, "load", load_game);
  init_items(gmine, menu, "undo", undo_game);
  init_items(gmine, menu, "options", option_window);
  init_items(gmine, menu, "quitter", gtk_main_quit);
  root_menu = gtk_menu_item_new_with_label("Menu");
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(root_menu), menu);
  menu_bar = gtk_menu_bar_new();
  gtk_table_attach_defaults(GTK_TABLE(gmine->table),
			    menu_bar, 0, 2, 0, 1);
  gtk_menu_bar_append(GTK_MENU_BAR(menu_bar), root_menu);
  smiley_button(gmine);
}

