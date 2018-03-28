# checkmail:  watch mailbox for growth

PATH=/bin:/usr/bin
MAIL=/usr/spool/mail/`getname`	# system dependent

t=${1-60}

x="`ls -l $MAIL`"
while :
do
	y="`ls -l $MAIL`"
	echo $x $y
	x="$y"
	sleep $t
done | awk '$4 < $12 { print "You have mail" }'
