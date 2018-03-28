/*
** gtk-lcd-banner.c for  in /home/nico/instances/glcd
**
** Made by nicolas
** Login   <nico@epita.fr>
**
** Started on  Mon Mar 21 21:13:12 2005 nicolas
** Last update Thu Mar 24 00:02:55 2005 nicolas
*/

#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <string.h>
#include "gtk-lcd-banner.h"
#include "gtk-lcd.h"

static GtkLcdClass	*parent_class = NULL;

static void	gtk_lcd_banner_class_init(GtkLcd_BannerClass *klass);
static void	gtk_lcd_banner_init(GtkLcd_Banner *lcd_banner);
static void	gtk_lcd_banner_destroy(GtkObject *object);
static void	gtk_lcd_banner_set(GtkObject *object, GtkArg *arg, guint arg_id);
static void	gtk_lcd_banner_get(GtkObject *object, GtkArg *arg, guint arg_id);
static void	gtk_lcd_banner_draw(gpointer data);

enum {
  ARG_0,
  ARG_NBR,
  ARG_STR
};

guint		gtk_lcd_banner_get_type(void)
{
  static guint	gtk_lcd_banner_type = 0;

  if (!gtk_lcd_banner_type)
    {
      GtkTypeInfo gtk_lcd_banner_info =
	{
	  "GtkLcd_Banner",
	  sizeof(GtkLcd_Banner),
	  sizeof(GtkLcd_BannerClass),
	  (GtkClassInitFunc) gtk_lcd_banner_class_init,
	  (GtkObjectInitFunc) gtk_lcd_banner_init,
	  NULL,
	  NULL,
	  NULL
	};
      gtk_lcd_banner_type = gtk_type_unique(gtk_lcd_get_type(), &gtk_lcd_banner_info);
    }
  return (gtk_lcd_banner_type);
}

static void		gtk_lcd_banner_class_init(GtkLcd_BannerClass *klass)
{
  GtkObjectClass	*object_class;

  object_class = (GtkObjectClass *) klass;
  object_class->destroy = gtk_lcd_banner_destroy;
  object_class->set_arg = gtk_lcd_banner_set;
  object_class->get_arg = gtk_lcd_banner_get;
  parent_class = gtk_type_class(gtk_lcd_get_type());
  gtk_object_add_arg_type("GtkLcd_Banner::nbr", GTK_TYPE_INT, GTK_ARG_READWRITE, ARG_NBR);
  gtk_object_add_arg_type("GtkLcd_Banner::str", GTK_TYPE_STRING, GTK_ARG_READWRITE, ARG_STR);
}

static void	gtk_lcd_banner_init(GtkLcd_Banner *lcd_banner)
{
  lcd_banner->str = strdup("abcdefghijklmnopqrstuvwxyz 0123456789 ");
  lcd_banner->idx = 0;
  lcd_banner->nbr = 5;
  lcd_banner->modif = 0;
  g_timeout_add(500, (GtkFunction) gtk_lcd_banner_draw, lcd_banner);
}

static void	gtk_lcd_banner_destroy(GtkObject *object)
{
  GtkLcd_Banner *lcd_banner;

  if (object == NULL)
    return;
  lcd_banner = GTK_LCD_BANNER(object);
  GTK_OBJECT_CLASS(parent_class)->destroy(object);
  free(GTK_LCD_BANNER(object)->str);
}

GtkWidget	*gtk_lcd_banner_new(void)
{
  GtkLcd_Banner	*lcd_banner;

  lcd_banner = gtk_type_new(gtk_lcd_banner_get_type());
  lcd_banner->modif = 1;
  return (GTK_WIDGET(lcd_banner));
 }

static void	gtk_lcd_banner_draw(gpointer data)
{
  char		buf[GTK_LCD_BANNER(data)->nbr + 1];
  int		count;
  int		idx;

  if (GTK_LCD_BANNER(data)->modif == 0)
    return;
  idx = GTK_LCD_BANNER(data)->idx;
  for (count = 0; count < GTK_LCD_BANNER(data)->nbr; count++)
    {
      buf[count] = GTK_LCD_BANNER(data)->str[idx];
      idx++;
      if (!GTK_LCD_BANNER(data)->str[idx])
	idx = 0;
    }
  GTK_LCD_BANNER(data)->idx++;
  if (!GTK_LCD_BANNER(data)->str[GTK_LCD_BANNER(data)->idx])
    GTK_LCD_BANNER(data)->idx = 0;
  buf[GTK_LCD_BANNER(data)->nbr] = '\0';
  gtk_object_set(GTK_OBJECT(data), "GtkLcd::str", buf, NULL);
  return;
}

gint	gtk_lcd_banner_construct(gpointer data)
{
  gtk_lcd_banner_draw(data);
  return (0);
}

static void	gtk_lcd_banner_set(GtkObject *object, GtkArg *arg, guint arg_id)
{
  if (arg_id == ARG_NBR)
    (GTK_LCD_BANNER(object))->nbr = GTK_VALUE_INT(*arg);
  else if (arg_id == ARG_STR)
    {
      free((GTK_LCD_BANNER(object))->str);
      (GTK_LCD_BANNER(object))->str = strdup(GTK_VALUE_STRING(*arg));
      (GTK_LCD_BANNER(object))->idx = 0;
    }
  gtk_lcd_banner_construct((gpointer) object);
  gtk_widget_set_usize(GTK_WIDGET(object), 53 * (GTK_LCD_BANNER(object))->nbr, 80);
}

static void	gtk_lcd_banner_get(GtkObject *object, GtkArg *arg, guint arg_id)
{
  if (arg_id == ARG_NBR)
      GTK_VALUE_INT(*arg) = GTK_LCD_BANNER(object)->nbr;
  else if (arg_id == ARG_STR)
    GTK_VALUE_STRING(*arg) = g_strdup(GTK_LCD_BANNER(object)->str);
}

