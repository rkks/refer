/* Program 4.14: First-class data type for complex numbers
 * This interface for complex numbers includes a `typedef` that allows
 * implementations to declare variables of type `Complex` and to use these
 * variables as function arguments and return values. However, the data type is
 * not abstract, because this representation is not hidden from clients.
 */
typedef struct {
  float Re;
  float Im;
} Complex;
Complex COMPLEXinit(float, float);
float Re(Complex);
float Im(Complex);
Complex COMPLEXmult(Complex, Complex);
