#include <stdio.h>
#include <stdlib.h>
#include <crypt.h>
#include <pwd.h>
#include "ssp.h"

extern char *crypt_gensalt (const char *salt, const struct passwd *userinfo);

int main (int argc, char **argv)
{
	char *pass;
	char *salt;
	char *secret;
	struct passwd *pwd;

	if (argc !=2)
		err_quit ("Usage: new_crypt username");

	if ((pwd = getpwnam (argv [1])) == NULL)
		err_quit ("%s: No such user", argv [1]);

	pass = getpassphrase ("Password: ");
	salt = crypt_gensalt (NULL, pwd);
	secret = crypt (pass, salt);

	printf ("Password = \"%s\"\n", pass);
	printf ("Salt = \"%s\"\n", salt);
	printf ("Secret = \"%s\"\n", secret);

	return (0);
}
