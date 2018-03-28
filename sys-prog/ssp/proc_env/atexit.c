#include <stdio.h>
#include <stdlib.h>
#include "ssp.h"

static void eh1 (void);
static void eh2 (void);

int main (void)
{
	if (atexit (eh2) != 0)	
		err_msg ("Can't register eh2");
	if (atexit (eh2) != 0)	
		err_msg ("Can't register eh2");

	if (atexit (eh1) != 0)	
		err_msg ("Can't register eh1");

	printf ("Returning from main\n");
	return (0);
}

static void eh1 (void)
{
	printf ("First exit handler\n");
}

static void eh2 (void)
{
	printf ("Second exit handler\n");
}
