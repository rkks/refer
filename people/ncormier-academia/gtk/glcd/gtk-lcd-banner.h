/*
** gtk-lcd-banner.h for  in /home/nico/instances/glcd
**
** Made by nicolas
** Login   <nico@epita.fr>
**
** Started on  Mon Mar 21 21:20:00 2005 nicolas
** Last update Tue Mar 22 15:20:08 2005 nicolas cormier
*/

#ifndef __GTK_LCD_BANNER_H__
#define __GTK_LCD_BANNER_H__

#ifdef __cplusplus
  extern "C" {
#endif

#include <gtk/gtk.h>
#include "gtk-lcd.h"

#define GTK_TYPE_LCD_BANNER            (gtk_lcd_banner_get_type())
#define GTK_LCD_BANNER(obj)            (GTK_CHECK_CAST((obj), GTK_TYPE_LCD_BANNER, GtkLcd_Banner))
#define GTK_LCD_BANNER_CLASS(klass)    (GTK_CHECK_CAST_CLASS((klass), GTK_TYPE_LCD_BANNER, GtkLcd_BannerClass))
#define GTK_IS_LCD_BANNER(obj)         (GTK_CHECK_TYPE((obj), GTK_TYPE_LCD_BANNER))
#define GTK_IS_LCD_BANNER_CLASS(klass) (GTK_CHECK_CLASS_TYPE ((klass), GTK_TYPE_LCD_BANNER))
#define GTK_LCD_BANNER_GET_CLASS(obj)  (GTK_CHECK_GET_CLASS ((obj), GTK_TYPE_LCD_BANNER, GtkLcd_BannerClass))

typedef struct _GtkLcd_Banner GtkLcd_Banner;
typedef struct _GtkLcd_BannerClass GtkLcd_BannerClass;

struct		_GtkLcd_Banner
{
  GtkLcd	widget;
  char		*str;
  int		idx;
  int		nbr;
  int		modif;
};

struct _GtkLcd_BannerClass
{
  GtkLcdClass parent_class;
};

GtkWidget	*gtk_lcd_banner_new(void);
guint		gtk_lcd_banner_get_type(void) G_GNUC_CONST;
gint		gtk_lcd_banner_construct(gpointer data);

#ifdef __cplusplus
  }
#endif
#endif /* __GTK_LCD_BANNER_H__ */
