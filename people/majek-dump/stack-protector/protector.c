#include <stdio.h>
#include <stdlib.h>

__attribute__((noinline)) void fun() {
        char *buf = alloca(0x100);
        /* Don't allow gcc to optimize away the buf */
        asm volatile("" :: "m" (buf));
}


int main() {
	printf("[+] start\n");
	fun();
	printf("[+] end\n");
	return 0;
}
