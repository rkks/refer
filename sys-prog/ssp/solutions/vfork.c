#include <unistd.h>
#include "ssp.h"

static void func_1 (void);
static void func_2 (void);

int main (void)
{
	func_1 ();
	func_2 ();

	_exit (0);

	return (0);
}

static void func_1 (void)
{
	pid_t pid;

	if ((pid = vfork ()) == -1)
		err_msg ("vfork failed");
}

static void func_2 (void)
{
	char buf [LINE_LEN];
	int i;

	for (i = 0; i < LINE_LEN; i++)
		buf [i] = i % 255;
}
