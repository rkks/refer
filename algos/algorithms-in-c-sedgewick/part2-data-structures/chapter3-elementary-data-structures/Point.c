/* Program 3.4: Point data type implementation
 * This implementation provides the definition for the distance function for
 * points that is declared in Program 3.3. It makes use of a library function
 * to compute the square root.
 */
#include <math.h>
#include "Point.h"

float distance(point a, point b) {
  float dx = a.x - b.x, dy = a.y - b.y;
  return sqrt(dx*dx + dy*dy);
}
