#!/bin/bash
#
#  indt.sh - simple indent wrapper script for calling indent as filter
#  Copyright (C) 2007  Clifford Wolf <clifford@clifford.at>
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  This script uses the indentation of the first input line as indentation
#  for the entire output. This is usefull for indenting code blocks in an
#  editor such as vi: simply indent the first line and then pass the block
#  thru this script, e.g. using the ex '!' statement.
#
#  E.g. add following line to your ~/.vimrc file for indenting
#  a selected piece of C-code (via <V>) using <F12>:
#  map <F12> :!indt<CR>

tmpfile=`mktemp`
cat > $tmpfile
init_indent="`head -n1 $tmpfile | sed -e 's,[^ \t].*,,'`"
indent -npro -kr -i8 -ts8 -l500 -lc500 -ss -ncs -cp1 < $tmpfile | sed -e "s,^,$init_indent,"
rm $tmpfile

