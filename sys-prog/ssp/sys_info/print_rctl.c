#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <rctl.h>

void print_rctl (rctlblk_t *rblk);

int print_rctls (const char *name)
{
	rctlblk_t *rblk;

	if ((rblk = malloc (rctlblk_size ())) == NULL)
		return (-1);

	if (getrctl (name, NULL, rblk, RCTL_FIRST) == -1)
		return (-1);

	printf ("%s:\n", name);
	print_rctl (rblk);

	while (getrctl (name, rblk, rblk, RCTL_NEXT) != -1)
		print_rctl (rblk);

	free (rblk);

	return (0);
}

void print_rctl (rctlblk_t *rblk)
{
	int tmp;
	int sig;

	printf ("  Process ID: %ld\n", (long) rctlblk_get_recipient_pid (rblk));

	printf ("  Privilege: ");
	switch (rctlblk_get_privilege (rblk)) {
		case RCPRIV_BASIC:
			printf ("RCPRIV_BASIC\n");
			break;

		case RCPRIV_PRIVILEGED:
			printf ("RCPRIV_PRIVILEGED\n");
			break;

		case RCPRIV_SYSTEM:
			printf ("RCPRIV_SYSTEM\n");
			break;

		default:
			printf ("Unknown privilege\n");
			break;
	}

	printf ("  Value: %llu\n", rctlblk_get_value (rblk));
	printf ("  Enforced value: %llu\n", rctlblk_get_enforced_value (rblk));

	printf ("  Global flags: ");
	tmp = rctlblk_get_global_flags (rblk);
	if (tmp & RCTL_GLOBAL_DENY_ALWAYS)
		printf ("RCTL_GLOBAL_DENY_ALWAYS ");
	if (tmp & RCTL_GLOBAL_DENY_NEVER)
		printf ("RCTL_GLOBAL_DENY_NEVER ");
	if (tmp & RCTL_GLOBAL_CPU_TIME)
		printf ("RCTL_GLOBAL_CPU_TIME ");
	if (tmp & RCTL_GLOBAL_FILE_SIZE)
		printf ("RCTL_GLOBAL_FILE_SIZE ");
	if (tmp & RCTL_GLOBAL_INFINITE)
		printf ("RCTL_GLOBAL_INFINITE ");
	if (tmp & RCTL_GLOBAL_LOWERABLE)
		printf ("RCTL_GLOBAL_LOWERABLE ");
	if (tmp & RCTL_GLOBAL_NOBASIC)
		printf ("RCTL_GLOBAL_NOBASIC ");
	if (tmp & RCTL_GLOBAL_NOLOCALACTION)
		printf ("RCTL_GLOBAL_NOLOCALACTION ");
	if (tmp & RCTL_GLOBAL_UNOBSERVABLE)
		printf ("RCTL_GLOBAL_UNOBSERVABLE ");
	printf ("\n");

	printf ("  Global actions: ");
	tmp = rctlblk_get_global_action (rblk);
	if (tmp & RCTL_GLOBAL_NOACTION)
		printf ("RCTL_GLOBAL_NOACTION ");
	if (tmp & RCTL_GLOBAL_SYSLOG)
		printf ("RCTL_GLOBAL_SYSLOG ");
	printf ("\n");

	printf ("  Local flags: ");
	tmp = rctlblk_get_local_flags (rblk);
	if (tmp & RCTL_LOCAL_MAXIMAL)
		printf ("RCTL_LOCAL_MAXIMAL ");
	printf ("\n");

	printf ("  Local actions: ");
	tmp = rctlblk_get_local_action (rblk, &sig);
	if (tmp & RCTL_LOCAL_DENY)
		printf ("RCTL_LOCAL_DENY ");
	if (tmp & RCTL_LOCAL_NOACTION)
		printf ("RCTL_LOCAL_NOACTION ");
	if (tmp & RCTL_LOCAL_SIGNAL) {
		printf ("RCTL_LOCAL_SIGNAL ");
		switch (sig) {
			case SIGABRT:
				printf ("(SIGABRT)");
				break;

			case SIGXRES:
				printf ("(SIGXRES)");
				break;

			case SIGHUP:
				printf ("(SIGHUP)");
				break;

			case SIGSTOP:
				printf ("(SIGSTOP)");
				break;

			case SIGTERM:
				printf ("(SIGTERM)");
				break;

			case SIGKILL:
				printf ("(SIGKILL)");
				break;

			case SIGXCPU:
				printf ("(SIGXCPU)");
				break;

			case SIGXFSZ:
				printf ("(SIGXFSZ)");
				break;

			default:
				printf ("(Illegal signal)");
				break;
		}
	}
	printf ("\n");

	printf ("  Firing time: %llu\n\n", rctlblk_get_firing_time (rblk));
}
