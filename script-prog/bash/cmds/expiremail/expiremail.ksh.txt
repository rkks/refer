#! /usr/bin/ksh -p
##########################################################################
# Title      :	expiremail.ksh - expire mail messages by date
# Author     :	Heiner Steven <heiner.steven@odn.de>
# Date       :	1999-12-21
# Requires   :	awk, ksh, mailx
# Category   :	Mail
# SCCS-Id.   :	@(#) expiremail.ksh	1.18 05/09/15
##########################################################################
# Description
#	Selects e-mails matching some criterion from input folder, and
#	either writes them to an output folder, and/or removes them.
#	A criterion can be the age of the e-mail (as determined by the
#	"Date:" or "From:" header field), or a regular expression
#
# Examples
#	Delete all mails older than 30 days:
#		expiremail -d 30 mailfolder
#
#	Move all e-mail with a subject containing "INFO" to an
#	"info.archive" folder:
#	    $ expiremail -s "^Subject:.*INFO" -o info.archive mailfolder
#
#	Create a copy of all messages containing the subject "urgent",
#	and write them to the "urgent.mails" folder:
#	    $ expiremail -s "^Subject:.*urgent" -c -o urgent.mails mailfolder
#
# Note
#   o   Needs BSD "mailx" compatible mail user agent with the
#       following commands:
#               copy messagenumer outputfile
#               delete messagenumber
#               echo string
#               [size messagenumber]
#               quit
#
# Bugs
#    o	Some problems arouse with the script "hangig" in "Terminate",
#    	because "read -p" blocked although the process already terminated.
#    	If the "mailx" implementation has a "size" command, the
#    	work-around in "Terminate" should suffice. Otherwise, the
#    	"Terminate2" function should be used.
#
#    o  It's not necessary to use a temporary file for each
#       processed mail message. The date could be determined
#       piping the message to a script, or just by making
#       the mail user agent print a header line with date.
#
#    o  The date of a mail is determined searching the mail
#       header, and converting the date to a number of days. The
#       conversion is rather crude: it does not account for time
#       zones and does not even try to consider the time of day.
#
#       Maybe the date and time could be set on the temporary
#       mail file and a timestamp file using "touch"? This would
#       strip the date comparison down to
#       "[ $tmp -nt $timestamp ] && keep_msg"
#
#    o	If the "mailx" "autoprint" feature is enabled, each "SendLine" may
#    	result in more than one output line. Each response is therefore
#    	limited to one line using 'response=$(print "$response" | sed q)'.
#    	This additional invocation of "sed" (or "head -1") should be
#    	unnecessary with the expression 'response=${response%%\012*}', but
#    	the expression does not work. Neither does it work with '\n' or
#    	the control code '^M' inserted. Why??
#
# Portability
#    o	Tested with
#	    Solaris 2.8/SPARC (ksh M-11/16/88i, dtksh M-12/27/93d)
#	    Solaris 2.6/SPARC (ksh M-11/16/88i)
#	    Solaris 2.5/SPARC (ksh 11/16/88i)
#	    Linux 2.2.16 (PD KSH 5.2.14)
#    o	Reported to work with AIX (thanks Joeri Belis)
##########################################################################
PN=${0##*/}				# Program name
VER='1.18'
#: ${AWK:=awk}
: ${USER:=$(expr "$(id)" : '[uU][iI][dD]=[0-9]*(\([^\)]*\)).*')}
DAYS=30
function usage {
    print -u2 "$PN - expire mail messages, $VER (stv '99)
usage: $PN [-cnvD] [-d days] [-o oldmails] [mailfile ...]
   or  $PN [-cnvD] [-s regexp] [-o oldmails] [mailfile ...]
    -d:	minimum age of a message (in days) that should be deleted
    	(default is $DAYS)
    -s: search for \"egrep\" regular expression
    -c: copy mail messages (instead of expiring them)
    -o: file to write expired mails to
    -v: verbose output, write mail number currently processed
    -n: no change of the mailbox, just print what would have been done
    -D: turn on Debug output
All messages older than the given number of days (option -d), or
containing a line matching the \"egrep\" regular expression (option -s)
are removed from all specified mailfiles. If an output file was specified
using \"-o\", all expired mails are moved to this file.
Note that it's not possible to specify more than one output file."
    exit 1
}
function msg {
    typeset line
    for line
    do print -u2 "$PN: $line"
    done
}
function fatal { msg "$@"; exit 1; }
#########################################################################
# searchprog - search program using PATH
#########################################################################
function searchprog {
    typeset search dir
    for search
    do
	for dir in $(print "$PATH" | sed "s/^:/.:/;s/:\$/:./;s/:/ /g")
	do
	    [[ -x $dir/$search ]] || continue
	    print "$dir/$search"
	    return 0
	done
    done
    return 1
}
#########################################################################
# SendLine - send one line to co-process, return (multi-line) results
#
# A reply could consist of more than one line. If we read only
# the first line, in case of multi-line responses further
# invocations of this function would return older, unread lines
# instead of the response from the process.
# If we call "read" one time too often, it will block.
#
# We resolve this problem with the famous "GNUELPF" protocol:
#    o  After printing the request line, we make the co-process
#       return a well-known, unique line as a reply delimiter (in
#       this case "__GNUELPF__")
#    o  Now we can safely read all lines until we get the
#       delimiter
#
# Note that this method may generally be used to communicate with
# any co-process. We only requirements for the co-process:
#
#   1)  There must be a way to echo a well known delimiter
#       (i.e. a "print", "echo", or "NOOP" command)
#   2)  There must be a unique delimiter
#
# See the script "calc.ksh" for an example on how to use this
# method to communicate with a "bc" co-process.
#########################################################################
function SendLine {
    print -p -- "$@" || return $?
    [[ $Debug = true ]] && print -u2 "DEBUG: O" "$@"
    print -p -- "echo __GNUELPF__" || return $?
    while read -p
    do
    	[[ $REPLY = __GNUELPF__ ]] && break
    	[[ $Debug = true ]] && print -u2 "DEBUG: I <$REPLY>"
        print -r -- "$REPLY"
    done
    return 0
}
#########################################################################
# Deletemsg - delete a message by numer (lists are accepted, too)
#########################################################################
function Deletemsg {
    typeset msglist="$@"
    typeset response
    response=$(SendLine "delete $msglist")
    # Why does none of the following expressions work??
    # response=${response%%\n*}
    # response=${response%%\012*}
    # response=${response%%^M*}
    # response=${response%%[\n]*}
    # response=${response%%[\012]*}
    # response=${response%%[^M]*}
    # The ksh internal substitution should be much faster than the following
    # work-around:
    
    response=$(print "$response" | sed q)
    case "$response" in
    	${msglist}:*)	return 1;;	# i.e. "1: Inappropriate message"
	*)		return 0;;
    esac
}
#########################################################################
# Get mail article by number
# usage: Readmsg msgnumber outfile
#########################################################################
function Readmsg {
    (( $# != 2 )) && fatal "$0: need two arguments"
    typeset list="$1"
    typeset outfile="$2"
    typeset -l response		# value is converted to lower case
    response=$(SendLine "copy $list $outfile")
    response=$(print "$response" | sed q)
    [[ $Debug = true ]] && print -u2 "DEBUG: I <$response>"
    case "$response" in
    	*new*file*|*appended*)	return 0;;
	*)			return 1;;
    esac
}
#########################################################################
# Terminate - terminate mail user agent
#
# Does some plausibility checks on the mail user agent to
# determine, if it is still alive and functioning.
#
# Return codes
#       0       success
#       1       error communicating with mail agent
#########################################################################
 
function Terminate {
    typeset response
    typeset -u status=ERROR
    integer lastnum
    # See if there is a message in the mailbox left
    response=$(SendLine "size $")
    response=$(print "$response" | sed q)
 
    if [[ -n "$response" ]]
    then
        # If there is no message left, we get this response:
        #       "No applicable message"
	case "$response" in
	    *"No applicable"*)
		status=REMOVED
		;;
	    *)
		# Ordinarily the output looks like this:
		#   "53: 1234"      (number of last message, size)
		lastnum="${response%%:*}" 2>/dev/null
		if (( ${lastnum:=0} > 0 ))
		then
		    if (( $expired > 0 ))
		    then status=UPDATED
		    else status=UNCHANGED
		    fi
		else
		    status=ERROR
		    print -u2 "ERROR: unexpected response: <$response>"
		fi
		;;
	esac
    else
        status=ERROR
        print -u2 "ERROR: mail user agent did not answer"
    fi
 
    # Now terminate the user agent gracefully, saving changes
    if [[ $status != ERROR ]]
    then
    	print -p -- quit || status=ERROR
    fi
 
    print $status
    if [[ $status != ERROR ]]
    then
        return 0
    fi
    return 1
}
#########################################################################
# Terminate - terminate mail user agent
#
# Does some plausibility checks on the mail user agent to
# determine, if it is still alive and functioning.
#
# Return codes
#       0       success
#       1       error communicating with mail agent
#########################################################################
 
function Terminate2 {
    typeset response
    # After the "quit" command our famous "GNUELPF" protocol will no
    # longer work, so we cannot use the "SendLine" function
    [[ $Debug = true ]] && print -u2 "DEBUG: O <quit>"
    print -p -- quit || return $?
    # Sometimes the mail co-process terminates before we an read the final
    # answer using "read -p". With Linux, the read "hangs" indefinitely.
    # To prevent this, we set a timeout for the "read" command, and store
    # the result in a temporary file.
    # Start the (potentially blocking) read process in the background
    (read -p && print "$REPLY" > "$Tmp") & 	readpid=$!
    # Now start a "watchdog" process that will kill the reader after
    # some time:
    (
    	sleep 2; kill $readpid >/dev/null 2>&1 ||
	    { sleep 1; kill -1 $readpid >/dev/null 2>&1; } ||
	    { sleep 1; kill -9 $readpid; }
    ) &		watchdogpid=$!
    # Now wait for the reading process to terminate. It will terminate
    # reliably, either because the read terminated, or because the
    # "watchdog" process made it terminate.
    wait $readpid
    # Now stop the watchdog:
    kill -9 $watchdogpid >/dev/null 2>&1
    REPLY=TERMINATED			# Assume the worst
    [[ -s $Tmp ]] && read < "$Tmp"
    [[ $Debug = true ]] && print -u2 "DEBUG: I <$REPLY>"
    case "$REPLY" in
    	*removed*)		# Example: "mailbox.canary" removed.
	    print REMOVED
	    return 0;;
	*updated*|*complete*)	# Example: "mailbox.canary" updated.
	    print UPDATED
	    return 0;;
	*TERMINATED*)
	    print UPDATED	# Assume we changed something
	    return 0;;
	'')			# No changes -> no output
	    print UNCHANGED
	    return 0;;
    esac
    print ERROR
    return 1
}
#########################################################################
# GetDate - get date string from mail header
#########################################################################
 
function GetDate {
    sed -n -e '/^Date: /{;s/^Date: //;p;q;}' "$@"
}
#########################################################################
# GuessDate - provide other means to determine date of mail message
#   
# At this time this function assumes "From " lines in the
# following format: "From heiner Tue Jan 11 10:05:02 2000"
#########################################################################
 
function GuessDate {
    $AWK '
    	$0 ~ /^From / {
	    for ( i=3;i<=NF; ++i ) {
	    	if ( i>3 ) printf " "
		printf "%s", $i
	    }
	    print ""
	    exit (0)
	}
    ' "$@"
}
#########################################################################
# Date2Days - convert date string to number of days since year "0".
#
# A day is any number in the range 1..31
# A year is any number in the range 1000-9999
# A Month is an english, three letter abbreviation of a month name
#########################################################################
Date2Days () {
    typeset -l date=$*			# Convert to lower case
    print -- "$date" |
	$AWK '
	    BEGIN {
	    	# This table contains each month with the number
		# of days until, but not including this month
		# for the current year
	    	M ["jan"] = 0
		M ["feb"] = M ["jan"] + 31
		M ["mar"] = M ["feb"] + 29
		M ["apr"] = M ["mar"] + 31
		M ["may"] = M ["apr"] + 30
		M ["jun"] = M ["may"] + 31
		M ["jul"] = M ["jun"] + 30
		M ["aug"] = M ["jul"] + 31
		M ["sep"] = M ["aug"] + 31
		M ["oct"] = M ["sep"] + 30
		M ["nov"] = M ["oct"] + 31
		M ["dec"] = M ["nov"] + 30
	    }
	    {
	    	d = md = 0
		y = ""
	    	for ( i=1; i<=NF; i++ ) {
		    if ( $i ~ /^[0-9][0-9]*$/ ) {
			if ( 1 <= $i && $i <= 31 ) {
			    d = $i			# Day of month
			} else if ( 1000 <= $i && $i <= 9999 ) {
			    y = $i			# Year
			}
		    } else if ( M [$i] != "" ) {
		    	# This is a month name
			md = M [$i]	# days until this month
		    }
		}
		if ( y == "" ) {
		    # The year could not be determined. Maybe
		    # it was specified using two digits instead of four?
		    # This time we assume a number following the month
		    # name to be the year
		    d = md = 0
		    y = ""
		    for ( i=1; i<NF; ++i ) {
			if ( $i ~ /^[0-9][0-9]*$/ &&	\
				d == 0 && 1 <= $i && $i <= 31 ) {
			    d = $i			# Day of month
			} else if ( M [$i] != "" ) {
			    # This is a month name
			    md = M [$i]	# days until this month
			    if ( $(i+1) ~ /^[0-9][0-9]*$/ ) {
				y = $(i+1)
				# This is a number. If it consists of
				# two digits, convert it to four
				if ( 0<=y && y<=99 ) {
				    if ( y >= 70 ) y += 1900; else y += 2000
				} else {
				    y = 0
				}
			    }
			}
		    }
		}
	    }
	    END {
		#print "d=" d, "y=" y, "md=" md | "cat >&2"
		if ( d > 0 && y > 0 ) {
		    # This is just an approximation: every fourth year
		    # is a leap year.
		    days = (d-1) + md + int(y * (365 + 0.25))
		    print days
		}
	    }
	'
}
#########################################################################
# isMatching - does e-mail message match selection criterion?
#
# Return value:
#	0	message matches search
#	1	message does not match
#	>=2	error
#########################################################################
function isMatching {
    typeset maildate
    typeset days=
    integer age
    if [[ -n $Search ]]
    then
    	egrep "$Search" "$Mail" >/dev/null
	return $?
    else
	# Performance note: it is not strictly necessary to read the input file
	# twice here. GetDate could return more than one line.
	maildate=$(GetDate "$Mail")
	[[ -n $maildate ]] && days=$(Date2Days "$maildate")
	if [[ -z $days ]]
	then
	    maildate=$(GuessDate "$Mail")
	    [[ -n $maildate ]] && days=$(Date2Days "$maildate")
	fi
	#print -u2 "maildate=<$maildate> days=$days"
	if [[ -n $maildate && -n $days ]]
	then
	    age=$(($daystoday-$days))
	    #print -u2 "maildate=<$maildate> is $age days old"
	    if (( $age >= $DaysOld ))
	    then			# Expire this mail
	    	return 0
	    else
	    	return 1
	    fi
	else
	    #msg "cannot determine date for message $msgnum"
	    nodatelist="$nodatelist${nodatelist:+ }$msgnum"
	    return 2
	fi
    fi
}
#########################################################################
# shortlist - try to shorten a numerical list
#
# Example: "1 2 3 5 6 7 9" is shortened to "1-3,5,6-7,9"
#########################################################################
function shortlist {
    $AWK '
        BEGIN {
	    listsep = ","
	    rangesep = "-"
	}
    	NR >= 1 {
	    if ( initialized ) {
	    	startfield = 1
	    } else {
	    	oldval = $1
		startfield = 2
		initialized = 1
	    }
	    for ( fieldno = startfield; fieldno <= NF; ++fieldno ) {
		newval = $fieldno
		if ( newval == oldval + 1 ) {
		    if ( !inrun ) {
			inrun = 1
			if ( output != "" ) output = output listsep
			output = output oldval rangesep
		    }
		} else if ( inrun ) {
		    inrun = 0
		    output = output oldval
		} else {
		    if ( output != "" ) output = output listsep
		    output = output oldval
		}
		oldval = newval
	    }
	}
	END {
	    if ( output != "" && !inrun ) output = output rangesep
	    print output oldval
	}
    '
}
#########################################################################
# MAIN PROGRAM
#########################################################################
# Search a "mailx" compatible mail user agent
: ${Mailx:=$(searchprog mailx Mail mail)}
: ${AWK:=$(searchprog mawk gawk nawk awk)}
integer DaysOld
Copy=false
Verbose=false
Debug=false
Dryrun=false
while getopts chd:Dno:s:v opt 2>/dev/null
do
    case "$opt" in
    	c)	Copy=true;;
	d)	DaysOld=$OPTARG;;
	D)	Debug=true;;
	n)	Dryrun=true;;
	o)	OldMails=$OPTARG;;
	s)	Search=$OPTARG;;
	v)	Verbose=true;;
	h)	usage;;
	*)	usage;;
    esac
done
shift $OPTIND-1
[[ $Copy = true && -z $OldMails ]] && fatal "-c requires output file (-o)"
[[ -n $DaysOld && -n $Search ]] && fatal "-s and -d cannot be used together"
if (( $# < 1 ))
then
    # No mailbox specified
    for maildir in /var/spool/mail /usr/spool/mail /var/mail
    do
        if [[ -e "$maildir/$USER" ]]
	then
	    set -- "$maildir/$USER"
	    break
	fi
    done
fi
if [[ -z $Search ]]
then
    : ${DaysOld:=$DAYS}
fi
if [[ -n $DaysOld ]]
then
    integer daystoday=$(Date2Days $(LANG=C date) )
    [[ -n $daystoday ]] || fatal "cannot determine number of days for today"
fi
if [[ -n $OldMails ]]
then
    # Try to write to the folder, and exit if it does not succeed
    >> "$OldMails" || fatal "cannot write to file: $OldMails"
fi
# Remove temporary files if the program terminates.
# Note that the name of $NewFolder may change during program execution.
Mail=${TMPDIR:=/tmp}/expm.$$.m
Tmp=$TMPDIR/expm.$$.t
trap 'rm -f "$Mail" "$NewFolder" "$Tmp" >/dev/null 2>&1' EXIT
trap '
    trap "" 1 2 3 13 15		# ignore consecutive signals
    print -u2 "$PN: INTERRUPTED"
    kill $mailpid >/dev/null 2>&1
    exit 2			# NOTE: signals are still ignored
' HUP INT QUIT TERM
typeset -u status		# Contents are always in upper case
for MailFolder
do
    [[ -f $MailFolder && -r $MailFolder && -s $MailFolder ]] || {
    	msg "cannot read mail folder: $MailFolder - ignored"
	continue
    }
    # Create the new folder in the same directory than the original
    # folder. This way the subsequent "mv" will probably work.
    NewFolder=$(dirname "$MailFolder")/expm.$$
    #print -u2 "NewFolder=$NewFolder"
    # The new folder should have the same file permissions than
    # the old one. There should be a better way to copy file permissions
    # than this:
    cp -p "$MailFolder" "$NewFolder" || exit 1
    # Start mail program as co-process. We will use this program
    # to read and delete mails.
    # Options:
    #	-N		do not print header lines
    #	-n		do not read system wide startup files
    #	-I		print article ids
    #	-f folder	use the specified mailbox
    [[ $Verbose = true ]] && msg "reading mailbox $MailFolder..."
    #$Mailx -N -n -I -f "$NewFolder" |&	mailpid=$!
    $Mailx -N -n -f "$NewFolder" |&	mailpid=$!
    r=$(SendLine "echo ok" | tail -1)	# result could have multiple lines
    #print -u2 "echo ok: <$r>"
    [[ $r = ok ]] || fatal "could not communicate with mail agent co-process"
    integer expired=0			# Number of messages removed
    integer copied=0			# Number of messages retained
    integer msgnum=1
    nodatelist=
    while Readmsg $msgnum "$Mail"
    do
    	if isMatching "$Mail"
	then
	    if [[ $Dryrun = false ]]
	    then
		if [[ -n "$OldMails" ]]
		then
		    cat "$Mail" >> "$OldMails" || exit 1
		fi
		[[ $Copy = false ]] &&
		    Deletemsg $msgnum
	    fi
	    expired=expired+1
	else			# Keep this mail
	    copied=copied+1
	fi
    	[[ $Verbose = true ]] && msg "processed $msgnum, expired $expired\r\c"
	msgnum=msgnum+1
	> "$Mail"
    done
    msg "$MailFolder: $expired messages expired, $copied left"
    [[ -n $nodatelist ]] && {
	msg "$MailFolder: no date for the following messages: $(echo "$nodatelist" | shortlist)"
    }
    status=$(Terminate)		# quit the mail program, saving changes
    case "$status" in
    	UPDATED)
	    [[ $Dryrun = false && $expired > 0 ]] && {
	    	if [[ $Copy = false ]]
		then
		    mv "$NewFolder" "$MailFolder" || exit 1
		    msg "$MailFolder: updated"
		fi
	    }
	    ;;
	UNCHANGED)
	    if [[ $Dryrun = false ]]
	    then
		rm -f "$NewFolder" >/dev/null 2>&1 || exit 1
	    fi
	    msg "$MailFolder: unchanged"
	    ;;
	REMOVED)
	    if [[ $Dryrun = false ]]
	    then
		rm -f "$NewFolder" >/dev/null 2>&1
		rm -f "$MailFolder" || exit 1
	    fi
	    msg "$MailFolder: removed"
	    ;;
	*)
	    fatal "unexpected termination message from mail user agent: $status"
	    ;;
    esac
    # Now start a "watchdog" process that will kill the mail process after
    # some time:
    (
    	sleep 3
	kill $mailpid >/dev/null 2>&1 ||
	    { sleep 1; kill -1 $mailpid >/dev/null 2>&1; } ||
	    { sleep 1; kill -9 $mailpid; }
    ) &		watchdogpid=$!
    # Now wait for the mail process to terminate. It will terminate
    # reliably, either because the process is finished, or because the
    # "watchdog" process made it terminate.
    wait $mailpid
    # Now stop the watchdog:
    kill -9 $watchdogpid >/dev/null 2>&1
done
exit 0
