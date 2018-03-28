/* readslow:  keep reading, waiting for more */
#define	SIZE	512	/* arbitrary */

main()
{
	char buf[SIZE];
	int n;

	for (;;) {
		while ((n = read(0, buf, sizeof buf)) > 0)
			write(1, buf, n);
		sleep(10);
	}
}
