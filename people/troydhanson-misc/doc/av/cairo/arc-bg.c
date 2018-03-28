#include <cairo.h>
#include <stdio.h>
#include <math.h> // M_PI

#define OUTFILE "arc-bg.png"

int main() {

  double xc = 128.0;
  double yc = 128.0;
  double radius = 100.0;
  double angle1 = 45.0  * (M_PI/180.0);  /* angles are specified */
  double angle2 = 180.0 * (M_PI/180.0);  /* in radians           */

  cairo_surface_t *surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 400, 400);
  cairo_t *cr = cairo_create (surface);

  /* paint background */
  double r=0.1, g=0.0, b=0.2;
  cairo_set_source_rgb (cr, r, g, b);
  cairo_paint (cr);

  /* make primary arc */
  cairo_set_line_width (cr, 10.0);
  r=1.0, g=1.0, b=0.2;
  cairo_set_source_rgb (cr, r, g, b);
  cairo_arc (cr, xc, yc, radius, angle1, angle2);
  cairo_stroke (cr);

  /* draw helping lines */
  cairo_set_source_rgba (cr, 1, 0.2, 0.2, 0.6);
  cairo_set_line_width (cr, 6.0);

  cairo_arc (cr, xc, yc, 10.0, 0, 2*M_PI);
  cairo_fill (cr);

  cairo_arc (cr, xc, yc, radius, angle1, angle1);
  cairo_line_to (cr, xc, yc);
  cairo_arc (cr, xc, yc, radius, angle2, angle2);
  cairo_line_to (cr, xc, yc);
  cairo_stroke (cr);

  cairo_destroy (cr);
  cairo_surface_write_to_png (surface, OUTFILE);
  fprintf(stderr, "wrote " OUTFILE "\n");
  cairo_surface_destroy (surface);
  return 0;

}
