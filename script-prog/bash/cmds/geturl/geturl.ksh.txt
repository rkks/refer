#! /bin/ksh
##########################################################################
# Title      :	geturl.ksh - get web page
# Version    :	1.1
# Author     :	Heiner Steven <heiner.steven@odn.de>
# Date       :	2002-06-14
# Category   :	WWW
# Requires   :	getent, ksh
# SCCS-Id.   :	@(#) geturl.ksh	1.1 04/02/18
##########################################################################
# Description
#	Retrieves a web page using ksh built-in networking functionality
# Portability
#	ksh (Solaris 9), ksh93
#	does not work with PD-KSH 5.2.14 (Linux)
##########################################################################
PN=${0##*/}			# Program name
VER='1.1'
: ${HTTP_PROXY:=$http_proxy}
function Usage {
    print -u2 "$PN - get WWW page specified by URL, $VER (hs '97)
usage: $PN [-chv] url [...]
   -c: only check the url, do not get the contents
   -h: only print header, no contents
   -v: verbose mode
An url has the form
	http:[//HOST][:PORT]PATH
i.e.
	http://www.shelldorado.com:80/index.html"
    exit 1
}
function Msg {
    print -u2 "$PN:" "$@"
}
function Fatal { Msg "$@"; exit 1; }
# Return the parsed URL in the variables
#	Protocol, Host, Port, Path
function ParseUrl
{
    typeset U=$1 H=
    (( $# == 1 )) || Fatal "usage: $0 url"
    # URL-Format: http://HOST/DIR/.../FILE
    Protocol=${U%%:*}
    [[ -n $Protocol ]] || return 1		# Protocol must be specified
    U=${U#*:}					# Remove protocol
    H=$(expr "$U" : '//\([^/][^/]*\).*')	# hostname:portnumber
    if [[ -n $H ]]
    then
	#Host=`expr "$H" : '\([^:][^:]*\).*'`
    	Host=${H%%:*}
	Port=`expr "$H" : '[^:][^:]*:\([0-9][0-9]*\).*'`
	U=`expr "$U" : "//[^/][^/]*\(.*\)"`	# Remove hostname:portnumber
    fi
    Path=$U
    return 0
}
function ResolveHost
{
    (( $# == 1 )) || Fatal "usage: $0 host"
    typeset ip= host=$1
    ip=$(getent hosts "$host") || return
    ip=${ip%%[ 	]*}
    print "$ip"
}
# Strip header from HTTP response, and write it to a file.
# A header is terminated by an empty line
function StripHeader {
    # Line terminator may be CR LF (instead of LF)
    cat "$@" | (
	    OIFS="$IFS"; IFS=""
	    while read line
	    do
		echo "$line"
		case "$line" in
		    ""|"
")	break;
		esac
	    done > "$Header"
	    IFS="$OIFS"
	    cat)
    return 0
}
Header="${TMPDIR:=/tmp}/gu$$"		# Temp. file for HTTP response header
CMD=GET					# HTTP request ("GET" or "HEAD")
verbose=false				# true/false
printheader=false
check=false
UseProxy=false
while getopts :chv opt
do
    case "$opt" in
	#f)	argument=$OPTARG;;
	h)	CMD=HEAD; printheader=true;;
	c)	CMD=HEAD; check=true;;
	v)	verbose=true;;
	?)	Usage;;
    esac
done
shift OPTIND-1
[[ -n $HTTP_PROXY ]] && UseProxy=true
# Remove temporary files at signal or exit
trap 'rm -f "$Header" >/dev/null 2>&1' 0
trap "exit 2" 1 2 3 15
ExitCode=0
for url
do
    if $UseProxy
    then
	ParseUrl "$url"; DstHost=$Host
    	ParseUrl "$HTTP_PROXY" || {
	    Msg "malformed proxy URL: $HTTP_PROXY"; continue; }
	Path="$url"
    else
	ParseUrl "$url" || { Msg "malformed URL: $url"; continue; }
	DstHost=$Host
    fi
    [[ ${Protocol:-INVALID} = http ]] ||
	{ Msg "unknown protocol \"$Protocol\" (only \"http\" allowed)";
	  continue; }
    ip=$(ResolveHost "$Host")
    [[ -n $ip ]] || Fatal "cannot resolve host name: $Host"
    # Set default values
    : ${CMD:=GET}
    : ${Host:=127.0.0.1}
    : ${Port:=80}
    : ${Path:=/}
    [ $verbose = true ] && Msg "getting http://$Host:$Port$Path"
    exec 3<> /dev/tcp/$ip/$Port
    print -u3 "${CMD:-GET} ${Path:-/} HTTP/1.0
Host: $DstHost
"
    StripHeader < /dev/fd/3 ||
	{ Msg "could not connect to $Host:$Port"; continue; }
    exec 3<&- 3>&-
    $printheader && cat "$Header"
    if $check
    then
	# Search response header for state line "HTTP/1.0 200 OK"
	read response < "$Header" || Fatal "cannot read response header"
	if [[ $response = *" 200 "* ]]
	then print "OK	$url"
	else print "ERROR	$url"
	fi
    fi
done
exit $ExitCode
