#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include "ssp.h"

int main (int argc, char **argv)
{
	int c;
	int sig;
	int s_flag;
	int l_flag;
	int err_flag;
	char buf [SIG2STR_MAX];
	pid_t pid;

	s_flag= 0;
	l_flag =0;
	err_flag = 0;
	opterr = 0;

	while ((c = getopt (argc, argv, "ls:")) != EOF) {
		switch (c) {
			case 'l':
				if (s_flag)	
					err_flag++;

				l_flag++;
				break;

			case 's':
				if ((l_flag) || (s_flag))
					err_flag++;

				s_flag++;
				snprintf (buf, SIG2STR_MAX - 1, "%s", optarg);
				break;

			default:
				err_flag++;
				break;
		}
	}

	if ((!l_flag) && (!s_flag))
		err_flag++;

	if (((s_flag) && (optind == argc)) || ((l_flag) && (optind != argc)))
		err_flag++;

	if (err_flag) {
		log_msg ("Usage: ssp_kill -l");
		log_msg ("       ssp_kill -s signal PID");
		exit (1);
	}

	if (l_flag) {
		for (sig = 0; sig <= MAXSIG; sig++) {
			sig2str (sig, buf);
			printf ("%s ", buf);
		}
		printf ("\n");
	}
	else {
		if (str2sig (buf, &sig) == -1)
			err_quit ("ssp_kill: %s: No such signal", buf);
		for (; optind < argc; optind++) {
			pid = atoi (argv [optind]);
			if (kill (pid, sig) == -1)
				err_ret ("Can't kill PID %d", pid);
		}
	}

	return (0);
}
