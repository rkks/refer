/*
	Example using shared memory (bad example)
	AUP2, Sec. 7.13.1

	Copyright 2003 by Marc J. Rochkind. All rights reserved.
	May be copied only for purposes and under conditions described
	on the Web page www.basepath.com/aup/copyright.htm.

	The Example Files are provided "as is," without any warranty;
	without even the implied warranty of merchantability or fitness
	for a particular purpose. The author and his publisher are not
	responsible for any damages, direct or incidental, resulting
	from the use or non-use of these Example Files.

	The Example Files may contain defects, and some contain deliberate
	coding mistakes that were included for educational reasons.
	You are responsible for determining if and how the Example Files
	are to be used.

*/
#include "defs.h"
#include <sys/shm.h>
/*[pgm]*/
static int *getaddr(void)
{
	key_t key;
	int shmid, *p;

	(void)close(open("shmseg", O_WRONLY | O_CREAT, 0));
	ec_neg1( key = ftok("shmseg", 1) )
	ec_neg1( shmid = shmget(key, sizeof(int), IPC_CREAT | PERM_FILE) )
	ec_neg1( p = shmat(shmid, NULL, 0) )
	return p;

EC_CLEANUP_BGN
	return NULL;
EC_CLEANUP_END
}

int main(void)
{
	pid_t pid;

	if ((pid = fork()) == 0) {
		int *p, prev = 0;

		ec_null( p = getaddr() )
		while (*p != 99)
			if (prev != *p) {
				printf("child saw %d\n", *p);
				prev = *p;
			}
		printf("child is done\n");
	}
	else {
		int *p;

		ec_null( p = getaddr() )
		for (*p = 1; *p < 4; (*p)++)
			sleep(1);
		*p = 99;
	}
	exit(EXIT_SUCCESS);

EC_CLEANUP_BGN
	exit(EXIT_FAILURE);
EC_CLEANUP_END
}

