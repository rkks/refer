/* Exercise 5.5: Provide a nonrecursive implementation of Euclid's algorithm.
 */
#include <stdio.h>
#include <stdlib.h>

int gcd(int m, int n) {
	int temp;

	while (n > 0) {
		temp = m;
		m = n;
		n = temp%n;
	}
	return m;
}

int main(int argc, char *argv[]) {
	int m = atoi(argv[1]), n = atoi(argv[2]),
			result = gcd(m, n);
	printf("GCD of %d and %d: %d\n", m, n, result);
  return 0;
}
