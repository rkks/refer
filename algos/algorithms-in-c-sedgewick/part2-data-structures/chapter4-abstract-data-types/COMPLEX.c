/* Program 4.15: Complex-numbers data-type implementation
 * Theses function implementations for the complex numbers data type are
 * straightforward. However, we would prefer not to separate them from the
 * definition of the `Complex` type, which is defined in the interface for the
 * convenience of the client.
 */
#include "COMPLEX.h"

Complex COMPLEXinit(float Re, float Im) {
  Complex t;
  t.Re = Re;
  t.Im = Im;
  return t;
}

float Re(Complex z) {
  return z.Re;
}

float Im(Complex z) {
  return z.Im;
}

Complex COMPLEXmult(Complex a, Complex b) {
  Complex t;
  t.Re = a.Re*b.Re - a.Im*b.Im;
  t.Im = a.Re*b.Im + a.Im*b.Re;
  return t;
}
