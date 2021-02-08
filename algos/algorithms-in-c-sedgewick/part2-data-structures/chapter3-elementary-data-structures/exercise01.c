/* Exercise 3.1: Find the largest and smallest numbers that you can represent
 * with types `int`, `long int`, `short int`, `float` and `double` in your
 * programming environment.
 */
#include <stdio.h>
#include <limits.h>
#include <float.h>

int main() {
  printf("=== CONTENTS IN LIMITS.H ===\n");
  printf("MAX for int: %d\n", INT_MAX);
  printf("MIN for int: %d\n\n", INT_MIN);
  printf("MAX for long int: %ld\n", LONG_MAX);
  printf("MIN for long int: %ld\n\n", LONG_MIN);
  printf("MAX for short int: %d\n", SHRT_MAX);
  printf("MIN for short int: %d\n\n", SHRT_MIN);

  printf("=== CONTENTS IN FLOAT.H ===\n");
  printf("MAX for float: %e\n", FLT_MAX);
  printf("MIN for float: %e\n\n", FLT_MIN);
  printf("MAX for double: %e\n", DBL_MAX);
  printf("MIN for double: %e\n\n", DBL_MIN);
  return 0;
}
