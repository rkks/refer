/* Exercise 3.5: Implement the necessary functions to allow Program 3.2 to be
 * used for random `bits` (numbers that can take only the values 0 or 1).
 */
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
typedef bool Number;

Number randNum() {
  double r = (double) rand()/RAND_MAX;
  return r < .5 ? 0 : 1;
}

int main(int argc, char *argv[]) {
	int i, N = atoi(argv[1]);
	float m1 = 0.0, m2 = 0.0;
	Number x;

	for (i = 0; i < N; ++i) {
		x = randNum();
		m1 += ((float) x)/N;
		m2 += ((float) x*x)/N;
	}
	printf("       Average: %f\n", m1);
	printf("Std. deviation: %f\n", sqrt(m2-m1*m1));
	return 0;
}
