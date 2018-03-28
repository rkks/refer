/*
 *  getopt-short.c
 *  
 *  Parsing short-options command lines with getopt.
 *
 *
 *  Created by Mij <mij@bitchx.it> on 07/08/05.
 *  Original source file available on http://mij.oltrelinux.com/devel/unixprg/
 *
 */


#include <stdio.h>
#include <unistd.h>

#define OPT_NUM     4       /* a, b, c and x (h is not relevant here) */

extern char *optarg;

int main(int argc, char *argv[]) {
	int want_opt[OPT_NUM];     /* want option? */
	char ch;                   /* service variables */
	int i;
	char *my_argument;
	
	
	/* init want_opt array */
	for (i = 0; i < OPT_NUM; i++)
	   want_opt[i] = 0;

    /* optstring: a, b, c, h; and x taking an argument */
    while ((ch = getopt(argc, argv, "abchx:")) != -1) {     /* getopt() iterates over argv[] */
        switch (ch) {       /* what getopt() returned */
            case 'a':       /* a has been recognized (possibly for the second or more time) */
                want_opt[0] = want_opt[0] + 1;     /* remember about a */
                break;
            case 'b':       /* b */
                want_opt[1]++;
                break;
            case 'c':
                want_opt[2]++;
                break;
            case 'x':
                want_opt[3]++;
                my_argument = optarg;   /* preserve the pointer to x' argument */
                break;
            case 'h':   /* want help */
            default:
                /* no options recognized: print options summary */
                printf("Usage:\n%s [-a] [-b] [-c] [-h] [-x]\n", argv[0]);
                
                /* typically here: 
                exit(EXIT_FAILURE);
                */
        }
    }
    
    /* print out results */
    printf("You requested:\n");
    if (want_opt[0]) printf("a [%d]\n", want_opt[0]);
    if (want_opt[1]) printf("b [%d]\n", want_opt[1]);
    if (want_opt[2]) printf("c [%d]\n", want_opt[2]);
    if (want_opt[3]) printf("x [%d]: %s", want_opt[3], my_argument);  /* but only the last one rests */
 
    printf("\n");
    return 0;
}
