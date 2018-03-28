#include <stdio.h>
#include <stdlib.h>
#include "ssp.h"

int main (int argc, char **argv)
{
	int buf_size;
	char buf [LINE_LEN];
	char *stdin_buf;
	char *stdout_buf;

	if (argc != 2)
		buf_size = 1024;
	else
		buf_size = atoi (argv [1]);

	if ((stdin_buf = malloc (buf_size)) == NULL)
		err_msg ("malloc failed");
	if ((stdout_buf = malloc (buf_size)) == NULL)
		err_msg ("malloc failed");

	setvbuf (stdin, stdin_buf, _IOLBF, buf_size);
	setvbuf (stdout, stdout_buf, _IOLBF, buf_size);

	while (fgets (buf, LINE_LEN, stdin) != NULL)
		if (fputs (buf, stdout) == EOF)
			err_msg ("fputs failed");

	if (ferror (stdin))
		err_msg ("fgets failed");

	return (0);
}
