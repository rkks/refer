#! /bin/sh
# $Id: rcsgrep 1.5 1997/04/18 20:35:42 jerry Exp $
###	rcsgrep, rcsegrep, rcsfgrep - check out files and grep/egrep/fgrep them
###	Usage: rcsgrep [-s] [-a] [-rn.m] [grep options] regular-expression [files]
##
##	USE -s IF YOU DON'T WANT TO SEE THE CHECKOUT INFO FROM RCS.
##	USE -a TO RUN grep ON ALL REVISIONS OF THE FILE
##	USE -r TO RUN grep ON SOME PARTICULAR REVISION (DEFAULT: LATEST REVISION)

g=`echo g | tr g '\007'`	# CTRL-G FOR sed; NO CONFLICT WITH / IN PATHNAMES
rcsrevs=/usr/local/txtools/bin/rcsrevs	# GETS ALL REVISIONS OF RCS FILE FOR -a

# SHELL FUNCTION TO DO THE co AND grep:
doit()
{
	# READ THE FILE FROM co AND LOOK FOR MATCHES.
	# IF THERE'S A -e OPTION, $estr WILL BE DEFINED; OTHERWISE,
	# $regexp WILL BE DEFINED; PUT A SINGLE -e BEFORE THEM UNLESS -f WAS USED:
	case "$fnonly" in
	yes)# IF grep FINDS FILENAME, echo IT (FAKE grep -l).
		# NOTE: grep READS stdin "FROM THE if" (PIPE FEEDING THE if).
		co $silent -p$rev $file |
		# USING grep -s WOULD BE SIMPLER (AND FASTER, PROBABLY).
		# BUT AS SOON AS grep -s FINDS ONE MATCH, IT QUITS.
		# THEN co GETS KILLED WITH A SIGPIPE AND IT PRINTS rcs: cleaning up.
		# SO, USE STANDARD grep AND THROW AWAY MATCHES.
		if $prog $opts $fstr $eopt ${estr:+"$estr"} ${regexp:+"$regexp"} >/dev/null
		then echo "$basefile"
		fi
		;;
	*)	# HAVE TO DO grep INSIDE LOOP SO grep -n GIVES RIGHT LINE NUMBER
		co $silent -p$rev $file |
		$prog $opts $fstr $eopt ${estr:+"$estr"} ${regexp:+"$regexp"} |
		sed "s${g}^${g}$basefile:${g}"
		;;
	esac
}

case "$0" in
*rcsgrep) prog=/bin/grep ;;
*rcsegrep) prog=/bin/egrep ;;
*rcsfgrep) prog=/bin/fgrep ;;
*) echo "$0: Help!  Don't know how to run myself!" 1>&2; exit 1 ;;
esac

# SEPARATE OPTIONS AND FILENAMES:
eopt=-e estr= opts= files= fnonly= regexp= silent= rev= all=
while :
do
	case "$1" in
	"")	case $# in
		0)	break ;;
		*)	echo "`basename $0`: WARNING: ignoring empty argument!?!" 1>&2 ;;
		esac
		;;
	-a)	all=yes ;;	# CHECK OUT AND grep ALL REVISIONS
	-e)	# NEXT ARGUMENT IS AN EXPRESSION:
		case "$2" in
		"")	echo "`basename $0` quitting: missing expression after '-e'." 1>&2
			exit 1
			;;
		*)	estr="$2"
			shift
			;;
		esac
		;;
	-f)	# NEXT ARGUMENT IS A FILENAME.
		# DON'T LET egrep OR fgrep BOMB OVER AND OVER IF FILE NOT READABLE:
		if [ -r "$2" ]
		then
			fstr="-f $2"
			eopt=
		else
			echo "`basename $0` quitting: can't read file for '-f $2'." 1>&2
			exit
		fi
		shift
		;;
	-l)	fnonly=yes ;;	# grep -l ONLY PRINTS FILENAME
	-r?*)	rev=`expr "$1" : '-r\(.*\)'` ;;	# RCS REVISION NUMBER
	-r)	echo "`basename $0` quitting: missing revision number after -r." 1>&2
		exit 1
		;;
	-s*)	silent=-q ;;
	-*)	opts="$opts $1" ;;
	*)	# FIRST NON-OPTION ARGUMENT IS REGULAR EXPRESSION (EXCEPT WITH -e).
		# REST ARE FILES.
		if [ -z "$estr" -a -z "$regexp" ]
		then regexp="$1"
		else
			if [ -r "$1" -o -r "RCS/$1,v" -o -r "RCS/$1" ]
			then files="$files $1"
			else echo "`basename $0`: can't read file '$1'; skipping..." 1>&2
			fi
		fi
	esac
	shift
done

case "$all$rev" in
yes?*)
	echo "`basename $0` WARNING: ignoring -a.  Can't use -a with -r." 1>&2
	all=
	;;
esac

# IF NO files SET, USE RCS/*,v:
for file in ${files:-RCS/*,v}
do
	case "$file" in
	RCS/*,v) basefile="`expr $file : 'RCS/\(.*\),v'`" ;;
	*,v) basefile="`expr $file : '\(.*\),v'`" ;;
	*)	basefile="$file" ;;
	esac

	case "$all" in
	yes)
		# LOOP THROUGH ALL REVISIONS:
		for rev in `$rcsrevs $file`
		do doit
		done
		;;
	*) doit ;;
	esac
done
