#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

int ssp_system (const char *string)
{
	int status;
	int rc;
	int w;
	pid_t pid;
	char *shell;
	char *shell_path;

	if (sysconf (_SC_XOPEN_VERSION) < 4) {
		shell = "sh";
		shell_path = "/bin/sh";
	}
	else {
		shell = "sh";
		shell_path = "/usr/xpg4/bin/sh";
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

	if ((pid = vfork ()) == -1) {
		rc = -1;
		goto bail;
	}
	else if (pid == 0) {
		execl (shell_path, shell, "-c", string, NULL);
		_exit (127);
	}

	do {
		w = waitpid (pid, &status, 0);
	} while ((w == -1) && (errno == EINTR));

	rc = ((w == -1) ? w : status);

bail:
	return (rc);
}
