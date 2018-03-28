/*
 * Solution to exercise 4-14 in K&R2, page 91:
 *
 *	Define a macro swap(t,x,y) that interchanges two arguments of type t.
 *	(Block structure will help.)
 *
 * Feel free to modify and copy, if you really must, but preferably not.
 * This is just an exercise in preprocessor mechanics, not an example of
 * how it should really be used. The trickery is not worth it to save three
 * lines of code.
 *
 * To exchange the values of two variables we need a temporary variable and
 * this one needs a name. Any name we pick, the user of the macro might also
 * use. Thus, we use the preprocessor argument concatenation operator ## to
 * create the name from the actual variable names in the call. This guarantees
 * that the result won't be either of the actual arguments. In order to
 * make sure the result also does not fall into the implementation's name
 * space, we prefix the name with something safe.
 *
 * Lars Wirzenius <liw@iki.fi>
 */

#include <stdio.h>

#define swap(t, x, y) \
	do { \
		t safe ## x ## y; \
		safe ## x ## y = x; \
		x = y; \
		y = safe ## x ## y; \
	} while (0)

int main(void) {
	int ix, iy;
	double dx, dy;
	char *px, *py;
	
	ix = 42;
	iy = 69;
	printf("integers before swap: %d and %d\n", ix, iy);
	swap(int, ix, iy);
	printf("integers after swap: %d and %d\n", ix, iy);
	
	dx = 123.0;
	dy = 321.0;
	printf("doubles before swap: %g and %g\n", dx, dy);
	swap(double, dx, dy);
	printf("integers after swap: %g and %g\n", dx, dy);
	
	px = "hello";
	py = "world";
	printf("pointers before swap: %s and %s\n", px, py);
	swap(char *, px, py);
	printf("integers after swap: %s and %s\n", px, py);

	return 0;
}
