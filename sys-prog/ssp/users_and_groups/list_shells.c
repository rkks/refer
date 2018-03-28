#include <stdio.h>

extern char *getusershell (void);
extern void setusershell (void);
extern void endusershell (void);

int main (void)
{
	char *shell;

	setusershell ();

	while ((shell = getusershell ()) != NULL)
		printf ("%s\n", shell);

	endusershell ();

	return (0);
}
