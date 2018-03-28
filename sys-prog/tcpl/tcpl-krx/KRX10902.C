#include <stdio.h>

int main(void)
{
	int c;
	while ((c = getchar()) != EOF) {
		 if (c == ' ') {
			putchar(c);
		 	while((c = getchar()) == ' ' && c != EOF)
				;
		}
		if (c == EOF)
			break; /* the break keyword is mentioned
				* in the introduction... 
				* */

		putchar(c);
	}
	return 0;
}
