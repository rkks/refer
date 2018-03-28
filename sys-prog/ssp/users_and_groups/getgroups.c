#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <limits.h>
#include "ssp.h"

static void print_groups (int ngroups, const gid_t *groups);

int main (void)
{
	int ngroups;
	int i;
	gid_t groups [NGROUPS_MAX];

	if ((ngroups = getgroups (NGROUPS_MAX, groups)) == -1)
		err_msg ("getgroups failed");

	print_groups (ngroups, groups);

	for (i = 0; i < NGROUPS_MAX; i++)
		groups [i] = 10 + i;

	if (setgroups (NGROUPS_MAX, groups) == -1)
		err_msg ("setgroups failed");

	if ((ngroups = getgroups (NGROUPS_MAX, groups)) == -1)
		err_msg ("getgroups failed");

	print_groups (ngroups, groups);

	return (0);
}

static void print_groups (int ngroups, const gid_t *groups)
{
	int i;

	if (ngroups > 0) {
		printf ("%ld", (long) groups [0]);
		for (i = 1; i < ngroups; i++)
			printf (", %ld", (long) groups [i]);
		printf ("\n");
	}
}
