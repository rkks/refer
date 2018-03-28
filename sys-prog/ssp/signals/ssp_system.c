#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

int ssp_system (const char *string)
{
	int status;
	int rc;
	int w;
	pid_t pid;
	char *shell;
	char *shell_path;
	struct sigaction def_action;
	struct sigaction ignore;
	struct sigaction chld;
	struct sigaction intr;
	struct sigaction quit;
	sigset_t mask;
	sigset_t saved_mask;

	if (sysconf (_SC_XOPEN_VERSION) < 4) {
		shell = "sh";
		shell_path = "/bin/sh";
	}
	else {
		shell = "ksh";
		shell_path = "/bin/ksh";
	}

	if (string == NULL) {
		 if (access (shell_path, X_OK) == -1) {
			rc = 0;
			goto bail;
		}
		else {
			rc = 1;
			goto bail;
		}
	}

	sigemptyset (&mask);
	sigaddset (&mask, SIGCHLD);
	sigprocmask (SIG_BLOCK, &mask, &saved_mask);

	memset (&def_action, 0, sizeof (def_action));
	def_action.sa_handler = SIG_DFL;
	sigaction (SIGCHLD, &def_action, &chld);

	memset (&ignore, 0, sizeof (ignore));
	ignore.sa_handler = SIG_IGN;
	sigaction (SIGINT, &ignore, &intr);
	sigaction (SIGQUIT, &ignore, &quit);

	switch (pid = vfork ()) {
		case -1:
			rc = -1;
			break;

		case 0:
			sigaction (SIGINT, &intr, NULL);
			sigaction (SIGQUIT, &quit, NULL);
			sigprocmask (SIG_SETMASK, &saved_mask, NULL);

			execl (shell_path, shell, "-c", string, NULL);
			_exit (127);

		default:
			do {
				w = waitpid (pid, &status, 0);
			} while ((w == -1) && (errno == EINTR));

			rc = ((w == -1) ? w : status);
			break;
	}

	sigaction (SIGINT, &intr, NULL);
	sigaction (SIGQUIT, &quit, NULL);
	sigaction (SIGCHLD, &chld, NULL);
	sigprocmask (SIG_SETMASK, &saved_mask, NULL);

bail:
	return (rc);
}
