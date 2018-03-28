#include <string.h>

void    outputLine(char *);
char    *inputLine(void);

int
main(int argc, char **argv)
{
    int len;
    char *line;
    char list[1024];

    len = sizeof(list) - 2;
    list[0] = '\0';

    /*
     * For each line in the input...
     */
    while ((line = inputLine()) != NULL) {
        /*
         * Compute its length, plus room for a comma and a space.
         */
        len += strlen(line) + 2;

        /*
         * If we don't have room in the buffer, output
         * the buffer and start a new one.  Otherwise,
         * add a comma and this line.
         */
        if (len >= sizeof(list)) {
            if (list[0] != '\0')
                outputLine(list);

            strcpy(list, line);
            len = strlen(line);
        }
        else {
            strcat(list, ", ");
            strcat(list, line);
        }
    }

    /*
     * Output the last part of the list.
     */
    if (list[0] != '\0')
        outputLine(list);

    exit(0);
}
