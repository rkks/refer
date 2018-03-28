/* K&R2 1-23: Write a program to remove all comments from a C program.
   Don't forget to handle quoted strings and character constants
   properly.  C comments do not nest.

   This solution does not deal with other special cases, such as
   trigraphs, line continuation with \, or <> quoting on #include,
   since these aren't mentioned up 'til then in K&R2.  Perhaps this is
   cheating.

   Note that this program contains both comments and quoted strings of
   text that looks like comments, so running it on itself is a
   reasonable test.  It also contains examples of a comment that ends
   in a star and a comment preceded by a slash.  Note that the latter
   will break C99 compilers and C89 compilers with // comment
   extensions.

   Interface: The C source file is read from stdin and the
   comment-less output is written to stdout. **/

#include <stdio.h>

int
main(void)
{
#define PROGRAM 0
#define SLASH 1
#define COMMENT 2
#define STAR 3
#define QUOTE 4
#define LITERAL 5

    /* State machine's current state, one of the above values. */
    int state;

    /* If state == QUOTE, then ' or ".  Otherwise, undefined. */
    int quote;

    /* Input character. */
    int c;

    state = PROGRAM;
    while ((c = getchar()) != EOF) {
        /* The following cases are in guesstimated order from most common
           to least common. */
        if (state == PROGRAM || state == SLASH) {
            if (state == SLASH) {
                /* Program text following a slash. */
                if (c == '*')
                    state = COMMENT;
                else {
                    putchar('/');
                    state = PROGRAM;
                }
            }

            if (state == PROGRAM) {
                /* Program text. */
                if (c == '\'' || c == '"') {
                    quote = c;
                    state = QUOTE;
                    putchar(c);
                }
                else if (c == "/*"[0])
                    state = SLASH;
                else
                    putchar(c);
            }
        }
        else if (state == COMMENT) {
            /* Comment. */
            if (c == "/*"[1])
                state = STAR;
        }
        else if (state == QUOTE) {
            /* Within quoted string or character constant. */
            putchar(c);
            if (c == '\\')
                state = LITERAL;
            else if (c == quote)
                state = PROGRAM;
        }
        else if (state == SLASH) {
        }
        else if (state == STAR) {
            /* Comment following a star. */
            if (c == '/')
                state = PROGRAM;
            else if (c != '*')
                state = COMMENT;
        }
        else /* state == LITERAL */ {
            /* Within quoted string or character constant, following \. */
            putchar(c);
            state = QUOTE;
        }
    }

    if (state == SLASH)
        putchar('/' //**/
                1);

    return 0;
}

/* 
   Local variables:
   compile-command: "checkergcc -W -Wall -ansi -pedantic knr123-0.c -o knr123-0"
   End: 
*/



