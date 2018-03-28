/*
 *  Print system load statistics using the data from /proc/stat
 *
 *  Copyright (C) 2009  RIEGL Research ForschungsGmbH
 *  Copyright (C) 2009  Clifford Wolf <clifford@clifford.at>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <asm/param.h>
#include <sys/types.h>
#include <sys/wait.h>

#ifndef USER_HZ
#  define USER_HZ HZ
#endif

struct systime_s {
	unsigned long user, nice, system, idle, iowait, irq, softirq, steal;
	unsigned long total;
};

struct systime_s init, now;

void read_systime(struct systime_s *st)
{
	int rc = 0;
	FILE *f = fopen("/proc/stat", "r");
	if (f) {
		rc = fscanf(f, "cpu %lu %lu %lu %lu %lu %lu %lu %lu",
				&st->user, &st->nice, &st->system, &st->idle,
				&st->iowait, &st->irq, &st->softirq, &st->steal);
		st->total = st->user + st->nice + st->system + st->idle + st->iowait + st->irq + st->softirq + st->steal;
		fclose(f);
	}
	if (rc != 8) {
		fprintf(stderr, "IO or parser error while reading /proc/stat!\n");
		exit(1);
	}
}

double timediff(unsigned long a, unsigned long b)
{
	return (b - a) / (float)USER_HZ;
}

void print_statistics()
{
	static int counter = 0;
	if (counter % 10 == 0) {
		printf("\n%5s %8s %8s %8s %8s %8s %8s %8s %8s %8s\n",
				"", "user", "nice", "system", "idle",
				"iowait", "irq", "softirq", "steal", "total");
	}
	read_systime(&now);
	printf("%05d %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f\n",
			counter,
			timediff(init.user,    now.user),
			timediff(init.nice,    now.nice),
			timediff(init.system,  now.system),
			timediff(init.idle,    now.idle),
			timediff(init.iowait,  now.iowait),
			timediff(init.irq,     now.irq),
			timediff(init.softirq, now.softirq),
			timediff(init.steal,   now.steal),
			timediff(init.total,   now.total));
	counter++;
}

void print_statistics_verbose()
{
	read_systime(&now);
	fprintf(stderr, "\n");
	fprintf(stderr, "user    %8.3f sec\n", timediff(init.user,    now.user));
	fprintf(stderr, "nice    %8.3f sec\n", timediff(init.nice,    now.nice));
	fprintf(stderr, "system  %8.3f sec\n", timediff(init.system,  now.system));
	fprintf(stderr, "idle    %8.3f sec\n", timediff(init.idle,    now.idle));
	fprintf(stderr, "iowait  %8.3f sec\n", timediff(init.iowait,  now.iowait));
	fprintf(stderr, "irq     %8.3f sec\n", timediff(init.irq,     now.irq));
	fprintf(stderr, "softirq %8.3f sec\n", timediff(init.softirq, now.softirq));
	fprintf(stderr, "steal   %8.3f sec\n", timediff(init.steal,   now.steal));
	fprintf(stderr, "total   %8.3f sec\n", timediff(init.total,   now.total));
}

int main(int argc, char **argv)
{
	read_systime(&init);
	if (argc <= 1) {
		while (1) {
			sleep(1);
			print_statistics();
			init = now;
		}
	} else if (argc == 2 && !strcmp(argv[1], "-s")) {
		while (1) {
			sleep(1);
			print_statistics();
		}
	} else {
		if (argv[1][0] == '-') {
			fprintf(stderr, "Usage: %s [ -s | command [ arg.. ] ]\n", argv[0]);
			exit(1);
		}
		if (fork() == 0) {
			execvp(argv[1], &argv[1]);
			fprintf(stderr, "Failed to exec '%s'!\n", argv[1]);
			exit(1);
		}
		int status;
		wait(&status);
		print_statistics_verbose();
		return WEXITSTATUS(status);
	}
	return 0;
}

