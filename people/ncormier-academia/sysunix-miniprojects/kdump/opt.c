/*
** opt.c for  in /home/nico/lang/c/kdump
**
** Made by nicolas
** Login   <nico@epita.fr>
**
** Started on  Fri Oct 28 21:59:52 2005 nicolas
** Last update Wed Nov  2 14:07:08 2005 Nicolas Cormier
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "kdump.h"

static void	usage(void)
{
  fprintf(stderr, "usage: kdump [-dEnlRT] [-f trfile] [-m maxdata] [-p pid] [-t [cnisuw]]\n");
  exit(1);
}

static void	opt_set_trpoints(int *trpoints, char *arg)
{
  if (*arg)
    *trpoints = 0;
  for (; *arg; arg++)
    {
      switch (*arg)
	{
	case 'c':
	    *trpoints |= MASKOPT_TR_C;
	    break;
	case 'n':
	    *trpoints |= MASKOPT_TR_N;
	    break;
	case 'i':
	    *trpoints |= MASKOPT_TR_I;
	    break;
	case 's':
	    *trpoints |= MASKOPT_TR_S;
	    break;
	case 'u':
	    *trpoints |= MASKOPT_TR_U;
	    break;
	case 'w':
	    *trpoints |= MASKOPT_TR_W;
	    break;
	case '+':
	    *trpoints |= MASKOPT_TR_PLUS;
	    break;
	default:
	  //bad tr points !
	  usage();
	}
    }
}

void	opt_parse(int argc, char **argv, int *noarg_opt, int *trpoints, char **arg_opt)
{
  int		ch;

  while ((ch = getopt(argc,argv,"f:dElm:np:RTt:")) != -1)
    {
      switch ((char) ch)
	{
	case 'f':
	  arg_opt[TABIDX_FILE] = optarg;
	  break;
	case 'd':
	  *noarg_opt |= MASKOPT_DECIMAL;
	  break;
	case 'l':
	  *noarg_opt |= MASKOPT_LOOPREADING;
	  break;
	case 'm':
	  arg_opt[TABIDX_MAXDATA] = optarg;
	  break;
	case 'n':
	  *noarg_opt |= MASKOPT_HOCSUPPR;
	  break;
	case 'p':
	  arg_opt[TABIDX_PID] = optarg;
	  break;
	case 'E':
	  *noarg_opt |= MASKOPT_TIMESTAMPS;
	  break;
	case 'R':
	  *noarg_opt |= MASKOPT_RETIMESTAMPS;
	  break;
	case 'T':
	  *noarg_opt |= MASKOPT_ABSTIMESTAMPS;
	  break;
	case 't':
	  opt_set_trpoints(trpoints, optarg);
	  break;
	default:
	  usage();
	}

    }
}
