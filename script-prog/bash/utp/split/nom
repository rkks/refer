#!/bin/sh
#
###	nom - return names of non-matching files from current directory
###	Usage: nom files     (example:  lpr `nom *.ms`)
##
## nom takes filenames (usually, expanded by the shell) from its command line.
## It outputs all filenames in the current directory that *don't* match.
## Note: nom doesn't know about files whose names begin with "."
##
## Examples:
## To get the names of all files that *don't* end with ".ms":
##	% nom *.ms
## To edit all files whose names don't have any lower-case letters:
##	% vi `nom *[a-z]*`
## To copy all files to a directory named Backup (except Backup itself):
##	% cp `nom Backup` Backup

temp=/tmp/NOM$$
stat=1	# error exit status (set to 0 before normal exit)
trap 'rm -f $temp; exit $stat' 0 1 2 15

# Must have at least one argument, and all have to be in current directory:
case "$*" in
"")	echo Usage: `basename $0` pattern 1>&2; exit ;;
*/*)	echo "`basename $0` quitting: I can't handle '/'s." 1>&2; exit ;;
esac

# ls gives sorted file list. -d=don't enter directories, -1=one name/line:
ls -d ${1+"$@"} > $temp   # Get filenames we don't want to match
ls -1 | comm -23 - $temp  # Compare to current directory; output names we want
stat=0
