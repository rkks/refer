#!/bin/sh
# xwrist - open xmessage window with "take a break" message.
# Repeats every 10 minutes by default (else # of minutes in first cmdline arg).

# Complain and quit if we aren't running X:
: ${DISPLAY?"unset? I only work under the X Window System"}

myname=`basename $0`
# Set for your display and font:
geometry=600x200
# Text width for your fmt (some use -w<num>, some -l<num>, some -<num>, etc):
fmtarg="-w40"	# NOT USED NOW...
# Set window title:
title="$myname: take a break!"

case $# in
0)	delay=600 ;;
1)	delay=`expr "$1" \* 60`
	case "$delay" in
	[1-9][0-9]*) ;;
	"")	echo "$myname: bad repeat amount?" 1>&2; exit 1 ;;
	esac
	;;
*)	echo "Usage: $myname [repeat in minutes]" ;;
esac
    
while sleep $delay
do
	# If you press return or click okay, xmessage returns 1, loop repeats.
	# If you click quit, xmessage returns 0 and we exit.
	# (If you do a lot of typing, you might accidentally press return
	# as the xmessage pops up.  To avoid that, remove "-default okay".)
	#
	# You can change message... a stock quote (from lynx -dump | sed),
	# the time (from date), how long til quittin' time, or whatever.
	if xmessage -nearmouse -geometry $geometry -title "$title" \
		-buttons okay:1,quit:0 -default okay \
		"`/usr/games/fortune`"
#		"`/usr/games/fortune | fmt $fmtarg`"
	then exit 0
	fi
done
