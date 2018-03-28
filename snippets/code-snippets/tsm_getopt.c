#include <stdio.h>
#include "tsm_getopt.h"

/**
 * tsm_getopt (C) 2008 Tyler Montbriand, tsm@accesscomm.ca.  Freeware.
 */

static int tsm_findarg(const char *str, const opt options[]);

/*static struct
{
	char pidfile[512];
	short port;
} cmdline_options={{'\0'}, (short)1025};

const opt options[]={
	{ "pidfile", '\0', 1, cmdline_options.pidfile, "%s"},
	{ "port", 'p', 1, &cmdline_options.port, "%hd"},
	{ NULL, 0, 0, NULL, NULL}
};

int main(int argc, char *argv[])
{
	if(tsm_getopt(argc, argv, options) < 0)
		return(1);

	return(0);
}*/

static int tsm_findarg(const char *str, const opt options[])
{
	int n;
	if(str[1] == '\0')
		return(-1);

	for(n=0; options[n].lname != NULL; n++)
	{
		if((options[n].sname) &&
			(str[1] == options[n].sname) &&
			(str[2] == '\0'))
			return(n);

		if(str[1] == '-')
		if(strcmp(str+2, options[n].lname)==0)
			return(n);
	}

	return(-1);
}

int tsm_usage(int argc, char *argv[], const opt options[])
{
	int n;

	fprintf(stderr, "Usage:  %s\n", argv[0]);

	for(n=0; options[n].lname != NULL; n++)
	{
		fprintf(stderr, "\t--%s", options[n].lname);
		if(options[n].sname)
			fprintf(stderr,"(-%c)", options[n].sname);
		if(options[n].usage)
			fprintf(stderr, "\t%s", options[n].usage);
		fprintf(stderr, "\n");
	}
}

int tsm_getopt(int argc, char *argv[], const opt options[])
{
	int arg=1;
	while(arg < argc)
	{
		int which;
		if(strcmp(argv[arg], "--") == 0)
			return(arg+2);
		else if(argv[arg][0] == '-')
		{
			which=tsm_findarg(argv[arg], options);
			if(which < 0)
			{
				fprintf(stderr, "invalid option '%s'\n", 
					argv[arg]);
				return(-1);
			}

			if(options[which].args)
			{
				if((argc - arg) < 2)
				{
					fprintf(stderr, "Need argument for %s\n",
						argv[arg]);
					return(-1);
				}

				// Store if possible
				if(options[which].val)
				{
					if(options[which].cmdstr)
					{
						if(sscanf(argv[arg+1], 
							options[which].cmdstr, 
							options[which].val) != 1)
						{
							fprintf(stderr, "Invalid value "
								"'%s' for option '%s'\n",
								argv[arg], argv[arg+1]);
							return(-1);
						}
					}
					else
						*((char **)options[which].val)=argv[arg+1];

					arg++;
				}
			}
			else if(options[which].val != NULL)
			{
				// flag if possible
				*((int *)options[which].val)=1;
			}

			arg++;
		}
		else
			return(arg);
	}

	return(arg);
}
