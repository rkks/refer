#include <stdio.h>
#include <stdlib.h>
#include "ssp.h"

int main (int argc, char **argv)
{
	int c;
	int buf_size;
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

	setvbuf (stdin, stdin_buf, _IOFBF, buf_size);
	setvbuf (stdout, stdout_buf, _IOFBF, buf_size);

	while ((c = getc (stdin)) != EOF)
		if (putc (c, stdout) == EOF)
			err_msg ("putc failed");

	if (ferror (stdin))
		err_msg ("getc failed");

	return (0);
}
