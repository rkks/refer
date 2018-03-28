/*
** gtk-dclock.h for  in /home/nico/instances/glcd
** 
** Made by nicolas
** Login   <nico@epita.fr>
** 
** Started on  Wed Mar 23 23:34:41 2005 nicolas
** Last update Wed Mar 23 23:39:47 2005 nicolas
*/

#ifndef __GTK_DCLOCK_H__
#define __GTK_DCLOCK_H__

#ifdef __cplusplus
  extern "C" {
#endif

#include <gtk/gtk.h>
#include "gtk-lcd.h"

#define GTK_TYPE_DCLOCK            (gtk_dclock_get_type())
#define GTK_DCLOCK(obj)            (GTK_CHECK_CAST((obj), GTK_TYPE_DCLOCK, GtkDclock))
#define GTK_DCLOCK_CLASS(klass)    (GTK_CHECK_CAST_CLASS((klass), GTK_TYPE_DCLOCK, GtkDclockClass))
#define GTK_IS_DCLOCK(obj)         (GTK_CHECK_TYPE((obj), GTK_TYPE_DCLOCK))
#define GTK_IS_DCLOCK_CLASS(klass) (GTK_CHECK_CLASS_TYPE ((klass), GTK_TYPE_DCLOCK))
#define GTK_DCLOCK_GET_CLASS(obj)  (GTK_CHECK_GET_CLASS ((obj), GTK_TYPE_DCLOCK, GtkDclockClass))

typedef struct _GtkDclock GtkDclock;
typedef struct _GtkDclockClass GtkDclockClass;

struct		_GtkDclock
{
  GtkLcd	widget;
  int		modif;
};

struct _GtkDclockClass
{
  GtkLcdClass parent_class;
};

GtkWidget	*gtk_dclock_new(void);
guint		gtk_dclock_get_type(void) G_GNUC_CONST;
gint		gtk_dclock_construct(gpointer data);

#ifdef __cplusplus
  }
#endif
#endif /* __GTK_DCLOCK_H__ */
