#include <stdlib.h>
#include <string.h>

/*
 * Sub-options.
 */
char    *subopts[] = {
#define COLOR   0
    "color",
#define SOLID   1
    "solid",
    NULL
};

int
main(int argc, char **argv)
{
    int c;
    char buf[1024];
    extern int optind;
    extern char *optarg;
    char *options, *value;

    /*
     * Process the arguments.
     */
    while ((c = getopt(argc, argv, "cf:o:st")) != -1) {
        switch (c) {
        case 'c':
            outputLine("circle");
            break;
        case 'f':
            strcpy(buf, "filename: ");
            strcat(buf, optarg);
            outputLine(buf);
            break;
        case 's':
            outputLine("square");
            break;
        case 't':
            outputLine("triangle");
            break;
        case '?':
            outputLine("command line error");
            break;
        case 'o':
            options = optarg;

            /*
             * Process the sub-options.
             */
            while (*options != '\0') {
                switch (getsubopt(&options, subopts, &value)) {
                case COLOR:
                    if (value != NULL) {
                        strcpy(buf, "color: ");
                        strcat(buf, value);
                    }
                    else {
                        strcpy(buf, "missing color");
                    }

                    outputLine(buf);
                    break;
                case SOLID:
                    outputLine("solid");
                    break;
                default:
                    strcpy(buf, "unknown option: ");
                    strcat(buf, value);
                    outputLine(buf);
                    break;
                }
            }

            break;
        }
    }

    /*
     * Process extra arguments.
     */
    for (; optind < argc; optind++) {
        strcpy(buf, "extra argument: ");
        strcat(buf, argv[optind]);
        outputLine(buf);
    }

    exit(0);
}
