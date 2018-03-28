#!/bin/sh

INTERACTIVE=0
RUNNING=1
while [[ $RUNNING -ne 0 ]]
do
	case $1 in
		-i)
			INTERACTIVE=1
			shift
			;;
		*)
			RUNNING=0
			;;
	esac
done

if [[ $# -lt 1 || $# -gt 2 ]]
then
	echo "Syntax:  $0 [-i] string [filename]"
	exit 1
fi

if [[ $# -eq 2 ]]
then
	# Close stdin
	0<&-
	if [[ ! -e $2 ]]
	then
		echo "Input file $2 does not exist!"
		exit 2
	fi

	# Attach file to standard input
	exec 0< $2
	if [[ $? -ne 0 ]]
	then
		echo "Couldn't open $2!"
		exit 3
	fi
fi

LINENUM=0
STARTED=0
while read LINE
do
	if [[ ${INTERACTIVE} -ne 0 ]]
	then
		LINENUM=$((LINENUM+1))

		if [[ $LINENUM -eq $(((LINENUM/1000)*1000)) ]]
		then
			echo -en "\rRead ${LINENUM} lines..." >&2
		fi
	fi

	LINE2=${LINE/$1}

	if [[ ${#LINE2} -ne ${#LINE} ]]
	then
		STARTED=1
		echo -n > /tmp/$$-log
	fi

	if [[ $STARTED -eq 1 ]]
	then
		echo ${LINE} >> /tmp/$$-log
	fi
done

if [[ $INTERACTIVE -ne 0 ]]
then
	echo
fi

cat /tmp/$$-log && rm /tmp/$$-log
exit $?
