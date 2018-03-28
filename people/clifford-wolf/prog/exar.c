/*
 *  A tiny tool for creating ELF static binary self-extracting archives
 *
 *  Copyright (C) 2009  Clifford Wolf <clifford@clifford.at>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv)
{
	FILE *c, *sh;
	char buffer[1024];
	struct stat stb;
	int i, j;

	if (argc <= 3) {
		fprintf(stderr, "Usage: exar <output-file> <inputfile(s)>\n");
		fprintf(stderr, "       sh <output-file>.sh\n");
		return 1;
	}

	snprintf(buffer, 1024, "%s.c", argv[1]);
	c = fopen(buffer, "wt");

	snprintf(buffer, 1024, "%s.sh", argv[1]);
	sh = fopen(buffer, "wt");

	fprintf(c, "#include <stdio.h>\n");
	fprintf(c, "int main() {\nFILE *fp;\n");

	fprintf(sh, "EXAR_CC=${EXAR_CC:-gcc}\n");
	fprintf(sh, "EXAR_OO=${EXAR_OO:-elf32-i386}\n");
	fprintf(sh, "EXAR_OB=${EXAR_OB:-i386}\n");

	for (i = 2; i < argc; i++)
	{
		strcpy(buffer, argv[i]);
		for (j=0; buffer[j]; j++)
			if (buffer[j] == '/')
				fprintf(c, "mkdir(\"%.*s\");\n", j, buffer);

		for (j=0; buffer[j]; j++)
			if (!(buffer[j] >= 'a' && buffer[j] <= 'z') &&
					!(buffer[j] >= 'A' && buffer[j] <= 'Z') &&
					!(buffer[j] >= '0' && buffer[j] <= '9'))
				buffer[j] = '_';

		stat(argv[i], &stb);
		fprintf(c, "fp=fopen(\"%s\", \"w\");\n", argv[i]);
		fprintf(c, "extern char _binary_%s_start[];\n", buffer);
		fprintf(c, "fwrite(_binary_%s_start, %u, 1, fp);\n", buffer, (unsigned int)stb.st_size);
		fprintf(c, "fclose(fp);\n");
		fprintf(c, "chmod(\"%s\", 0%o);\n", argv[i], stb.st_mode & 07777);
		fprintf(sh, "objcopy -I binary -O $EXAR_OO -B $EXAR_OB \"%s\" \"%s_%d.o\"\n", argv[i], argv[1], i-1);
	}

	fprintf(sh, "$EXAR_CC -static -o \"%s\" \"%s.c\"", argv[1], argv[1]);
	for (i = 2; i < argc; i++)
		fprintf(sh, " \"%s_%d.o\"", argv[1], i-1);
	fprintf(sh, "\n");

	fprintf(sh, "rm -f");
	for (i = 2; i < argc; i++)
		fprintf(sh, " \"%s_%d.o\"", argv[1], i-1);
	fprintf(sh, "\n");

	fprintf(c, "return 0;\n");
	fprintf(c, "}\n");

	fclose(c);
	fclose(sh);
	return 0;
}

