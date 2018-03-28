#include "defs.h"
#include <semaphore.h>

#define SEMFILE "/testsem"

int main(void)
{
	sem_t *sem;
	pid_t pid;
	int val, i;

	(void)sem_unlink(SEMFILE);
	ec_cmp( sem = sem_open(SEMFILE, O_CREAT, 0666, 5), SEM_FAILED )
	if ((pid = fork()) == 0) {
		while (true) {
			printf("child waiting...\n");
			ec_neg1( sem_wait(sem) )
			ec_neg1( sem_getvalue(sem, &val) )
			printf("child got semaphore; value = %d\n", val);
		}
	}
	printf("parent ready for post loop\n");
	for (i = 0; i < 3; i++) {
		ec_neg1( sem_post(sem) )
		sleep(1);
	}
	kill(pid, SIGKILL);
	waitpid(pid, NULL, 0);
	printf("parent done\n");
	exit(EXIT_SUCCESS);

EC_CLEANUP_BGN
	exit(EXIT_FAILURE);
EC_CLEANUP_END
}
