#!/bin/sh
#
#  xdiff.sh - search for *.orig files and create unified diffes
#  Copyright (C) 2007  Clifford Wolf <clifford@clifford.at>
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.

for x in $( { find "$@" -name '*.orig' -type f; find "$@" -maxdepth 0 -type f; } | sed 's,\.orig$,,' | sort -u; )
do
	if [ -f "$x" -a -f "$x.orig" ]; then
		diff -ud "$x.orig" "$x"
	fi
done

