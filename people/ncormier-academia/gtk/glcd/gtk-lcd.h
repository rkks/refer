/*
** gtk-clock.h for  in /u/ept2/cormie_n/lang/c/glcd
**
** Made by nicolas cormier
** Login   <cormie_n@epita.fr>
**
** Started on  Mon Mar 21 14:31:07 2005 nicolas cormier
** Last update Wed Mar 23 23:57:54 2005 nicolas
*/

#ifndef __GTK_LCD_H__
#define __GTK_LCD_H__

#ifdef __cplusplus
  extern "C" {
#endif

#include <gtk/gtk.h>

#define GTK_TYPE_LCD            (gtk_lcd_get_type())
#define GTK_LCD(obj)            (GTK_CHECK_CAST((obj), GTK_TYPE_LCD, GtkLcd))
#define GTK_LCD_CLASS(klass)    (GTK_CHECK_CAST_CLASS((klass), GTK_TYPE_LCD, GtkLcdClass))
#define GTK_IS_LCD(obj)         (GTK_CHECK_TYPE((obj), GTK_TYPE_LCD))
#define GTK_IS_LCD_CLASS(klass) (GTK_CHECK_CLASS_TYPE ((klass), GTK_TYPE_LCD))
#define GTK_LCD_GET_CLASS(obj)  (GTK_CHECK_GET_CLASS ((obj), GTK_TYPE_LCD, GtkLcdClass))

#define S_01	1
#define S_02	2
#define S_03	4
#define S_04	8
#define S_05	16
#define S_06	32
#define S_07	64
#define S_08	128
#define S_09	256
#define S_10	512
#define S_11	1024
#define S_12	2048
#define S_13	4096
#define S_14	16384
#define S_15	32768
#define S_16	65536
#define S_17	131072
#define S_18	262144
#define S_19	524288
#define S_20	1048576
#define S_21	2097152
#define S_22	4194304
#define S_23	8388608
#define S_24	16777216
#define S_25	33554432
#define S_26	67108864
#define S_27	134217728
#define S_28	268435456

#define LCD_COLOR	0
#define LCD_STR	1

typedef struct _GtkLcd GtkLcd;
typedef struct _GtkLcdClass GtkLcdClass;

struct		_GtkLcd
{
  GtkWidget	parent;
  char		*str;
  char		*color;
  char		*bgcolor;
};

struct _GtkLcdClass
{
  GtkWidgetClass parent_class;
};

typedef struct
{
  char		c;
  unsigned int	mask;
}	letter_mask;

GtkWidget	*gtk_lcd_new(void);
guint		gtk_lcd_get_type(void) G_GNUC_CONST;
gint		gtk_lcd_construct(gpointer data);

#ifdef __cplusplus
  }
#endif
#endif /* __GTK_LCD_H__ */
