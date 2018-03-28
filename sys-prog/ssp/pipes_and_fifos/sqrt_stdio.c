#include <stdio.h>
#include <math.h>
#include "ssp.h"

int main (void)
{
	double num;
	double res;
	char buf [LINE_LEN];

	while (fgets (buf, LINE_LEN, stdin) != NULL) {
		if (sscanf (buf, "%lf", &num) == 1) {
			res = sqrt (num);
			if (printf ("%f\n", res) == EOF)
				err_msg ("printf failed");
		}
		else {
			if (printf ("Invalid arg\n") == EOF)
				err_msg ("printf failed");
		}
	}

	return (0);
}
