#include <stdlib.h>
#include <stdio.h>

struct {
    char    *abbrev;
    char    *fullname;
} days[] = {
    "Sun",  "Sunday",
    "Mon",  "Monday",
    "Tue",  "Tuesday",
    "Wed",  "Wednesday",
    "Thu",  "Thursday",
    "Fri",  "Friday",
    "Sat",  "Saturday",
    0,      0
};

int
main(void)
{
    int i;
    int status;
    char command[BUFSIZ];

    /*
     * For each day, construct a command.
     */
    for (i=0; days[i].abbrev != NULL; i++) {
        /*
         * Run the date command, and use grep to search for
         * the day's abbreviated name.  Redirect the output
         * to /dev/null; we'll use the exit status to find
         * what we want.
         */
        sprintf(command, "date | grep %s > /dev/null", days[i].abbrev);

        /*
         * Run the command.  The termination status is returned
         * in status.
         */
        status = system(command);

        /*
         * The exit status is in the second byte of the
         * termination status.
         *
         * Grep returns 0 if a match was found, 1 if no
         * match was found, and 2 if an error occurred.
         */
        switch ((status >> 8) & 0xff) {
        case 0:
            printf("Today is %s.\n", days[i].fullname);
            break;
        case 1:
            printf("Today is not %s.\n", days[i].fullname);
            break;
        case 2:
            printf("Error in pattern specification.\n");
            exit(1);
        }
    }

    /*
     * Exit with a status of 0, indicating that
     * everything went fine.
     */
    exit(0);
}
