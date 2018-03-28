#include <stdio.h>
#include <ctype.h>

int main (void)
{
	char c;

	printf ("Enter the character to test: ");
	scanf ("%c", &c);

	printf ("isalpha ('%c') is %s\n", c, isalpha ((int) c) ? "True" : "False");
	printf ("isalnum ('%c') is %s\n", c, isalnum ((int) c) ? "True" : "False");
	printf ("isascii ('%c') is %s\n", c, isascii ((int) c) ? "True" : "False");
	printf ("iscntrl ('%c') is %s\n", c, iscntrl ((int) c) ? "True" : "False");
	printf ("isdigit ('%c') is %s\n", c, isdigit ((int) c) ? "True" : "False");
	printf ("isgraph ('%c') is %s\n", c, isgraph ((int) c) ? "True" : "False");
	printf ("islower ('%c') is %s\n", c, islower ((int) c) ? "True" : "False");
	printf ("isprint ('%c') is %s\n", c, isprint ((int) c) ? "True" : "False");
	printf ("ispunct ('%c') is %s\n", c, ispunct ((int) c) ? "True" : "False");
	printf ("isspace ('%c') is %s\n", c, isspace ((int) c) ? "True" : "False");
	printf ("isupper ('%c') is %s\n", c, isupper ((int) c) ? "True" : "False");
	printf ("isxdigit ('%c') is %s\n\n", c,
		isxdigit ((int) c) ? "True" : "False");

	printf ("tolower ('%c') gives %c\n", c, tolower ((int) c));
	printf ("_tolower ('%c') gives %c\n", c, _tolower ((int) c));
	printf ("toupper ('%c') gives %c\n", c, toupper ((int) c));
	printf ("_toupper ('%c') gives %c\n", c, _toupper ((int) c));
	printf ("toascii ('%c') gives %c\n", c, toascii ((int) c));

	return (0);
}
