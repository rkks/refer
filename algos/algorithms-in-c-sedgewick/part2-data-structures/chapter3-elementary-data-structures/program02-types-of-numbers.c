/* Program 3.2: Types of numbers
 * This program computes the average and standard deviation of a
 * sequence x_1, x_2, ..., x_N of integers generated by the
 * library procedure `rand`.
 *
 * Note that a direct implementation from the definition of the
 * variance requires one pass to compute the average and another
 * to compute the sums of the squares of the differences between
 * the members of the sequence and the average, but rearranging
 * the formula makes it possible for us to compute the variance in
 * one pass through the data.
 *
 * We use the `typedef` declaration to localize reference to the
 * fact that the type of the data in `int`. For example, we could
 * keep the `typedef` and the function `randNum` in a separate
 * file (referenced by an `include` directive), an then we could
 * use this program to teset random numbers of a different type by
 * changing that file.
 *
 * Whatever the type of the data, the program uses `int`s for
 * indices and `float`s to compute the average and standard
 * deviation, and will be effective only if conversion functions
 * from the data to `float` perform in a reasonable manner.
 *
 * Future improvement:
 * 1. to put the typedef and randNum into a separate header file Num.h
 * 2. split into three files:
 *   * an interface, which defines the data structure and declares the
 *     functions to be used to manipulate the data structure
 *   * an implementation of the functions declared in the interface
 *   * a client program that uses the functions declared in the interface to
 *     work at a higher level of a abstraction
 */
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
typedef int Number;

Number randNum() { return rand(); }

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
