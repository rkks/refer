#! /usr/bin/ksh
##########################################################################
# Shellscript:	cgitemplate.ksh - CGI script template
# Version    :	1.2
# Author     :	Heiner Steven <heiner.steven@odn.de>
# Date       :	2001-08-26
# Requires   :	ksh, urlgetopt
# Category   :	CGI
# SCCS-Id.   :	@(#) cgitemplate.ksh	1.2 04/02/18
##########################################################################
# Description
#
# Note
#    o	Needs the following, non-standard scripts and prograrms:
#	    urlgetopt (http://www.shelldorado.com/scripts/cmds/urlgetopt)
##########################################################################
PN=${0##*/}			# Program name
VER='1.2'
Debug=false
#Debug=true
# Append directories with local commands to the command search path:
PATH=$PATH:/home/heiner/cmds
export PATH
function Usage {
    $iscgi && print "<pre>"
    print -u2 "$PN - short description, $VER"
    $iscgi && print "</pre>"
    exit 1
}
function Msg {
    $iscgi && print "<pre>"
    print -u2 "$PN:" "$@"
    $iscgi && print "</pre>"
}
function Fatal { Msg "$@"; exit 1; }
# Redirect error messages to standard output, where we can see them
$Debug && exec 2>&1
# The variable "iscgi" can be used to determine, if the script was
# invoked as a CGI program, or from a command line
if [[ -n $REQUEST_METHOD ]]
then iscgi=true
else iscgi=false
fi
if $iscgi
then
    if $Debug
    then print "Content-type: text/plain\r\n\r"
    else print "Content-type: text/html\r\n\r"
    fi
    case "$REQUEST_METHOD" in
    	(GET)
	    # CGI Arguments are in QUERY_STRING
	    ;;
	(POST)
	    # First line from standard input contains CGI arguments
	    read -r query_string || exit 2
	    # If QUERY_STRING is non-empty, append the string read from
	    # standard input. This is useful for debugging purposes
	    QUERY_STRING=${QUERY_STRING:+$QUERY_STRING&$query_string}
	    ;;
	(*)
	    Fatal "ERROR: unknown request method: $REQUEST_METHOD"
	    ;;
    esac
    # Decode CGI arguments, sent script environment variables.  Each
    # variable will get the prefix FORM_. If a form variable has the name
    # "email", the corresponding script variable will be named FORM_email
    eval "$(urlgetopt -p FORM_ -l $QUERY_STRING)"
else
    while getopts :h opt
    do
	case "$opt" in
	    # your flags here
	    #f)	argument=$OPTARG;;
	    h)	Usage;;
	    ?)	Usage;;
	esac
    done
    shift OPTIND-1
fi
# At this point, the script's arguments are processed, and the script should
# write HTML code to standard output
print "DEBUG: Script arguments and their values:"
set | grep "^FORM_"
exit 0
