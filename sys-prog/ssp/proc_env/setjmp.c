#include <stdio.h>
#include <setjmp.h>
#include "ssp.h"

#define FOO_CMD 1

static char *token_ptr;
static jmp_buf main_context;

static void process_line (char *line);
static void do_foo (void);
static int get_token (void);

int main (void)
{
	char buf [LINE_LEN];

	if (setjmp (main_context) != 0)
		printf ("Error\n");

	while (fgets (buf, LINE_LEN, stdin) != NULL)
		process_line (buf);

	return (0);
}

static void process_line (char *line)
{
	int token;

	token_ptr = line;
	while ((token = get_token ()) != -1) {
		switch (token) {
			case FOO_CMD:
				do_foo ();
				break;
		}
	}
}

static void do_foo (void)
{
	int token;

	if ((token = get_token ()) == -1)
		longjmp (main_context, 1);

	/* Other processing for this function */
}

static int get_token (void)
{
	/*
	 * Get next token from line pointed to
	 * by token_ptr, and return its number.
	 */

	return (0);
}
