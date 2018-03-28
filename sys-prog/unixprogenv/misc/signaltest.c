#include <stdio.h>
#include <signal.h>
#include <errno.h>
extern int errno;

main() {
	int c, n;
	char buf[100];
	int onintr();

	signal(SIGINT, onintr);
	for (;;) {
		n = read(0, buf, 100);
		if (n > 0)
			printf(buf);
		else {
			if (errno == EINTR) {
				errno = 0;
				printf("interrupt side %d\n", n);
			} else {
				printf("true end of file %d\n", n);
			}
		}
	}
}

onintr() {
	signal(SIGINT, onintr);
	printf("interrupt\n");
}
