#! /bin/sh
# runsed - run a sed script on file(s), replace originals with edited versions

# Script looks nicer with tabstops set at 4 (in vi, :se ts=4)

myname="`basename $0`"
script=sedscr
stat=1                  # Default exit status (reset to 0 before normal exit)
temperr=/tmp/RUNSEDe$$
tempout=/tmp/RUNSEDo$$
trap 'rm -f $temperr $tempout; exit $stat' 0
trap 'echo "$myname: Interrupt!  Cleaning up..." 1>&2' 1 2 15

if [ ! -r $script -o ! -f $script ] ; then
	echo "$myname quitting: $script isn't readable or isn't a file." 1>&2
	exit
fi

case $# in
0) echo "Usage: $myname file-to-edit [files-to-edit]" 1>&2; exit ;;
esac

for x
do
	echo "$myname: editing $x: " 1>&2
	if [ "$x" = $script ] ; then
		echo "$myname: not editing $script!" 1>&2
	elif [ ! -s "$x" -o ! -f "$x" ]; then 
		echo "$myname: original $x is empty or not a file." 1>&2
	elif [ ! -w "$x" ] ; then
		echo "$myname: can't write $x -- skipping..." 1>&2
	else
		# If get here, run sed.  To keep source file's permissions
		# and owner the same, don't overwrite it until the end --
		# and use "cat > file" to write the file in place.
		sed -f $script "$x" >$tempout 2>$temperr
		if [ $? -ne 0 -o -s $temperr ] ; then
			cat $temperr 1>&2
			echo "$myname quitting: 'sed -f $script $x' bombed!?!" 1>&2
			exit
		elif [ -s $tempout ] ; then
			if cmp -s "$x" $tempout; then
				echo "$myname: $x file not changed." 1>&2
			else
				if /bin/cat $tempout > $x
				then echo "$myname: done with $x" 1>&2
				else
					echo "$myname quitting: problem replacing $x?" 1>&2
					exit
				fi
			fi
		else 
			echo "$myname quitting: sed produced an empty file - check your $script." 1>&2
			echo "$myname: didn't change $x" 1>&2
			exit
		fi
	fi
done

echo "$myname: all done" 1>&2
stat=0
exit
