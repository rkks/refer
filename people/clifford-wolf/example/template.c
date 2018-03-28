/*
 *  <Program Title Here>
 *
 *  Copyright (C) YYYY  Clifford Wolf <clifford@clifford.at>
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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void help(const char *progname)
{
	fprintf(stderr, "Usage: %s [-n] [-t nsecs]\n", progname);
	exit(1);
}

int main(int argc, char **argv)
{
	int opt;

	while ((opt = getopt(argc, argv, "nt:")) != -1)
	{
		switch (opt)
		{
		case 'n':
			printf("Got -n flag.\n");
			break;
		case 't':
			printf("nsecs = %d\n", atoi(optarg));
			break;
		default:
			help(argv[0]);
		}
	}

	while (optind < argc)
		printf("additional arg: %s\n", argv[optind++]);

	if (optind != argc)
		help(argv[0]);

	return 0;
}

