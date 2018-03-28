#include <stdio.h>
#include <locale.h>
#include <libintl.h>

int main (void)
{
	setlocale (LC_ALL, "");
	textdomain ("hello");
	bindtextdomain ("hello", "locale");

	printf ("%s\n", gettext ("Hello, World!"));

	return (0);
}
