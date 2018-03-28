/*
** toto.c for  in /goinfre
**
** Made by nicolas cormier
** Login   <cormie_n@epita.fr>
**
** Started on  Thu Apr 28 13:07:10 2005 nicolas cormier
** Last update Thu Apr 28 14:09:16 2005 nicolas cormier
*/

#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include "wall1.xpm"

GtkWidget	*window;
GdkPixmap	*pixmap;
GdkPixmap	*pixmap2;

static gint     expose_event(GtkWidget *widget, GdkEventExpose *event, void *disp)
{
  gdk_draw_pixmap(widget->window,
                  widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
                  pixmap,
                  event->area.x, event->area.y,
                  event->area.x, event->area.y,
                  event->area.width, event->area.height);
  return (FALSE);
}

static gint     configure_event(GtkWidget *widget, GdkEventConfigure *event, void *disp)
{
  GdkBitmap     *mask;
  GtkStyle      *style;

  if (pixmap)
    gdk_pixmap_unref(pixmap);
  pixmap = gdk_pixmap_new(widget->window, 40, 40, -1);

  style = gtk_widget_get_style(window);
  pixmap2 = gdk_pixmap_create_from_xpm_d(widget->window, &mask,
					 &style->bg[GTK_STATE_NORMAL],
					 (gchar **) wall1_xpm);
  gdk_draw_pixmap(pixmap,
                  widget->style->fg_gc[GTK_WIDGET_STATE(widget)],
                  pixmap2, 0, 0, 0, 0, 14, 14);
  return (TRUE);
}

int		main()
{
  GtkWidget	*draw;

  gtk_init(NULL, NULL);
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_widget_show(window);

  draw = gtk_drawing_area_new();
  gtk_drawing_area_size(GTK_DRAWING_AREA(draw), 40, 40);
  gtk_widget_show(draw);
  gtk_container_add(GTK_CONTAINER(window), draw);
  gtk_signal_connect(GTK_OBJECT(draw),
                     "expose_event", (GtkSignalFunc) expose_event, NULL);
  gtk_signal_connect(GTK_OBJECT(draw),
                     "configure_event", (GtkSignalFunc) configure_event, NULL);
  gtk_main();
}
