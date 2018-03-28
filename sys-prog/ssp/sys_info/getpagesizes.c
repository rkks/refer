#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include "ssp.h"

int main (void)
{
	int num_page_sizes;
	int res;
	int i;
	size_t *page_sizes;

	if ((num_page_sizes = getpagesizes (NULL, 0)) == -1)
		err_msg ("getpagesizes (NULL, 0) failed");

	if ((page_sizes = malloc (sizeof (size_t) * num_page_sizes)) == NULL)
		err_msg ("malloc failed");

	if ((res = getpagesizes (page_sizes, num_page_sizes)) == -1)
		err_msg ("getpagesizes failed");

	printf ("Supported page sizes:");
	for (i = 0; i < res; i++)
		printf (" %d", page_sizes [i]);
	printf ("\n");

	free (page_sizes);

	return (0);
}
