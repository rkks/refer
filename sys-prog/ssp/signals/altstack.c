#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/mman.h>
#include "ssp.h"

static void sigusr (int sig);
static void print_stack_info (const char *msg, stack_t *stack);

int main (void)
{
	struct sigaction act;
	stack_t alt_stack;
	int fd;

	act.sa_handler = sigusr;
	sigemptyset (&act.sa_mask);
	act.sa_flags = 0;
	if (sigaction (SIGUSR1, &act, NULL) == -1)
		err_msg ("sigaction failed");

	printf ("Using default stack\n");
	raise (SIGUSR1);

	if ((fd = open ("/dev/zero", O_RDWR)) == -1)
		err_msg ("open failed");
	if ((alt_stack.ss_sp = mmap (NULL, SIGSTKSZ, PROT_READ | PROT_WRITE,
		MAP_PRIVATE, fd, 0)) == MAP_FAILED) {
		err_msg ("mmap failed");
	}
	close (fd);
	alt_stack.ss_size = SIGSTKSZ;
	alt_stack.ss_flags = 0;
	if (sigaltstack (&alt_stack, NULL) == -1)
		err_msg ("sigaltstack failed");

	act.sa_flags = SA_ONSTACK;
	if (sigaction (SIGUSR1, &act, NULL) == -1)
		err_msg ("sigaction failed");

	printf ("\nUsing alternate stack\n");
	raise (SIGUSR1);

	return (0);
}

static void sigusr (int sig)
{
	stack_t alt_stack;

	if (sigaltstack (NULL, &alt_stack) == -1)
		err_msg ("sigaltstack failed");

	print_stack_info ("  Handler", &alt_stack);
}

static void print_stack_info (const char *msg, stack_t *stack)
{
	printf ("%s stack base: %p\n", msg, stack -> ss_sp);
	printf ("%s stack size: %ld\n", msg, (long) stack -> ss_size);
	if (stack -> ss_flags == 0)
		printf ("%s stack flags: 0\n", msg);
	else {
		printf ("%s stack flags: ", msg);
		if (stack -> ss_flags & SS_DISABLE)
			printf ("SS_DISABLE ");
		if (stack -> ss_flags & SS_ONSTACK)
			printf ("SS_ONSTACK");
		printf ("\n");
	}
}
