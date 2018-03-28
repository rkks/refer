#!/bin/sh

# An array push for BASH.

ARR=( A B C )
function push
{
	local LEN=`eval echo "\\${#${1}[@]}"` A="$1"
	shift
	while [ "$#" -gt 0 ]
	do
		eval "${A}[$LEN]=${1}"
		LEN=$((LEN+1))
		shift
	done
}

push ARR D E F G
set | grep ARR
