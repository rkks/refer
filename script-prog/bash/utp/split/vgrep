#!/bin/sh
# 
# vgrep - create a list of files that don't contain a specified pattern

case $# in
0|1) echo "Usage: `basename $0` pattern file [files...]" 1>&2; exit 2 ;;
2)  # Given a single filename, grep returns a count with no colon or name.
    grep -c -e "$1" "$2" | sed -n "s|^0\$|$2|p"
    ;;
*)  # With more than one filename, grep returns "name:count" for each file.
    pat="$1"; shift
    grep -c -e "$pat" "$@" | sed -n "s|:0\$||p"
    ;;
esac
