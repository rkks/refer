#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "ssp.h"

int main (void)
{
	double num;
	double res;
	int n;
	char buf [LINE_LEN];

	while ((n = read (STDIN_FILENO, buf, LINE_LEN)) > 0) {
		buf [n] = '\0';
		if (sscanf (buf, "%lf", &num) == 1) {
			res = sqrt (num);
			snprintf (buf, LINE_LEN, "%lf\n", res);
			n = strlen (buf);
			if (writen (STDOUT_FILENO, buf, n) == -1)
				err_msg ("writen failed");
		}
		else {
			if (writen (STDOUT_FILENO, "Invalid arg\n", 12) == -1)
				err_msg ("writen failed");
		}
	}

	return (0);
}
