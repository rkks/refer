#include <stdio.h>
#include "ssp.h"

extern FILE *ssp_popen (const char *command, const char *mode);
extern int ssp_pclose (FILE *fp);

int main (void)
{
	char buf [LINE_LEN];
	FILE *fp;

	if ((fp = ssp_popen ("./rot13", "r")) == NULL)
		err_msg ("ssp_popen failed");

	for (;;) {
		printf ("Enter some text: ");
		fflush (stdout);

		if (fgets (buf, LINE_LEN, fp) == NULL)
			break;

		if (printf ("%s", buf) == EOF)
			err_msg ("printf to pipe failed");
	}

	if (ssp_pclose (fp) == -1)
		err_msg ("ssp_pclose failed");
	printf ("\n");

	return (0);
}
