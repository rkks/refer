#!/bin/sh
# showenv - show environment of a process, by PID, by reading from /proc
# If second argument given, show just that one environment variable.

f=/proc/$1/environ

if [ ! -r "$f" ]; then
    echo "`basename $0`: can't access process $1" 1>&2
    exit 1
fi

case $# in
1) tr '\000' '\012' < $f | sort ;;
2) tr '\000' '\012' < $f | grep "^$2=" ;;
*) echo "Usage: `basename $0` pid [envariable-name]" 1>&2; exit 1 ;;
esac
