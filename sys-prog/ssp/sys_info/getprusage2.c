#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/resource.h>
#ifdef NEED_SNPRINTF
#include <sys/procfs.h>
#else
#include <sys/old_procfs.h>
#endif
#include <limits.h>
#include "ssp.h"

static int getprusage (pid_t pid, prusage_t *pr_usage);
static void print_rusage (pid_t pid, prusage_t *buf);

int main (int argc, char **argv)
{
	pid_t pid;
	prusage_t buf;
	int i;

	if (argc == 1) {
		if (getprusage (-1, &buf) == -1)
			err_msg ("getprusage failed");
		print_rusage (getpid (), &buf);
	}
	else {
		for (i = 1; i < argc; i++) {
			pid = atoi (argv [i]);
			if (getprusage (pid, &buf) == -1)
				err_ret ("getprusage failed");
			else
				print_rusage (pid, &buf);
		}
	}

	return (0);
}

static int getprusage (pid_t pid, prusage_t *pr_usage)
{
	int fd;
	char name [PATH_MAX];

	if (pid == -1)
		snprintf (name, PATH_MAX, "/proc/%ld", (long) getpid ());
	else
		snprintf (name, PATH_MAX, "/proc/%ld", (long) pid);

	if ((fd = open (name, O_RDONLY)) == -1)
		return (-1);

	if (ioctl (fd, PIOCUSAGE, pr_usage) == -1) {
		close (fd);
		return (-1);
	}
	else {
		close (fd);
		return (0);
	}
}

static void print_rusage (pid_t pid, prusage_t *buf)
{
	printf ("Resource usage for PID %ld:\n", (long) pid);
	printf ("  LWP ID: %ld\n", (long) buf -> pr_lwpid);
	printf ("  Number of LWPs: %d\n", (int) buf -> pr_count);
	printf ("  Timestamp: %ld.%ld\n", buf -> pr_tstamp.tv_sec,
		buf -> pr_tstamp.tv_nsec);
	printf ("  Creation time: %ld.%ld\n", buf -> pr_create.tv_sec,
		buf -> pr_create.tv_nsec);
	printf ("  Termination time: %ld.%ld\n", buf -> pr_term.tv_sec,
		buf -> pr_term.tv_nsec);
	printf ("  Real (elapsed) time: %ld.%ld\n", buf -> pr_rtime.tv_sec,
		buf -> pr_rtime.tv_nsec);
	printf ("  User CPU time: %ld.%ld\n", buf -> pr_utime.tv_sec,
		buf -> pr_utime.tv_nsec);
	printf ("  System CPU time: %ld.%ld\n", buf -> pr_stime.tv_sec,
		buf -> pr_stime.tv_nsec);
	printf ("  System trap CPU time: %ld.%ld\n", buf -> pr_ttime.tv_sec,
		buf -> pr_ttime.tv_nsec);
	printf ("  Text page fault CPU time: %ld.%ld\n", buf -> pr_tftime.tv_sec,
		buf -> pr_tftime.tv_nsec);
	printf ("  Data page fault CPU time: %ld.%ld\n", buf -> pr_dftime.tv_sec,
		buf -> pr_dftime.tv_nsec);
	printf ("  Kernel page fault CPU time: %ld.%ld\n", buf -> pr_kftime.tv_sec,
		buf -> pr_kftime.tv_nsec);
	printf ("  User lock wait time: %ld.%ld\n", buf -> pr_ltime.tv_sec,
		buf -> pr_ltime.tv_nsec);
	printf ("  Other sleep time: %ld.%ld\n", buf -> pr_slptime.tv_sec,
		buf -> pr_slptime.tv_nsec);
	printf ("  CPU latency time: %ld.%ld\n", buf -> pr_wtime.tv_sec,
		buf -> pr_wtime.tv_nsec);
	printf ("  Stopped time: %ld.%ld\n", buf -> pr_stoptime.tv_sec,
		buf -> pr_stoptime.tv_nsec);
	printf ("  Minor faults: %ld\n", buf -> pr_minf);
	printf ("  Major faults: %ld\n", buf -> pr_majf);
	printf ("  Number of swaps: %ld\n", buf -> pr_nswap);
	printf ("  Input blocks: %ld\n", buf -> pr_inblk);
	printf ("  Output blocks: %ld\n", buf -> pr_oublk);
	printf ("  Messages sent: %ld\n", buf -> pr_msnd);
	printf ("  Messages received: %ld\n", buf -> pr_mrcv);
	printf ("  Signals received: %ld\n", buf -> pr_sigs);
	printf ("  Voluntary context switches: %ld\n", buf -> pr_vctx);
	printf ("  Involuntary context switches: %ld\n", buf -> pr_ictx);
	printf ("  System calls: %ld\n", buf -> pr_sysc);
	printf ("  Characters read/written: %ld\n", buf -> pr_ioch);
}
