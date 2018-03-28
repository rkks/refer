#!/bin/sh

function getc
{
	eval "$1=\"\""

	if [[ ! -z "$GETC_LAST" ]]
	then
		eval "$1=\"$GETC_LAST\""
		GETC_LAST=""
		return 0
	fi

	read -n 1 $1
	return $?
}

function ungetc
{
	GETC_LAST="$1"
}

# A simple shell implementation of scanf.  It does not support all features.
# It currently handles the following combinations:
#   %d -- Reads a decimal integer
#   %s -- Reads a string up to the next IFS character
#   %x -- Reads a hexadecimal integer
#   %% -- Matches a literal %
# Everything else matches the literal characters.
function scanf
{
	local _S=$1
	local RVAL=0
	local _N=0
	local MODE
	local _C
	local _I
	local OLD_IFS

	# We've stored cmdstr in _S, so make parameters start at $1
	shift

	# Loop through command string
	while [[ "$_N" -lt ${#_S} ]]
	do
		case $MODE in
		# Cleanup after reading in a variable
		end)
			if [[ -z "$_I" ]]
			then
#				echo "Nothing read"
				return $RVAL
			fi

#			echo "Read '$_I'"
#			echo eval "$1=\"$_I\""
			eval "$1=\"$_I\"" ; shift
			_N=$((_N+1))
			RVAL=$((RVAL+1))
			MODE=""
			_I=""
			;;

		# Read in a hex number digit by digit
		x)	#echo "Reading hex"

			getc _C
			case "$_C" in
			`echo -en "[${IFS}]"`)
				if [[ ! -z "$_I" ]]
				then
					MODE="end"
				fi
				;;

			[0-9])	true	;;
			[aA])	_C=10	;;
			[bB])	_C=11	;;
			[cC])	_C=12	;;
			[dD])	_C=13	;;
			[eE])	_C=14	;;
			[fF])	_C=15	;;
			*)	ungetc "$_C"
				_C=""
				MODE="end"
				;;
			esac

			if [[ ! -z "$_C" ]]
			then
				if [[ -z "$_I" ]] ; then _I=0 ; fi
				_I=$(((_I*16)+_C))
			fi
			;;

		# Read in a string digit by digit
		s)	#echo "Reading string"
			OLD_IFS="${IFS}" ; IFS=""
			getc _C
			IFS="${OLD_IFS}"

			case "$_C" in
			"")	#echo "EOF"
				MODE="end"
				;;

			`echo -en "[${IFS}]"`)
				#echo "Separator"
				MODE="end"				
				;;

			*)	#echo "Adding '$_C' to string"
				_I="$_I$_C"
				;;
			esac

			;;

		# Read in a decimal integer, digit by digit
		d)	#echo "Reading decimal integer"
			getc _C

			case "$_C" in
			[0-9])	if [[ -z "$_I" ]] ; then	_I=$_C
				else _I=$(((_I*10)+_C)) ; 	fi
				;;

			`echo -en "[${IFS}]"`)
				if [[ ! -z "$_I" ]]
				then
					MODE="end"
				fi
				;;

			*)	ungetc "$_C"
				MODE="end"
				;;
			esac
			;;

		# Match whitespace/IFS seperator
		_)	#echo "Matching seperators"

			OLD_IFS="${IFS}" ; IFS=""
			read -n 1 _C
			IFS="${OLD_IFS}"

			case "$_C" in
			`echo -en "[${IFS}]"`)
				;;

			"")	#echo "EOF"
				MODE="end"
				;;

			*)	ungetc "$_C"
				MODE=""
				;;
			esac

			if [[ -z "$_C" ]]
			then
				MODE="end"
			fi

			I="$_I$_C"			
			;;

		# Choose which kind of thing to scan for
		%)	case "${_S:$_N:1}" in
			[dsx])	#echo "Switching to ${_S:$_N:1}"
				MODE="${_S:$_N:1}"
				;;

			[%])	#echo "Match literal %"
				read -n 1 _C
				if [[ "$_C" != "%" ]]
				then
					#echo "Failed to match literal %"
					return $RVAL
				fi

				MODE=""
				_N=$((_N+1))
				;;

			*)	return $RVAL
				;;
			esac
			;;

		# Match literal characters or parse beginning of % token
		*)	case "${_S:$_N:1}" in
			%)	#echo "Beginning token parse"
				MODE="%"
				;;

			" ")	#echo "Reading whitespace/separators"
				MODE="_"
				;;

			*)	#echo "Waiting for ${_S:$_N:1}"
				getc _C

				if [[ "$_C" != "${_S:$_N:1}" ]]
				then
					return $RVAL
				fi
				;;
			esac			

			_N=$((_N+1))
			;;
		esac
	done

	return $RVAL
}

# Some examples
scanf "%d.%d.%d.%d/%d" A B C D M <<< 192.168.0.1/24

scanf "%%%d" Q <<< %1

# Scanning hexadecimal
scanf '0x%x' X <<< 0xfc

# As per 'read', blank IFS tells it not to break strings on anything.
# Note that spaces in the command string match IFS characters, not space.
IFS="" scanf "%s" STR <<< "a b c d e f g"
echo $STR

