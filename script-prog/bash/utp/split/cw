#! /bin/sh
#
###	cx, cw, c-w -- Make files executable, writable, unwritable
###	Usage: cmd file [files...]
##
##	cx and cw execute chmod +x and chmod +w, respectively, on all files
##	listed on the command line.  c-w executes chmod -w.

case "$0" in
*cx)	chmod +x "$@" ;;
*cw)	chmod +w "$@" ;;
*c-w)	chmod -w "$@" ;;
*)	echo "$0 aborting: I can't run myself!" 1>&2; exit 1 ;;
esac
