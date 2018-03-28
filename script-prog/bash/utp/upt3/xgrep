#! /bin/sh
#  xgrep - print just the matches on separate lines
#  Written by Greg Ubben, DoD, 3 Feb 1994

if [ $# -eq 0 ]; then
	echo "Usage:  $0 pattern [file...]" >&2
	exit 2
fi

re=$1; shift
x=`echo \\\02`			# unlikely ^B will occur in the input
#x=`echo 2 | tr 2 \\\02`	# use if your echo doesn't change \02 to ^B

sed "
	\\$x$re$x!d
	s//$x&$x/g
	s/[^$x]*$x//
	s/$x[^$x]*$x/\\
/g
	s/$x.*//
" "$@"
