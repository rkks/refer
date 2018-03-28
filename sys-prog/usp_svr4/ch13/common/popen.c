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
    FILE *pf;
    char line[BUFSIZ];

    /*
     * Open a pipe to the date command.  We will
     * be reading from the pipe.
     */
    if ((pf = popen("date", "r")) == NULL) {
        perror("popen");
        exit(1);
    }

    /*
     * Read one line of output from the pipe.
     */
    if (fgets(line, sizeof(line), pf) == NULL) {
        fprintf(stderr, "No ouput from date command!\n");
        exit(1);
    }

    /*
     * For each day, see if it matches the output
     * from the date command.
     */
    for (i=0; days[i].abbrev != NULL; i++) {
        if (strncmp(line, days[i].abbrev, 3) == 0)
            printf("Today is %s.\n", days[i].fullname);
        else
            printf("Today is not %s.\n", days[i].fullname);
    }

    /*
     * Close the pipe and pick up the command's
     * termination status (which we ignore).
     */
    pclose(pf);

    /*
     * Exit with a status of 0, indicating that
     * everything went fine.
     */
    exit(0);
}
