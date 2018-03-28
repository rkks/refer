#!/bin/sh
# del - remove files interactively; if more than 3, prompt only once
# Usage: del file [files...]

case $# in
0)     echo "`basename $0`: you didn't say which file(s) to delete"; exit 1;;
[123]) /bin/rm -i "$@" ;;
*)     echo "$*"
       echo do you want to delete these files\?
       read a
       case "$a" in
       [yY]*) /bin/rm "$@" ;;
       esac
       ;;
esac
