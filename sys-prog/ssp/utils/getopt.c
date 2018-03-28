#include <stdio.h>
#include <stdlib.h>
#include "ssp.h"

#define BG 0
#define FG 1
#define SOLID 2

char *subopts [] = {
	"bg",
	"fg",
	"solid",
	NULL
};

int main (int argc, char **argv)
{
	int c;
	int circle;
	int rectangle;
	int triangle;
	int err_flag;
	char *options;
	char *value;

	circle = 0;
	rectangle = 0;
	triangle = 0;
	err_flag = 0;

	while ((c = getopt (argc, argv, "cf:o:rt")) != EOF) {
		switch (c) {
			case 'c':
				if (rectangle || triangle)
					err_flag++;
				else {
					printf ("Circle\n");
					circle++;
				}
				break;

			case 'f':
				printf ("Filename is %s\n", optarg);
				break;

			case 'o':
				options = optarg;
				while (*options != '\0') {
					switch (getsubopt (&options, subopts, &value)) {
						case BG:
							if (value != NULL)
								printf ("Background colour is %s\n", value);
							else
								printf ("Missing background colour\n");
							break;

						case FG:
							if (value != NULL)
								printf ("Foreground colour is %s\n", value);
							else
								printf ("Missing foreground colour\n");
							break;

						case SOLID:
							printf ("Solid\n");
							break;

						default:
							printf ("Unknown option: %s\n", value);
							break;
					}
				}
				break;

			case 'r':
				if (circle || triangle)
					err_flag++;
				else {
					printf ("Rectangle\n");
					rectangle++;
				}
				break;

			case 't':
				if (circle || rectangle)
					err_flag++;
				else {
					printf ("Triangle\n");
					triangle++;
				}
				break;

			default:
				err_flag++;
				break;
		}
	}

	if (!circle && !rectangle && !triangle)
		printf ("Square\n");

	if (err_flag)
		err_quit ("Usage: getopt [-c | -r | -t] [-f filename] [-o options]");

	for (; optind < argc; optind++)
		printf ("Operand: %s\n", argv [optind]);

	return (0);
}
