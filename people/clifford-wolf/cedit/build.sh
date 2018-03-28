#!/bin/sh

if ! qmake --version | grep -q '^Using Qt version 4'; then
	echo "Can't find qmake or it is not set up for building with Qt 4.." >&2
	exit 1
fi

set -ve
gcc -o wrindent -Wall -DWRINDENT_MAIN -Os wrindent.c
perl templates.pl templates_*.txt > templates.cpp
qmake
make
