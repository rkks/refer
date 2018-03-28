#include "ssp.h"

extern int ssp_system (const char *string);

int main (void)
{
	int status;

	if ((status = ssp_system ("date")) == -1)
		err_msg ("system failed");
	print_term_status (status);

	if ((status = ssp_system ("bad_command")) == -1)
		err_msg ("system failed");
	print_term_status (status);

	if ((status = ssp_system ("date; exit 42")) == -1)
		err_msg ("system failed");
	print_term_status (status);

	return (0);
}
