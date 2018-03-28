/*
 * C comment stripper.
 *
 * Strips comments from C or C++ code.
 */

#include <stdio.h>

enum state_t { normal, string, character, block_comment, line_comment};

enum token_t { none, backslash, slash, star, tri1, tri2, tri_backslash};

static int print_mode(enum state_t s)
{
        return (s == normal || s == string || s == character);
}

void cstrip(FILE *infile, FILE *outfile)
{
        int ch;
        int comment_newline = 0;
        enum state_t state = normal;
        enum token_t token = none;
        enum token_t last_token = none;
        
        if (!infile || !outfile || (infile == outfile)) {
                return;
        }


        while ((ch = fgetc(infile)) != EOF) {
                switch (ch) {
                case '/':
                        if (token == tri2) {
                                token = tri_backslash;
                                if (print_mode(state))
                                        fputc(ch, outfile);
                        } else if (state == string || state == character) {
                                fputc(ch, outfile);
                                token = slash;
                        } else if (state == block_comment && token == star) {
                                state = normal;
                                token = none;

                                /* Replace block comments with whitespace. */
                                if (comment_newline) {
                                        fputc('\n', outfile);
                                } else {
                                        fputc(' ', outfile);
                                }
                        } else if (state == normal && token == slash) {
                                state = line_comment;
                                token = slash;
                        } else {
                                token = slash;
                        }
                        
                        break;

                case '\\':
                        if (state == normal && token == slash)
                                fputc('/', outfile);
                        if (print_mode(state))
                                fputc(ch, outfile);

                        if (token == backslash || token == tri_backslash) {
                                token = none;
                        } else {
                                last_token = token;
                                token = backslash;
                        }
                                
                        break;

                case '"':
                        if (state == normal && token == slash)
                                fputc('/', outfile);
                        if (state == string && token != backslash)
                                state = normal;
                        else if (state == normal && token != backslash)
                                state = string;

                        if (print_mode(state))
                                fputc(ch, outfile);

                        token = none;
                        
                        break;

                case '\'':
                        if (state == normal && token == slash)
                                fputc('/', outfile);
                        if (state == character && token != backslash)
                                state = normal;
                        else if (state == normal && token != backslash)
                                state = character;

                        if (print_mode(state))
                                fputc(ch, outfile);

                        token = none;
                        
                        break;

                case '\n':
                        /* This test is independent of the others. */
                        if (state == block_comment)
                                comment_newline = 1;
                        
                        if (state == normal && token == slash)
                                fputc('/', outfile);
                        
                        if (token == backslash || token == tri_backslash)
                                token = last_token;
                        else if (state == line_comment &&
                                        token != backslash) {
                                state = normal;
                                token = none;
                        } else {
                                token = none;
                        }

                        if (print_mode(state))
                                fputc(ch, outfile);

                        break;

                case '*':
                        if (state == normal && token == slash) {
                                state = block_comment;
                                token = none;
                                comment_newline = 0;
                        } else {
                                token = star;
                        }
                        
                        if (print_mode(state))
                                fputc(ch, outfile);

                        break;

                case '?':
                        if (state == normal && token == slash)
                                fputc('/', outfile);
                        
                        if (token == tri1) {
                                token = tri2;
                        } else if (token == tri2) {
                                token = tri2;   /* retain state */
                        } else {
                                /* We might need the last token if this
                                 * trigraph turns out to be a backslash.
                                 */
                                last_token = token;
                                token = tri1;
                        }

                        if (print_mode(state))
                                fputc(ch, outfile);

                        break;

                default:
                        if (state == normal && token == slash)
                                fputc('/', outfile);

                        if (print_mode(state))
                                fputc(ch, outfile);

                        token = none;

                        break;
                } /* switch */

        } /* while */

        return;
}


/* Small driver program. */

int main(void)
{
        cstrip(stdin, stdout);

        return 0;
}

