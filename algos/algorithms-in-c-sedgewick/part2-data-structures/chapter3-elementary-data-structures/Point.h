/* Program 3.3: Point data type interface
 * This interface defines a data type consisting of the set of
 * values "pairs of floating-point numbers" and the operation
 * consists of a function that computes the distance between two
 * points.
 */
typedef struct {
  float x;
  float y;
} point;

float distance (point, point);
