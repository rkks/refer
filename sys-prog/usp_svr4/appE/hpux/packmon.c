#include <stdio.h>

int
main(int argc, char **argv)
{
    printf("HP-UX 10.x does not support the DLPI extensions Sun has added.\n");
    printf("This program, unfortunately, needs those extensions. Therefore,\n");
    printf("this example does not do anything.\n");
    exit(0);
}
