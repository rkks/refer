/*
	Long-pathname test program
	AUP2, Sec. 3.6.4 (not in book)

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
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

int main(void)
{
	char name[201];
	char path[10000], path2[sizeof(path)];
	long max_path;
	int i;

	(void)system("rm -rf xxxxxxxxxxxxxxxxxxxxxxxxxx*");
	errno = 0;
	max_path = pathconf(".", _PC_PATH_MAX);
	if (max_path == -1) {
		if (errno == 0) {
			printf("No limit on path\n");
			exit(EXIT_SUCCESS);
		}
		else {
			perror("pathconf");
			exit(EXIT_FAILURE);
		}
	}
	if (max_path > sizeof(path) - 1) {
		printf("Recompile with a bigger path buffer\n");
		exit(EXIT_SUCCESS);
	}
	if (getcwd(path2, sizeof(path2) - 1) == NULL) {
		perror("getcwd");
		exit(EXIT_FAILURE);
	}
	memset(name, 'x', sizeof(name) - 1);
	name[sizeof(name) - 1] = '\0';
	for (i = 0; i < 30; i++) {
		if (mkdir(name, 0700) == -1 || chdir(name) == -1) {
			perror("mkdir or chdir");
			exit(EXIT_FAILURE);
		}
		strcat(path2, "/");
		strcat(path2, name);
	}
	if (access(path2, F_OK) == -1 && errno == ENAMETOOLONG)
		printf("access returns ENAMETOOLONG, as expected\n");
	if (getcwd(path, sizeof(path) - 1) == NULL) {
		perror("getcwd 2");
		exit(EXIT_FAILURE);
	}
	if (strcmp(path, path2) == 0)
		printf("getcwd returned the correct path\n");
	printf("max_path = %ld; strlen(path) = %d\n", max_path, strlen(path));
	(void)system("rm -rf xxxxxxxxxxxxxxxxxxxxxxxxxx*");
	exit(EXIT_SUCCESS);
}
