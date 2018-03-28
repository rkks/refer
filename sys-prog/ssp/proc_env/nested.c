#include <stdio.h>
#include "ssp.h"

#define FOO_CMD 1

static char *token_ptr;

static void process_line (char *line);
static void do_foo (void);
static int get_token (void);

int main (void)
{
	char buf [LINE_LEN];

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

	token = get_token ();
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
