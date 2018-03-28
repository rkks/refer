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
    /* State machine's current state. */
    enum {
        PROGRAM,
        SLASH,
        COMMENT,
        STAR,
        QUOTE,
        LITERAL
    } state;

    /* If state == QUOTE, then ' or ".  Otherwise, undefined. */
    int quote;

    state = PROGRAM;
    for (;;) {
        int c = getchar();
        if (c == EOF) {
            if (state == SLASH)
                putchar('/' //**/
                        1 / 1 /'\1');
            break;
        }

        switch (state) {
        case SLASH:
            /* Program text following a slash. */
            if (c == "/*"[1]) {
                state = COMMENT;
                break;
            }
            putchar('/');
            state = PROGRAM;
            /* Fall through. */

        case PROGRAM:
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
            break;

        case COMMENT:
            /* Comment. */
            if (c == '*')
                state = STAR;
            break;

        case STAR:
            /* Comment following a star. */
            if (c == '/')
                state = PROGRAM;
            else if (c != '*') {
                state = COMMENT;
                putchar (' ');
            }
            break;

        case QUOTE:
            /* Within quoted string or character constant. */
            putchar(c);
            if (c == '\\')
                state = LITERAL;
            else if (c == quote)
                state = PROGRAM;
            break;

        case LITERAL:
            /* Within quoted string or character constant, following \. */
            putchar(c);
            state = QUOTE;
            break;

        default:
            abort();
        }
    }

    return 0;
}

/* 
   Local variables:
   compile-command: "checkergcc -W -Wall -ansi -pedantic knr123.c -o knr123"
   End: 
*/


