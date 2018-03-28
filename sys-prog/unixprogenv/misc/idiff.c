/* idiff:  interactive diff */

#include <stdio.h>
#include <ctype.h>
char	*progname;
#define	HUGE	10000	/* large number of lines */

main(argc, argv)
	int argc;
	char *argv[];
{
	FILE *fin, *fout, *f1, *f2, *efopen();
	char buf[BUFSIZ], *mktemp();
	char *diffout = "idiff.XXXXXX";

	progname = argv[0];
	if (argc != 3) {
		fprintf(stderr, "Usage: idiff file1 file2\n");
		exit(1);
	}
	f1 = efopen(argv[1], "r");
	f2 = efopen(argv[2], "r");
	fout = efopen("idiff.out", "w");
	mktemp(diffout);
	sprintf(buf,"diff %s %s >%s",argv[1],argv[2],diffout);
	system(buf);
	fin = efopen(diffout, "r");
	idiff(f1, f2, fin, fout);
	unlink(diffout);
	printf("%s output in file idiff.out\n", progname);
	exit(0);
}

idiff(f1, f2, fin, fout)	/* process diffs */
	FILE *f1, *f2, *fin, *fout;
{
	char *tempfile = "idiff.XXXXXX";
	char buf[BUFSIZ], buf2[BUFSIZ], *mktemp();
	FILE *ft, *efopen();
	int cmd, n, from1, to1, from2, to2, nf1, nf2;

	mktemp(tempfile);
	nf1 = nf2 = 0;
	while (fgets(buf, sizeof buf, fin) != NULL) {
		parse(buf, &from1, &to1, &cmd, &from2, &to2);
		n = to1-from1 + to2-from2 + 1; /* #lines from diff */
		if (cmd == 'c')
			n += 2;
		else if (cmd == 'a')
			from1++;
		else if (cmd == 'd')
			from2++;
		printf("%s", buf);
		while (n-- > 0) {
			fgets(buf, sizeof buf, fin);
			printf("%s", buf);
		}
		do {
			printf("? ");
			fflush(stdout);
			fgets(buf, sizeof buf, stdin);
			switch (buf[0]) {
			case '>':
				nskip(f1, to1-nf1);
				ncopy(f2, to2-nf2, fout);
				break;
			case '<':
				nskip(f2, to2-nf2);
				ncopy(f1, to1-nf1, fout);
				break;
			case 'e':
				ncopy(f1, from1-1-nf1, fout);
				nskip(f2, from2-1-nf2);
				ft = efopen(tempfile, "w");
				ncopy(f1, to1+1-from1, ft);
				fprintf(ft, "---\n");
				ncopy(f2, to2+1-from2, ft);
				fclose(ft);
				sprintf(buf2, "ed %s", tempfile);	
				system(buf2);
				ft = efopen(tempfile, "r");
				ncopy(ft, HUGE, fout);
				fclose(ft);
				break;
			case '!':
				system(buf+1);
				printf("!\n");
				break;
			default:
				printf("< or > or e or !\n");
				break;
			}
		} while (buf[0]!='<' && buf[0]!='>' && buf[0]!='e');
		nf1 = to1;
		nf2 = to2;
	}
	ncopy(f1, HUGE, fout);	/* can fail on very long files */
	unlink(tempfile);
}

parse(s, pfrom1, pto1, pcmd, pfrom2, pto2)
	char *s;
	int *pcmd, *pfrom1, *pto1, *pfrom2, *pto2;
{
#define a2i(p) while (isdigit(*s)) p = 10*(p) + *s++ - '0'

	*pfrom1 = *pto1 = *pfrom2 = *pto2 = 0;
	a2i(*pfrom1);
	if (*s == ',') {
		s++;
		a2i(*pto1);
	} else
		*pto1 = *pfrom1;
	*pcmd = *s++;
	a2i(*pfrom2);
	if (*s == ',') {
		s++;
		a2i(*pto2);
	} else
		*pto2 = *pfrom2;
}

nskip(fin, n)	/* skip n lines of file fin */
	FILE *fin;
{
	char buf[BUFSIZ];

	while (n-- > 0)
		fgets(buf, sizeof buf, fin);
}

ncopy(fin, n, fout)	/* copy n lines from fin to fout */
	FILE *fin, *fout;
{
	char buf[BUFSIZ];

	while (n-- > 0) {
		if (fgets(buf, sizeof buf, fin) == NULL)
			return;
		fputs(buf, fout);
	}
}

#include "efopen.c"
