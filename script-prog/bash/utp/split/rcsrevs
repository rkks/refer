#! /bin/sh
###	rcsrevs - get all revision number of RCS file
###	Usage: rcsrevs [rlog options] file
##
##	rcsrevs gives a list of all revision numbers of an RCS file.
##	By default, rcsrevs gives all revision numbers; you can cut
##	this down with a -rrange option.  Other rlog options work too.

#
# Run rlog to get revision list.  If -r option, let rlog pick range.
# Use sed to look for the lines after the row of dashes; grab the numbers:
#  ----------------------------
#>  revision 1.3        
#   date: 90/04/24 21:54:19;  author: jdpeek;  state: Exp;  lines added/del: 2/2
#  	...
#  ----------------------------
#>  revision 1.2        
#   date: 85/10/20 16:32:54;  author: jerryp;  state: Exp;  lines added/del: 4/3
#  	...
#
rlog "$@" |
sed -n '
:loop
/^----------------------------$/ {
	n
	/^revision [0-9]/ {
		s/revision \([0-9.][0-9.]*\).*/\1/
		H
		b
	}
	b loop
}
$ {
	g
	s/^\n//
	p
}'
