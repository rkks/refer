#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main (void)
{
	char *pass;
	char *salt;
	char *secret;

	pass = getpassphrase ("Password: ");
	salt = "RT";
	secret = crypt (pass, salt);

	printf ("Password = \"%s\"\n", pass);
	printf ("Salt = \"%s\"\n", salt);
	printf ("Secret = \"%s\"\n", secret);

	return (0);
}
