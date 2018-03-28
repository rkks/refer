#!/bin/sh
# proxynotify demo

# set our "To:"
TO=schampeo@hesketh.com

# get our mx function
. $HOME/lib/mx.sh

function getip() {
        for host
        do
                dig in host $host |\
				grep -v ^\; |\
				grep 'A' |\
				grep $host |\
				cut -f5
        done
}

if [ -z "$1" ] 
then 
    echo "Usage: $0 [file]"
    exit 1
elif [ -r "$1" ]
then
    echo "found a file and it is readable"
else 
    echo "file $1 not readable or does not exist"
    exit 1
fi

for domain in `cat "$1"`
do
	echo "processing $domain"
	themx=`mx $domain` 
	echo "MX for $domain is '$themx'"
	if [ ! -z $themx ]
	then 
		mail -s "proxy" postmaster@$themx < formletter
	else
		echo "couldn't find MX for $domain,"
		echo "mailing direct-to-IP instead."
		theip=`getip $domain`
		echo "IP for $domain is '$theip'"
		if [ ! -z $theip ]; then
			mail -s "proxy" postmaster@$theip < formletter
		else
			echo "giving up, can't find anyone to notify"
			echo "$domain" >> /tmp/proxybadlist.$$
		fi
	fi
done
if [ -f /tmp/proxybadlist.$$ ]; then
	mail -s "bad proxies" $TO </tmp/proxybadlist.$$
	rm /tmp/proxybadlist.$$
fi
