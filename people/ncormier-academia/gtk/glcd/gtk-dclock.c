/*
** gtk-dclock.c for  in /home/nico/instances/glcd
** 
** Made by nicolas
** Login   <nico@epita.fr>
** 
** Started on  Wed Mar 23 23:33:18 2005 nicolas
** Last update Thu Mar 24 00:03:44 2005 nicolas
*/

#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <string.h>
#include <time.h>
#include "gtk-dclock.h"
#include "gtk-lcd.h"

static GtkLcdClass	*parent_class = NULL;

static void	gtk_dclock_class_init(GtkDclockClass *klass);
static void	gtk_dclock_init(GtkDclock *dclock);
static void	gtk_dclock_destroy(GtkObject *object);
static void	gtk_dclock_draw(gpointer data);

enum {
  ARG_0,
  ARG_NBR,
  ARG_STR
};

guint		gtk_dclock_get_type(void)
{
  static guint	gtk_dclock_type = 0;

  if (!gtk_dclock_type)
    {
      GtkTypeInfo gtk_dclock_info =
	{
	  "GtkDclock",
	  sizeof(GtkDclock),
	  sizeof(GtkDclockClass),
	  (GtkClassInitFunc) gtk_dclock_class_init,
	  (GtkObjectInitFunc) gtk_dclock_init,
	  NULL,
	  NULL,
	  NULL
	};
      gtk_dclock_type = gtk_type_unique(gtk_lcd_get_type(), &gtk_dclock_info);
    }
  return (gtk_dclock_type);
}

static void		gtk_dclock_class_init(GtkDclockClass *klass)
{
  GtkObjectClass	*object_class;

  object_class = (GtkObjectClass *) klass;
  object_class->destroy = gtk_dclock_destroy;
  parent_class = gtk_type_class(gtk_lcd_get_type());
}

static void	gtk_dclock_init(GtkDclock *dclock)
{
  g_timeout_add(1000, (GtkFunction) gtk_dclock_draw, dclock);
}

static void	gtk_dclock_destroy(GtkObject *object)
{
  GtkDclock *dclock;

  if (object == NULL)
    return;
  dclock = GTK_DCLOCK(object);
  GTK_OBJECT_CLASS(parent_class)->destroy(object);
}

GtkWidget	*gtk_dclock_new(void)
{
  GtkDclock	*dclock;

  dclock = gtk_type_new(gtk_dclock_get_type());
  gtk_dclock_construct((gpointer) dclock);
  return (GTK_WIDGET(dclock));
 }

static void	gtk_dclock_draw(gpointer data)
{
  char		buf[1024];
  time_t        tval;

  time(&tval);
  strftime(buf, sizeof(buf), "%H:%M:%S", localtime(&tval));
  gtk_object_set(GTK_OBJECT(data), "GtkLcd::str", buf, NULL);
  return;
}

gint	gtk_dclock_construct(gpointer data)
{
  gtk_dclock_draw(data);
  return (0);
}

