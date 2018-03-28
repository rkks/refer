#include <cairo.h>
#include <stdio.h>
#include <math.h> // M_PI

#define OUTFILE "fill.png"

int main() {

  cairo_surface_t *surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 400, 400);
  cairo_t *cr = cairo_create (surface);

  double s = 100;
  cairo_set_source_rgb (cr, 0, 0, 0);
  cairo_rectangle (cr, 0.25*s, 0.25*s, 0.5*s, 0.5*s);
  cairo_fill (cr);

  cairo_destroy (cr);
  cairo_surface_write_to_png (surface, OUTFILE);
  fprintf(stderr,"wrote " OUTFILE "\n");
  cairo_surface_destroy (surface);
  return 0;

}
