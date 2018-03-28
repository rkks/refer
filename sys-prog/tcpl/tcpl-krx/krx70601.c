/* Exercise 7-6 - write a program to compare two files, printing the first line
 * where they differ
 *
 * Note : I amended this a bit...if a file is shorter than the other, but is identical
 * up to that point, the program prints out "EOF" as the string that's not equal. 
 * 
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFF_SIZE 1000

/* uses fgets, removes the '\n' at the end of the string if it exists */
char *safegets(char *buffer, int length, FILE *file)
{
        char *ptr;
        int len;
        
        if (buffer != NULL)
        {
                ptr = fgets(buffer, length, file);
                
                if (ptr != NULL)
                {
                        len = strlen(buffer);
                
                        if (len > 0)
                        {
                                if (buffer[len - 1] == '\n')
                                {
                                        buffer[len - 1] = '\0';
                                }
                        }
                }
                
                return ptr;
        }       
        
        return NULL;
}

int main(int argc, char *argv[])
{
        FILE *leftFile, *rightFile;
        char buff1[BUFF_SIZE], buff2[BUFF_SIZE];
        char *ptr1, *ptr2;
        unsigned long lineNum = 0;
        
        if (argc < 3)
        {
                fprintf(stderr, "Usage : 7_6 <path to file> <path to file>\n");
                return 0;
        }
        
        if (!(leftFile = fopen(argv[1], "r")))
        {
                fprintf(stderr, "Couldn't open %s for reading\n", argv[1]);     
                return 0;
        }
        
        if (!(rightFile = fopen(argv[2], "r")))
        {
                fprintf(stderr, "Couldn't open %s for reading\n", argv[2]);
                fclose(leftFile); /* RJH 10 Jul 2000 */
                return 0;
        }
        
        /* read through each file, line by line */
        ptr1 = safegets(buff1, BUFF_SIZE, leftFile);
        ptr2 = safegets(buff2, BUFF_SIZE, rightFile);
        ++lineNum;
        
        /* stop when either we've exhausted either file's data */
        while (ptr1 != NULL && ptr2 != NULL)
        {
                /* compare the two lines */
                if (strcmp(buff1, buff2) != 0)
                {
                        printf("Difference:\n");
                        printf("%lu\t\"%s\" != \"%s\"\n", lineNum, buff1, buff2);
                        goto CleanUp;
                }
                
                ptr1 = safegets(buff1, BUFF_SIZE, leftFile);
                ptr2 = safegets(buff2, BUFF_SIZE, rightFile);
                ++lineNum;
        }       

        /* 
         * if one of the files ended prematurely, it definitely 
         * isn't equivalent to the other 
         */
        if (ptr1 != NULL && ptr2 == NULL)
        {
                printf("Difference:\n");
                printf("%lu\t\"%s\" != \"EOF\"\n", lineNum, buff1);
        }       
        else if (ptr1 == NULL && ptr2 != NULL)
        {
                printf("Difference:\n");
                printf("%lu\t\"EOF\" != \"%s\"\n", lineNum, buff2);
        }
        else
        {
                printf("No differences\n");
        }

CleanUp:

        fclose(leftFile);
        fclose(rightFile);      
        return EXIT_SUCCESS;    
}
