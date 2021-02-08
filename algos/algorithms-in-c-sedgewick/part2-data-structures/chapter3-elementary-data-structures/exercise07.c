/* Exercise 3.7: Write a client program that uses the data type in Program 3.3
 * and 3.4 for the following task: Read a sequence of points (pairs of
 * floating-numbers) from standard input, and find the one that is closest to
 * the first.
 */
#include <stdio.h>
#include <float.h>
#include "Point.h"

int main() {
  point p0, p, p_closest;
  float distance_closest = FLT_MAX, d;

  scanf("%f %f\n", &p0.x, &p0.y);
  printf("Initial Point: %f, %f\n", p0.x, p0.y);
  while (scanf("%f %f\n", &p.x, &p.y) == 2) {
    printf("New Point: %f, %f\n", p.x, p.y);
    if ((d = distance(p0, p)) < distance_closest) {
      distance_closest = d;
      p_closest.x = p.x;
      p_closest.y = p.y;
      printf("Closest Point Updated\n");
    }
  }
  return 0;
}
