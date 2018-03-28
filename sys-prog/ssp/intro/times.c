#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

#define ITERATIONS 100

static void nop (void);

int main (void)
{
	hrtime_t start;
	hrtime_t stop;
	int i;

	start = gethrtime ();
	for (i = 0; i < ITERATIONS; i++)
		getpid ();
	stop = gethrtime ();
	printf ("getpid = %lld\n", (stop - start) / ITERATIONS);

	start = gethrtime ();
	for (i = 0; i < ITERATIONS; i++)
		gethrtime ();
	stop = gethrtime ();
	printf ("gethrtime = %lld\n", (stop - start) / ITERATIONS);

	start = gethrtime ();
	for (i = 0; i < ITERATIONS; i++)
		nop ();
	stop = gethrtime ();
	printf ("nop = %lld\n", (stop - start) / ITERATIONS);

	return (0);
}

static void nop (void)
{
	/* Do nothing */
}
