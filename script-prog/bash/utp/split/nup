#!/bin/bash
# We use /bin/bash because it sets $UID variable
#
# nup, ndown - run /sbin/if{up,down} ppp0; put flag on screen if under X

msg_at='-geometry 86x51+645+72'    # comment out for auto xmessage placement
pidfile=/tmp/.nup-pid

case "$0" in
*nup)	if [ -f $pidfile ]
	then
		echo "`basename $0`: lockfile $pidfile exists. Is modem already up?" 1>&2
		exit 1
	elif [ -n "$DISPLAY" ]
	then
		xmessage $msg_at -title 'MODEM UP' -fg white -bg red 'modem up' &
		# Store xmessage PID number in $pidfile:
		echo $! > $pidfile
	fi
	exec /sbin/ifup ppp0
	;;
*ndown)	if [ -n "$DISPLAY" -a -s $pidfile ]
	then
		# If xmessage process is still running with same PID,
		# kill it and remove $pidfile:
		pid=`cat $pidfile`
		case "`ps $pid`" in
		*xmessage*)
			kill $pid
			rm -f $pidfile
			;;
		esac
	fi
	exec /sbin/ifdown ppp0
	;;
*) echo "$0: aborting: I don't know how to run myself." 1>&2; exit 1 ;;
esac
