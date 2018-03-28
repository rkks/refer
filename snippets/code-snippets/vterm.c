#include <unistd.h>
#include <fcntl.h>
#include <pty.h>
#include <errno.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

#define _XOPEN_SOURCE
#include <stdlib.h>
extern char *ptsname(int fd);


#include <sys/types.h>
#include <sys/wait.h>

/**
 *	popen() and system() create a whole new shell, which
 * 	clutters strace output with tons of garbage.
 *	this just execs raw and does no such thing.
 */
int system_noshell(const char *cmd, ...);

int main(int argc, char *argv[])
{	// Open a pseudo-terminal, but don't make it our stdin/out/err
	int fd_master=posix_openpt(O_RDWR|O_NOCTTY);
	int ppid=getpid();	// Process ID of parent
	int cpid=-1;		// Get child PID later
	const char *pts;	// slave pseudoterm


	int s;	
	char c;

	assert( fd_master >= 0 );	// Did master terminal open?

	fprintf(stderr, "[P] Opened fd_master = %d\n", fd_master);

	assert( unlockpt(fd_master) >= 0 );	// Unlock PTY

	fprintf(stderr, "[P] Unlocked fd_master\n");

	assert( (pts=ptsname(fd_master)) != NULL ); // Get slave name

	fprintf(stderr, "[P] Name of slave: %s\n", pts);

	// Create a child process to use the slave pty
	assert( (cpid=fork()) >= 0);

	/**
	 *	Following a specific series of events here:
	 *	Parent			Child
	 *  ===================================
	 *	Open Master
	 *	fork
	 *				Open Child
	 *				Close Master
	 *	read			write '|'
	 *	write 'abc'
	 *	write EOF
	 *				read abc
	 *				EOF
	 *				exit
	 *	wait()
	 *	exit
	 */

	if(cpid == 0)	// Child code
	{
		int fd_slave=-1;	// Slave PTY
		// Save real stderr so fprintf writes to it instead of pty
		int STDERR=dup(STDERR_FILENO);
		FILE *stderr=fdopen(STDERR, "w");
		setvbuf(stderr, NULL, _IONBF, 0);

		assert( close(fd_master) >= 0 ); // Ditch master PTY

		assert( (fd_slave=open(pts,O_RDWR)) >= 0); // Open slave PTY

		fprintf(stderr, "\t[C] Opened slave %s\n", pts);

		// This will dup fd over stdin,out,err then close fd
		// This function needs compilation with -lutil
		assert( login_tty(fd_slave) >= 0 );

		// Yes..  I'll untangle the monstrous ioctl later
		assert( system_noshell("/bin/stty", "-echo", NULL) == 0);

		fprintf(stderr, "\t[C] Disabled echo\n");

		// We let the parent know the child has control of
		// the slave terminal by writing a char to it.
		assert( write(STDOUT_FILENO, "|", 1) == 1 );

		fprintf(stderr, "\t[C] Wrote char\n");

		//sleep(2);

		// We exec cat, to read until EOF.
		assert( execl("/bin/cat", "/bin/cat", NULL) >= 0);

		exit(1);
	}

	fprintf(stderr, "[P] Created child pid=%d\n", cpid);

	// Parent code
	assert( read(fd_master, &c, 1) == 1 );
	assert( c == '|' );	// Child should have written |

	fprintf(stderr, "[P] Read first char from child\n");

	// The \004 in the middle is not read by cat.
	// Instead, it makes cat do two reads of three bytes
	// instead of one read of 6 bytes.
	assert( write(fd_master, "abc\004abc", 7) == 7);

	fprintf(stderr, "[P] Wrote data to child\n");

	// This hangs if I don't write two EOF chars.
	assert( write(fd_master, "\004\004", 2) == 2 );

	fprintf(stderr, "[P] Wrote EOF\n");

	while(read(fd_master, &c, 1) == 1)	// Read bytes until EOF
	{
		if(c<0x20)	fprintf(stderr, "[P] Read ^%c\n", c+'@');
		else		fprintf(stderr, "[P] Read byte '%c'\n", c);
	}

	assert( wait(&s) == cpid );	// Wait for child to exit
	fprintf(stderr, "[P] Child has exited\n");
	assert( close(fd_master) >= 0 );
	fprintf(stderr, "[P] All finished.\n");
	return(0);
}

#define MAX_ARGS 16

int system_noshell(const char *cmd, ...)
{
	const char *args[MAX_ARGS]={cmd};
	int n=0, status;
	va_list ap;

	// Assemble varargs into the args array
	va_start(ap, cmd);
		do
			args[++n]=va_arg(ap, void *);
		while((args[n]) && (n<(MAX_ARGS-2)));
	va_end(ap);

	args[++n]=NULL;	// Terminate argument list

	n=fork();
	if(n < 0)
		return(-1);
	else if(n == 0)
	{
		execvp(cmd, args);
		exit(255);
	}

	assert( waitpid(n, &status, 0) == n);
	return(WEXITSTATUS(status));
}
