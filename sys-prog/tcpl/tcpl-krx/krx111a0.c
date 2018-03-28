#include <assert.h>
#include <stdio.h>

int main(void)
{
    FILE *f;
    unsigned long i;
    static char *ws = " \f\t\v";
    static char *al = "abcdefghijklmnopqrstuvwxyz";
    static char *i5 = "a b c d e f g h i j k l m "
                      "n o p q r s t u v w x y z "
                      "a b c d e f g h i j k l m "
                      "n o p q r s t u v w x y z "
                      "a b c d e f g h i j k l m "
                      "n\n";

    /* Generate the following: */
    /* 0. input file contains zero words */
    f = fopen("test0", "w");
    assert(f != NULL);
    fclose(f);

    /* 1. input file contains 1 enormous word without any newlines */
    f = fopen("test1", "w");
    assert(f != NULL);
    for (i = 0; i < ((66000ul / 26) + 1); i++)
        fputs(al, f);
    fclose(f);

    /* 2. input file contains all white space without newlines */
    f = fopen("test2", "w");
    assert(f != NULL);
    for (i = 0; i < ((66000ul / 4) + 1); i++)
        fputs(ws, f);
    fclose(f);
 
    /* 3. input file contains 66000 newlines */
    f = fopen("test3", "w");
    assert(f != NULL);
    for (i = 0; i < 66000; i++)
        fputc('\n', f);
    fclose(f);
 
    /* 4. input file contains word/ 
     *    {huge sequence of whitespace of different kinds}
     *    /word 
     */
    f = fopen("test4", "w");
    assert(f != NULL);
    fputs("word", f);
    for (i = 0; i < ((66000ul / 26) + 1); i++)
        fputs(ws, f);
    fputs("word", f);
    fclose(f);

    /* 5. input file contains 66000 single letter words,
     *    66 to the line 
     */
    f = fopen("test5", "w");
    assert(f != NULL);
    for (i = 0; i < 1000; i++)
        fputs(i5, f);
    fclose(f);

    /* 6. input file contains 66000 words without any newlines */
    f = fopen("test6", "w");
    assert(f != NULL);
    for (i = 0; i < 66000; i++)
        fputs("word ", f);
    fclose(f);

    return 0;
}



