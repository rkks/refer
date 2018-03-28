#!/bin/sh
# commer - label columns in "comm" output
# Usage: commer [-i] file1 file2
#   -i option indents output lines into columns as "comm" does
#
# Note that script WILL FAIL if any input lines start with a TAB.

case "$1" in
-i) indent='&'; shift ;;
-*|"") echo "Usage: `basename $0` [-i] file1 file2" 1>&2; exit 1 ;;
esac

# In "comm" output, column 1 (lines in file 1) has no leading TAB.
# Column 2 (lines in file 2) has one leading TAB.
# Column 3 (lines in both files) has two leading TABs.
# Search for these tabs and use them to label lines.
# (You could replace ONE and TWO with the filenames $1 and $2)
comm "$1" "$2" |
sed "{
/^		/ {s//BOTH>$indent/; b}
/^	/ {s// ONE>$indent/; b}
s/^/ TWO>/
}"
