/*
** gtk-clock.c for  in /goinfre
**
** Made by nicolas cormier
** Login   <cormie_n@epita.fr>
**
** Started on  Thu Mar 17 11:10:53 2005 nicolas cormier
** Last update Thu Mar 24 00:11:44 2005 nicolas
*/

#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <string.h>
#include "gtk-lcd.h"

static GtkObjectClass	*parent_class = NULL;

static void	gtk_lcd_class_init(GtkLcdClass *klass);
static void	gtk_lcd_init(GtkLcd *lcd);
static void	gtk_lcd_realize(GtkWidget *widget);
static void	gtk_lcd_draw(GtkWidget *widget, GdkRectangle *area);
static void	gtk_lcd_size_request(GtkWidget *widget, GtkRequisition *req);
static void	gtk_lcd_destroy(GtkObject *object);
static void	gtk_lcd_set(GtkObject *object, GtkArg *arg, guint arg_id);
static void	gtk_lcd_get(GtkObject *object, GtkArg *arg, guint arg_id);
static gint	gtk_lcd_expose(GtkWidget *widget, GdkEventExpose *event);
static void	change_color_letter(int seek, int mask, GtkWidget *widget, GdkGC *lcd_gc, GdkColor *colors);

enum {
  ARG_0,
  ARG_COLOR,
  ARG_BGCOLOR,
  ARG_STR
};

static letter_mask	gl_letter_mask[] =
  {
    {'0', S_03 | S_01 | S_02 | S_13 | S_26 | S_28 | S_27 | S_16 | S_10 | S_12 | S_18 | S_20},
    {'1', S_05 | S_07 | S_08 | S_21 | S_27 | S_28},
    {'2', S_01 | S_02 | S_13 | S_15 | S_14 | S_16 | S_27 | S_28},
    {'3', S_01 | S_02 | S_13 | S_15 | S_14 | S_27 | S_28 | S_26},
    {'4', S_03 | S_14 | S_15 | S_08 | S_21},
    {'5', S_02 | S_01 | S_03 | S_14 | S_15 | S_26 | S_28 | S_27},
    {'6', S_02 | S_01 | S_03 | S_14 | S_15 | S_26 | S_28 | S_27 | S_16},
    {'7', S_01 | S_02 | S_10 | S_12 | S_18 | S_20},
    {'8', S_02 | S_01 | S_03 | S_16 | S_27 | S_28 | S_26 | S_13 | S_14 | S_15},
    {'9', S_02 | S_01 | S_03 | S_14 | S_15 | S_13 | S_26 | S_28 | S_27},
    {'a', S_03 | S_01 | S_02 | S_13 | S_26 | S_15 | S_14 | S_16 | S_26},
    {'b', S_03 | S_01 | S_09 | S_12 | S_22 | S_25 | S_27 | S_16 | S_14},
    {'c', S_03 | S_01 | S_02 | S_16 | S_27 | S_28},
    {'d', S_01 | S_03 | S_16 | S_27 | S_25 | S_23 | S_11 | S_09 | S_01},
    {'e', S_01 | S_02 | S_03 | S_14 | S_16 | S_27 | S_28},
    {'f', S_01 | S_02 | S_03 | S_16 | S_14},
    {'g', S_02 | S_01 | S_03 | S_15 | S_26 | S_28 | S_27 | S_16},
    {'h', S_03 | S_16 | S_14 | S_15 | S_13 | S_26},
    {'i', S_01 | S_02 | S_08 | S_21 | S_27 | S_28},
    {'j', S_01 | S_02 | S_08 | S_21 | S_27},
    {'k', S_03 | S_16 | S_14 | S_12 | S_10 | S_22 | S_24},
    {'l', S_03 | S_16 | S_27 | S_28},
    {'m', S_16 | S_03 | S_04 | S_06 | S_12 | S_10 | S_13 | S_26},
    {'n', S_16 | S_03 | S_04 | S_06 | S_22 | S_24 | S_26 | S_13},
    {'o', S_01 | S_02 | S_13 | S_26 | S_28 | S_27 | S_16 | S_03},
    {'p', S_01 | S_02 | S_13 | S_15 | S_14 | S_16 | S_03},
    {'q', S_01 | S_02 | S_13 | S_26 | S_28 | S_27 | S_16 | S_03 | S_22 | S_24},
    {'r', S_03 | S_01 | S_02 | S_13 | S_15 | S_22 | S_24 | S_16 | S_14},
    {'s', S_01 | S_02 | S_03 | S_14 | S_15 | S_26 | S_28 | S_27},
    {'t', S_01 | S_02 | S_08 | S_21},
    {'u', S_03 | S_16 | S_27 | S_28 | S_26 | S_13},
    {'v', S_03 | S_16 | S_20 | S_18 | S_12 | S_10},
    {'w', S_03 | S_16 | S_20 | S_18 | S_22 | S_24 | S_26 | S_13},
    {'x', S_04 | S_06 | S_22 | S_24 | S_20 | S_18 | S_12 | S_10},
    {'y', S_03 | S_14 | S_15 | S_13 | S_26 | S_28 | S_27},
    {'z', S_01 | S_02 | S_10 | S_12 | S_18 | S_20 | S_27 | S_28},
    {'`', S_04},
    {'_', S_27 | S_28},
    {'*', S_08 | S_14 | S_15 | S_21 | S_04 | S_06 | S_10 | S_12 | S_18 | S_20 | S_22 | S_24},
    {'\\', S_04 | S_06 | S_22 | S_24},
    {'/', S_10 | S_12 | S_18 | S_20},
    {'+', S_08 | S_14 | S_15 | S_21},
    {'-', S_14 | S_15},
    {'=', S_14 | S_15 | S_27 | S_28},
    {'\"', S_04 | S_06 | S_22 | S_24},
    {'<', S_05 | S_07 | S_17 | S_19},
    {'>', S_09 | S_11 | S_23 | S_25},
    {'^', S_05 | S_07 | S_09 | S_11},
    {'|', S_08 | S_21},
    {'[', S_01 | S_03 | S_16 | S_27},
    {']', S_02 | S_13 | S_26 | S_28},
    {':', S_06 | S_05 | S_09 | S_12 | S_18 | S_19 | S_22 | S_25},
    {'.', S_18 | S_19 | S_22 | S_25},
    {'\0', 0}
  };

guint		gtk_lcd_get_type(void)
{
  static guint	gtk_lcd_type = 0;

  if (!gtk_lcd_type)
    {
      GtkTypeInfo gtk_lcd_info =
	{
	  "GtkLcd",
	  sizeof(GtkLcd),
	  sizeof(GtkLcdClass),
	  (GtkClassInitFunc) gtk_lcd_class_init,
	  (GtkObjectInitFunc) gtk_lcd_init,
	  NULL,
	  NULL,
	  NULL
	};
      gtk_lcd_type = gtk_type_unique(gtk_widget_get_type(), &gtk_lcd_info);
    }
  return (gtk_lcd_type);
}

static void		gtk_lcd_class_init(GtkLcdClass *klass)
{
  GtkObjectClass	*object_class;
  GtkWidgetClass	*widget_class;

  object_class = (GtkObjectClass *) klass;
  widget_class = (GtkWidgetClass*) klass;
  object_class->destroy = gtk_lcd_destroy;
  parent_class = gtk_type_class(gtk_widget_get_type());
  widget_class->realize = gtk_lcd_realize;
  widget_class->size_request = gtk_lcd_size_request;
  widget_class->expose_event = gtk_lcd_expose;
  object_class->set_arg = gtk_lcd_set;
  object_class->get_arg = gtk_lcd_get;
  gtk_object_add_arg_type("GtkLcd::color", GTK_TYPE_STRING, GTK_ARG_READWRITE, ARG_COLOR);
  gtk_object_add_arg_type("GtkLcd::bgcolor", GTK_TYPE_STRING, GTK_ARG_READWRITE, ARG_BGCOLOR);
  gtk_object_add_arg_type("GtkLcd::str", GTK_TYPE_STRING, GTK_ARG_READWRITE, ARG_STR);
}

static void	gtk_lcd_init(GtkLcd *lcd)
{
  GTK_WIDGET_SET_FLAGS(lcd, GTK_CAN_FOCUS | GTK_RECEIVES_DEFAULT);
  GTK_WIDGET_UNSET_FLAGS(lcd, GTK_NO_WINDOW);
  lcd->str = strdup("0123");
  lcd->color = strdup("Yellow");
  lcd->bgcolor = strdup("Black");
}

static void	gtk_lcd_destroy(GtkObject *object)
{
  parent_class->destroy(object);
  free(GTK_LCD(object)->str);
  free(GTK_LCD(object)->color);
  free(GTK_LCD(object)->bgcolor);
}

GtkWidget	*gtk_lcd_new(void)
{
  GtkLcd	*lcd;

  lcd = gtk_type_new(gtk_lcd_get_type());
  return (GTK_WIDGET(lcd));
}

static void	gtk_lcd_size_request(GtkWidget *widget, GtkRequisition *allocation)
{
  allocation->width =  53 * strlen(GTK_LCD(widget)->str);
  allocation->height = 80;
}

static void	gtk_lcd_realize(GtkWidget *widget)
{
  GtkLcd	*lcd;
  GdkWindowAttr	attributes;
  gint		attributes_mask;

  if (widget == NULL)
    return;
  if (!GTK_IS_LCD (widget))
    return;
  lcd = GTK_LCD(widget);
  GTK_WIDGET_SET_FLAGS(widget, GTK_REALIZED);
  attributes.window_type = GDK_WINDOW_CHILD;
  attributes.x = widget->allocation.x;
  attributes.y = widget->allocation.y;
  attributes.width = widget->allocation.width;
  attributes.height = widget->allocation.height;
  attributes.wclass = GDK_INPUT_OUTPUT;
  attributes.visual = gtk_widget_get_visual(widget);
  attributes.colormap = gtk_widget_get_colormap(widget);
  attributes.event_mask = gtk_widget_get_events(widget) | GDK_EXPOSURE_MASK;
  attributes_mask = GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL | GDK_WA_COLORMAP;
  widget->window = gdk_window_new(gtk_widget_get_parent_window(widget),
				  &attributes, attributes_mask);
  gdk_window_set_user_data(widget->window, lcd);
  widget->style = gtk_style_attach(widget->style, widget->window);
  gtk_style_set_background(widget->style, widget->window, GTK_STATE_NORMAL);
}

static gint	gtk_lcd_expose(GtkWidget *widget, GdkEventExpose *event)
{
  gtk_lcd_construct((gpointer) widget);
  return (0);
}

gint		gtk_lcd_construct(gpointer data)
{
  GtkLcd	*lcd;

  lcd = data;
  if (!GTK_WIDGET_DRAWABLE(lcd))
    return (0);
  gtk_lcd_draw(GTK_WIDGET(lcd), NULL);
  return (1);
}

static void	gtk_lcd_draw_diag(GtkWidget *widget, GdkGC *lcd_gc, GdkColor *colors, int x, int y, int mask, int count)
{
  GdkPoint	poly[4];

  poly[0].x = x + 3;
  poly[0].y = y + 12;
  poly[1].x = x + 6;
  poly[1].y = y + 12;
  poly[2].x = x + 15;
  poly[2].y = y + 21;
  poly[3].x = x + 13;
  poly[3].y = y + 21;
  change_color_letter(mask, gl_letter_mask[count].mask, widget, lcd_gc, colors);
  gdk_draw_polygon(widget->window, lcd_gc, 1, poly, 4);
  poly[0].x = x + 3 + 18 + 1;
  poly[0].y = y + 12;
  poly[1].x = x + 6 + 18 + 1;
  poly[1].y = y + 12;
  poly[2].x = x + 15 + 1;
  poly[2].y = y + 21;
  poly[3].x = x + 13 + 1;
  poly[3].y = y + 21;
  change_color_letter((mask *= 2), gl_letter_mask[count].mask, widget, lcd_gc, colors);
  gdk_draw_polygon(widget->window, lcd_gc, 1, poly, 4);
  poly[0].x = x + 15;
  poly[0].y = y + 23;
  poly[1].x = x + 17;
  poly[1].y = y + 23;
  poly[2].x = x + 25;
  poly[2].y = y + 32;
  poly[3].x = x + 24;
  poly[3].y = y + 32;
  change_color_letter((mask *= 2), gl_letter_mask[count].mask, widget, lcd_gc, colors);
  gdk_draw_polygon(widget->window, lcd_gc, 1, poly, 4);
  poly[0].x = x + 15 - 3;
  poly[0].y = y + 23;
  poly[1].x = x + 17 - 3;
  poly[1].y = y + 23;
  poly[2].x = x + 25 - 18 - 3;
  poly[2].y = y + 32;
  poly[3].x = x + 24 - 18 - 3;
  poly[3].y = y + 32;
  change_color_letter((mask *= 2), gl_letter_mask[count].mask, widget, lcd_gc, colors);
  gdk_draw_polygon(widget->window, lcd_gc, 1, poly, 4);
}

static void	change_color_letter(int seek, int mask, GtkWidget *widget, GdkGC *lcd_gc, GdkColor *colors)
{
  if ((mask & seek) == seek)
    gdk_gc_set_foreground(lcd_gc, colors + 2);
  else
    gdk_gc_set_foreground(lcd_gc, colors + 1);
}

static void	gtk_lcd_draw_letter(GtkWidget *widget, int x, GdkColor *colors, GdkGC *lcd_gc, char c)
{
  int		count;
  int		epais = 1;

  for (count = 0; gl_letter_mask[count].c; count++)
    if (gl_letter_mask[count].c == c)
      break;
  change_color_letter(S_01, gl_letter_mask[count].mask, widget, lcd_gc, colors);
  gdk_draw_rectangle(widget->window, lcd_gc, 1, x + 5, 10, 20, 1 + epais);
  change_color_letter(S_02, gl_letter_mask[count].mask, widget, lcd_gc, colors);
  gdk_draw_rectangle(widget->window, lcd_gc, 1, x + 5 + 22, 10, 20, 1 + epais);
  change_color_letter(S_14, gl_letter_mask[count].mask, widget, lcd_gc, colors);
  gdk_draw_rectangle(widget->window, lcd_gc, 1, x + 5, 30 + 4 + epais, 20, 1 + epais);
  change_color_letter(S_15, gl_letter_mask[count].mask, widget, lcd_gc, colors);
  gdk_draw_rectangle(widget->window, lcd_gc, 1, x + 5 + 22, 30 + 4 + epais, 20, 1 + epais);
  change_color_letter(S_27, gl_letter_mask[count].mask, widget, lcd_gc, colors);
  gdk_draw_rectangle(widget->window, lcd_gc, 1, x + 5, 50 + 6 + (2 * epais), 20, 1 + epais);
  change_color_letter(S_28, gl_letter_mask[count].mask, widget, lcd_gc, colors);
  gdk_draw_rectangle(widget->window, lcd_gc, 1, x + 5 + 22, 50 + 6 + (2 * epais), 20, 1 + epais);
  change_color_letter(S_03, gl_letter_mask[count].mask, widget, lcd_gc, colors);
  gdk_draw_rectangle(widget->window, lcd_gc, 1, x + 3, 10 + 2 + epais, 1 + epais, 20);
  change_color_letter(S_08, gl_letter_mask[count].mask, widget, lcd_gc, colors);
  gdk_draw_rectangle(widget->window, lcd_gc, 1, x + 3 + 22, 10 + 2 + epais, 1 + epais, 20);
  change_color_letter(S_13, gl_letter_mask[count].mask, widget, lcd_gc, colors);
  gdk_draw_rectangle(widget->window, lcd_gc, 1, x + 3 + 22 + 22, 10 + 2 + epais, 1 + epais, 20);
  change_color_letter(S_16, gl_letter_mask[count].mask, widget, lcd_gc, colors);
  gdk_draw_rectangle(widget->window, lcd_gc, 1, x + 3, 10 + 2 + 20 + 4 + (2 * epais), 1 + epais, 20);
  change_color_letter(S_21, gl_letter_mask[count].mask, widget, lcd_gc, colors);
  gdk_draw_rectangle(widget->window, lcd_gc, 1, x + 3 + 22, 10 + 2 + 20 + 4 + (2 * epais), 1 + epais, 20);
  change_color_letter(S_26, gl_letter_mask[count].mask, widget, lcd_gc, colors);
  gdk_draw_rectangle(widget->window, lcd_gc, 1, x + 3 + 22 + 22, 10 + 2 + 20 + 4 + (2 * epais), 1 + epais, 20);
  gdk_gc_set_foreground(lcd_gc, colors + 1);
  gtk_lcd_draw_diag(widget, lcd_gc, colors, x + 0 + epais, 0 + (1 * epais), S_04, count);
  gtk_lcd_draw_diag(widget, lcd_gc, colors, x + 22 + epais, 0 + (1 * epais), S_09, count);
  gtk_lcd_draw_diag(widget, lcd_gc, colors, x + 0 + epais, 22 + (2 * epais), S_17, count);
  gtk_lcd_draw_diag(widget, lcd_gc, colors, x + 22 + epais, 22 + (2 * epais), S_22, count);
}

static void	gtk_lcd_draw(GtkWidget *widget, GdkRectangle *area)
{
  GtkLcd	*lcd;
  GdkColor	colors[3];
  GdkGC		*lcd_gc;
  int		alloc_width;
  int		alloc_height;
  int		count;

  if (!GTK_WIDGET_DRAWABLE(widget))
    return;
  lcd = GTK_LCD(widget);
  lcd_gc = gdk_gc_new(widget->window);
  alloc_width = widget->allocation.width - 1;
  alloc_height = widget->allocation.height - 1;
  gdk_color_parse(lcd->bgcolor, &colors[0]);
  gdk_color_parse("#333333", &colors[1]);
  gdk_color_parse(lcd->color, &colors[2]);
  gdk_colormap_alloc_color(gdk_colormap_get_system(), &colors[0], FALSE, TRUE);
  gdk_colormap_alloc_color(gdk_colormap_get_system(), &colors[1], FALSE, TRUE);
  gdk_colormap_alloc_color(gdk_colormap_get_system(), &colors[2], FALSE, TRUE);
  gdk_gc_set_foreground(lcd_gc, colors);
  gdk_draw_rectangle(widget->window, lcd_gc, 1, 0, 0, alloc_width, alloc_height);
  for (count = 0; lcd->str[count]; count++)
    gtk_lcd_draw_letter(widget, (count * 53), colors, lcd_gc, lcd->str[count]);
  gdk_colormap_free_colors(gdk_colormap_get_system(), colors, 3);
  gdk_gc_destroy(lcd_gc);
}

static void	gtk_lcd_set(GtkObject *object, GtkArg *arg, guint arg_id)
{
  char		*str;

  if (arg_id == ARG_STR)
    {
      free(GTK_LCD(object)->str);
      GTK_LCD(object)->str = strdup(GTK_VALUE_STRING(*arg));
      for (str = GTK_LCD(object)->str; *str; str++)
	if (*str >= 'A' && *str <= 'Z')
	  *str = (*str - 'A') + 'a';
    }
  else if (arg_id == ARG_COLOR)
    {
      free(GTK_LCD(object)->color);
      GTK_LCD(object)->color = strdup(GTK_VALUE_STRING(*arg));
    }
  else if (arg_id == ARG_BGCOLOR)
    {
      free(GTK_LCD(object)->bgcolor);
      GTK_LCD(object)->bgcolor = strdup(GTK_VALUE_STRING(*arg));
    }
  gtk_lcd_draw(GTK_WIDGET(object), NULL);
}

static void	gtk_lcd_get(GtkObject *object, GtkArg *arg, guint arg_id)
{
  if (arg_id == ARG_STR)
      GTK_VALUE_STRING(*arg) = g_strdup(GTK_LCD(object)->str);
  else if (arg_id == ARG_COLOR)
    GTK_VALUE_STRING(*arg) = g_strdup(GTK_LCD(object)->color);
  else if (arg_id == ARG_BGCOLOR)
    GTK_VALUE_STRING(*arg) = g_strdup(GTK_LCD(object)->bgcolor);
}

