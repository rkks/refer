#!/bin/sh
#
#  bked.sh - call the editor but create a backup file first
#  Copyright (C) 2007  Clifford Wolf <clifford@clifford.at>
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.

for x; do
	if [ "${x#-}" != "$x" ]; then
		continue
	fi
	if [ ! -f $x.orig ]; then
		touch $x
		cp -v $x $x.orig
	fi
done

${EDITOR:-vi} "$@"

