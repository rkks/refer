#include	"lprps.h"

/* Write the number of pages, hostname, and loginname to the
 * accounting file.  This function is called by main() at the end
 * (if all was OK, by printer_flushing(), and by handle_intr() if
 * an interrupt is received. */

void
do_acct(void)
{
	FILE	*fp;

	if (end_page > start_page &&
		acct_file != NULL &&
		(fp = fopen(acct_file, "a")) != NULL) {
			fprintf(fp, "%7.2f %s:%s\n",
			    		(double)(end_page - start_page),
			    		hostname, loginname);
			if (fclose(fp) == EOF)
				log_sys("do_acct: fclose error");
	}
}
