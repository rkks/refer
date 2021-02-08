/* Program 4.22: First-class ADT interface for polynomials
 * As usual, a handle to a polynomial is a pointer to a structure that is
 * unspecified except for the tag name.
 */
typedef struct poly *Poly;
void showPOLY(Poly);
Poly POLYterm(int, int);
Poly POLYadd(Poly, Poly);
Poly POLYmult(Poly, Poly);
float POLYeval(Poly, float);
