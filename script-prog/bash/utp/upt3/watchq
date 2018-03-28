#!/bin/sh
#
# watchq - "daemon" script that watches printer queue(s) for errors

# Change these next variables to suit your situation
watch=/usr/local/lib/watchqs   # File that holds names of the queues to watch
writeto=lisa                   # User who gets notices about printer
temp=/tmp/WATCHQ$$             # Temporary file that holds the output of lpq

queues="`cat $watch`"          # Put list of queue names in $queues
trap 'queues="`cat $watch`"' 1 # Reset $queues if we get a SIGHUP
trap 'rm -f $temp; exit' 0 15  # Clean up temp file when killed

# Loop forever (until someone kills script):
while :
do
    # for each queue
    for queue in $queues
    do
        # Query the printer and look for errors:
        lpq -P$queue >$temp
        if egrep '(out of paper|error|warning)' $temp > /dev/null
        then
            # If there were errors, tell $writeto about them:
            echo "PRINTER QUEUE $queue:" | cat - $temp | write $writeto
        fi
    done

    # Wait 30 seconds before checking the printers again:
    sleep 30
done
