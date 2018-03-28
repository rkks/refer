#! /bin/sh
#
###	recomment - re-format a program's comment block, usually from "vi"
###	Usage (from 'vi'): !}recomment   (reads block, writes back in place)
#
##	IF YOU MAKE COMMENT BLOCKS LIKE THIS:
##		##
##		##	These are comment lines blah blah blah blah blah blah blah
##		##	More comment lines
##	OR LIKE THIS:
##		# These are
##		# comment lines blah blah blah blah blah blah blah blah blah
##	OR LIKE THIS:
##		* These are
##		* comment lines blah blah blah blah blah blah blah blah blah
##	(AND SO ON...)
##	IT CAN BE A HASSLE TO ADD MORE TEXT TO ONE OF THE LINES IN A BLOCK,
##	BECAUSE THE LINE CAN GET TOO LONG, WHICH REQUIRES YOU TO FOLD THAT
##	LINE ONTO THE NEXT LINE, WHICH MEANS YOU HAVE TO WORK AROUND THE
##	LEADING COMMENT CHARACTER(S).  YUCK.
##
##	recomment READS THE LINES YOU FEED ITS STANDARD INPUT.  IT LOOKS
##	AT THE FIRST LINE AND FIGURES OUT WHAT CHARACTERS YOU'RE USING
##	TO COMMENT THE LINE (SEE THE $cchar VARIABLE FOR A LIST, TYPICALLY
##	SPACES, TABS, #'S OR *'S).  THEN, IT STRIPS THOSE COMMENT CHARACTERS
##	OFF OF EACH LINE... FEEDS THE REMAINING BLOCK OF TEXT TO THE fmt
##	ULILITY, AND USES sed TO ADD THE COMMENT CHARACTERS AGAIN.
##
##	YOU USUALLY USE recomment FROM INSIDE vi, WITH COMMANDS LIKE:
##			!}recomment		(REFORMAT TO THE NEXT BLANK LINE)
##	OR		5!!recomment	(REFORMAT THIS LINE AND THE NEXT 4)
##
##	NORMALLY, recomment LETS fmt CHOOSE THE WIDTH OF THE COMMENT BLOCK
##	(72 CHARACTERS, I THINK).  TO GET ANOTHER WIDTH, YOU CAN EITHER:
##		* GIVE THE WIDTH ON THE COMMANDLINE, LIKE:
##			recomment -50
##		* SET AN ENVIRONMENT VARIABLE NAMED CBLKWID.  FOR EXAMPLE,
##		  IN C-SHELL, USE:
##			setenv CBLKWID 50
##	GIVE THE MAXIMUM WIDTH, IN CHARACTERS, FOR THE COMMENT TEXT.
##
##	recomment ISN'T PERFECT, BUT IT'S USUALLY MUCH BETTER THAN NOTHING.

umask 077	# MAKE $temp PRIVATE
temp=/tmp/RECOMMENT$$
trap 'rm -f $temp; exit' 0 1 2 15

cchars=' 	#*'		# LEADING CHARACTERS TO PRESERVE (INCL. SPACE, TAB)

case "$1" in
-[0-9]|-[0-9][0-9]|-[0-9][0-9][0-9]) widopt="$1" ;;
"")	# CHECK ENVARIABLE (SILENTLY IGNORE ERRORS SO DON'T SCREW UP TEXT):
	case "$CBLKWID" in
	[0-9]|[0-9][0-9]|[0-9][0-9][0-9]) widopt="-$CBLKWID" ;;
	esac
	;;
*)	# PRINT ERROR, THEN GIVE THEM TEXT BACK (SO vi WON'T DELETE LINES):
	echo "Usage: `basename $0` [-widthnum].  (Text output with no reformatting.)" 1>&2
	cat
	exit
	;;
esac
	
cat > $temp

# GET COMMENT CHARACTERS USED ON FIRST LINE; STORE IN $comment:
comment="`sed -n \"1s/^\([$cchars]*\).*/\1/p\" $temp`"
# GET NUMBER OF CHARACTERS IN COMMENT CHARACTER STRING:
cwidth=`expr "$comment" : '.*'`

# RE-FORMAT THE COMMENT BLOCK.  IF $widopt SET, USE IT:
colrm 1 $cwidth < $temp |		# STRIP OFF COMMENT LEADER FROM LINES
fmt $widopt |					# RE-FORMAT THE TEXT, AND
sed "s/^/$comment/"				# PUT THE COMMENT CHARACTERS BACK
