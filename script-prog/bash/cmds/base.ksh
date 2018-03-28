#! /usr/bin/ksh
##########################################################################
# Title      :	base.ksh - print number to different bases (KSH version)
# Author     :	Heiner Steven <heiner.steven@odn.de>
# Date       :	1995-03-07
# Requires   :	bc, ksh
# Category   :	Desktop
# SCCS-Id.   :	@(#) base.ksh	1.4 04/02/18
##########################################################################
# Description
#
# Changes
# 21.03.95 hs	fixed error occuring with 0xb as input (0.2)
# 14.05.96 hs	Rewritten for ksh. Speed increasement: 60x (1.0)
# 17.05.96 hs	Reset output base to 10 (1.1)
##########################################################################
PN=${0##*/}				# Program name
VER='1.4'
function Usage
{
    print -u2 "$PN - convert number to different bases, $VER (hs '96)
usage: $PN [number ...]
If no number is given, the numbers are read from standard input.
A number may be
    binary (base 2)		starting with 0b (i.e. 0b1100)
    octal (base 8)		starting with 0  (i.e. 014)
    hexadecimal (base 16)	starting with 0x (i.e. 0xc)
    decimal			otherwise (i.e. 12)"
    exit 1
}
function Msg
{
    typeset i
    for i
    do print -u2 "$PN: $i"
    done
}
function PrintBases
{
    typeset -u Number			# Convert to upper case (for "bc")
    integer Base
    typeset Dec Result
    # Determine base of the number
    for Number
    do
	case "$Number" in
	    (0B+([01]))				# binary
		Base=2	
		Number=${Number#0B} ;;
	    (0X+([0-9A-F]))			# hexadecimal
		Base=16
		Number=${Number#0X} ;;
	    (0+([0-7]))				# octal
		Base=8
		Number=${Number##0} ;;
	    ([1-9]*([0-9]))			# decimal
		Base=10;;
	    (*([0-9A-F])+([A-F])*([0-9A-F]))	# hexadecimal, no prefix
		Base=16;;
	    (*)
		Msg "illegal number $Number - ignored"
		continue;;
	esac
	# Convert to decimal number. Do not use ksh, because
	# there is an upper limit to the maximum integer size.
	set -e				# Terminate at errors
	print -p "ibase=$Base; $Number; ibase=A"
	read -p Dec
	# Let "bc" do the conversions. "bc" will
	# echo "GNUELPF" for the last line.
	# Assign the result of c() to a dummy to prevent
	# "bc" from printing the "result" of the function.
	print -p "d=c($Dec)"
	# Read lines from "bc", build one line
	Result=
	while read -p && [[ $REPLY != GNUELPF ]]
	do
	    Result="${Result:+$Result	}$REPLY"
	done
	set +e
	print "$Result"
    done
}
if (( $# > 0 ))
then
    while getopts :h Opt
    do
	case "$1" in
	    (h)	Usage;;
	    (*)	Usage;;
	esac
    done
    shift OPTIND-1
fi
bc |&					# Start "bc" as co-process
# DEBUG:
# tee in | bc | tee out |&
# Define a "bc" function for base conversion, because "bc" would
# run out of "string space" for large input otherwise.
# The "GNUELPF" string tells PrintBases() that the end of
# the calculations is reached.
set -e					# Terminate at errors
print -p '
/* Convert x to some other bases */
define c (x) {
obase=16; "hex="; x
obase=10; "dec="; x
obase=8;  "oct="; x
obase=2;  "bin="; x
obase=A
"GNUELPF
"
}
'
set +e
if (( $# > 0 ))
then
    PrintBases "$@"
else					# read from stdin
    while read Line
    do
	PrintBases $Line
    done
fi
