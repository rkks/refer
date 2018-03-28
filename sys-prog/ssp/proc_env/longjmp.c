#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

static jmp_buf main_context;

static void func1 (int i, int j, int k);
static void func2 (void);

int main (void)
{
	int var1;
	register int var2;
	volatile int var3;

	var1 = 1;
	var2 = 20;
	var3 = 300;

	printf ("Before setjmp: var1 = %d, var2 = %d, var3 = %d\n", var1,
		var2, var3);
	if (setjmp (main_context) != 0) {
		printf ("After longjmp: var1 = %d, var2 = %d, var3 = %d\n",
			var1, var2, var3);
		exit (0);
	}

	var1 = 1000;
	var2 = 20000;
	var3 = 300000;

	func1 (var1, var2, var3);

	return (0);
}

static void func1 (int i, int j, int k)
{
	printf ("In func1: var1 = %d, var2 = %d, var3 = %d\n", i, j, k);
	func2 ();
}

static void func2 (void)
{
	longjmp (main_context, 1);
}
