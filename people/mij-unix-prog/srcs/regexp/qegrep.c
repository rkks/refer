/*
 *  qegrep.c
 *
 *  compares its standard input against the regular
 *  expression given (on command line), and prints
 *  out those lines matching that RE.
 *
 *
 *  Created by Mij <mij@bitchx.it> on Mon Dec 29 2003.
 *  Original source file available on http://mij.oltrelinux.com/devel/unixprg/
 */

 /* max error message length */
#define MAX_ERR_LENGTH 80
 /* max length of a line of text from stdin */
#define MAX_TXT_LENGTH 600

#include <stdio.h>
 /* for --ansi (see off_t in regex.h) */
#include <sys/types.h>
 /* for strchr(): */
#include <string.h>
#include <regex.h>


int main(int argc, char *argv[]) {
     /* will contain the regular expression */
    regex_t myre;
    int err;
    char err_msg[MAX_ERR_LENGTH];
    char text[MAX_TXT_LENGTH];
    
     /* safe check on cmd line args */
    if ( argc < 2 || argc > 2 ) {
        printf("Usage:\n\tqegrep 'RE' < file\n\tOR\n\techo \"string\" | qegrep 'RE'\n");
        return 1;
    }
    
     /* compiles the RE. If this step fails, reveals what's wrong with the RE */
    if ( (err = regcomp(&myre, argv[1], REG_EXTENDED)) != 0 ) {
        regerror(err, &myre, err_msg, MAX_ERR_LENGTH);
        printf("Error analyzing regular expression '%s': %s.\n", argv[1], err_msg);
        return 1;
    }
    
     /* "executes" the RE against the text taken from stdin */
    while ( fgets(text, MAX_TXT_LENGTH, stdin) != NULL ) {
         /* we rely on the fact that text contains newline chars */
        *(strchr(text, '\n')) = '\0';
        if ( (err = regexec(&myre, text, 0, NULL, 0)) == 0 ) puts(text);
        else if ( err != REG_NOMATCH ) {
             /* this is when errors have been encountered */
            regerror(err, &myre, err_msg, MAX_ERR_LENGTH);
            return 2;
        }
    } 

     /* meaningless here. Useful in many other situations. */
    regfree(&myre);
    
    return 0;
}
    
    
