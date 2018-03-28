#! /bin/sh
###	! - run a command, store output in a temporary file, output filename
###	Typical usage: comm `! sort file1` `! sort file2`
##
##	Program idea from article on USENET net.unix-wizards, 24 Sep 1986,
##	by Jason Zions of Hewlett-Packard Colorado Networks, jason@hpcnoe.UUCP.
##	Answer to request by Arthur David Olson, ado@elsie.UUCP, for something 
##	that worked this way.
##
##	Problem: doesn't clean up after itself; should delete temp files.
##	Answer (dirty): make it start a background process to sleep and rm.
##	Or, make the temp files in a directory that cron cleans up every day.
##
##	If your shell (e.g. bash) can do process substitution, use it instead.

temp=/tmp/bang$$

case $# in
0)	echo "Usage: `basename $0` command [args]" 1>&2
	echo $temp	# We hope command will fail when it can't open this file
	exit 1
	;;
*)	"$@" > $temp
	echo $temp
	;;
esac
