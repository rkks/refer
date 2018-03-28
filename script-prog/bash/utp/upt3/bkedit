#! /bin/sh
#
##	bkedit - make backup copy of a file, then edit it with vi
##	Usage: bkedit file

if cp "$1" "$1.bak"
then
    vi "$1"
    exit   # USE STATUS FROM vi
else
    echo "bkedit quitting: can't make backup?" 1>&2
    exit 1
fi
