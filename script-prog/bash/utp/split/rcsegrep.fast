#! /bin/sh
###	rcsegrep.fast - fast version of rcsegrep that reads RCS files directly
###	Usage: rcsegrep.fast [-n] pattern [files]
#
#	Thanks to Dale Dougherty for the match() idea.
#
#	Tabstops in this script set at 4 (in vi, use :se ts=4 sw=4)

# CONFIGURE: If your RCS files use a ",v" or other extension,
# edit the "for" loop below where the comment is

myname=`basename $0`
nopt=0					# DEFAULT -n SETTING (0 = OFF)

if [ ! -d RCS ]; then
	echo "$myname quitting: can't find RCS directory." 1>&2
	exit 1
fi
case "$#" in
0)	echo "Usage: $myname [-n] pattern [files]" 1>&2; exit 1;;
esac

# GRAB OPTION/PATTERN, BE SURE FILENAMES (IF ANY) ARE PATHS TO RCS DIRECTORY:
while :
do
	case "$1" in
	-n)	nopt=1
		shift
		;;
	-*)	echo "Usage: $myname [-n] pattern [files]" 1>&2
		exit 1
		;;
	*)	pattern="$1"
		shift
		break	# LET for LOOP BELOW HANDLE FILENAMES (IF ANY)
		;;
	esac
done

for f
do
	case "$f" in
	*RCS/?*) files="$files $f" ;;
	*) files="$files RCS/$f" ;;	# Edit if RCS files end in ",v", etc.
	esac
done

nawk '
BEGIN {
	no=0
	yes=1
	lastfn="fajskdfjahsfoqarufhfsa"	# INITIALIZE
}
{
	# JUST STARTED READING NEW FILE:
	if (FILENAME != lastfn) {
		lastfn=FILENAME
		infile=no	# READING THE TEXT (NOT RCS CONTROL INFO AT TOP OF FILE)
		skipline=no	# SKIP THIS LINE IF IT IS OLD REVISION INFO (AFTER TEXT)
		textnext=no	# IF JUST ABOUT TO READ TEXT, SET TO YES
		# MAKE FILENAME WITHOUT "RCS/"
		fn=substr(FILENAME,5)
	}

	# SKIP TO NEXT LINE IF NOT READING THE TEXT:
	if (skipline == yes)
		next

	if (infile == yes) {
		if ($0 == "@") {
			# END OF TEXT:
			skipline=yes
			next
		}
	}
	else {
		# NOT IN FILE YET.  CHECK FOR START OF FILE.  IT LOOKS LIKE:
		#	text
		#	@<text-of-file-line-1>
		if ($0 == "text") {
			textnext=yes
			next
		}
		if (textnext == yes) {
			textnext=no	# RESET, WHETHER THIS IS REAL START OR NOT:
			if ($0 ~ /^@/) {
				infile=yes
				lineno = 0
				sub("^@", "")
				# DROP THROUGH TO TEXT CHECK BELOW
			}
		}
		else
			next	# FALSE ALARM... NOT IN TEXT OF FILE YET
	}

	# IF WE GET HERE, WE ARE IN THE TEXT OF THE FILE.  SEARCH FOR XREFS:
	lineno++
	# RCS CHANGES ANY @ TO @@.  UN-DO THAT:
	gsub("@@", "@")
	if (match($0, pattern)) {
		printf "%s:", fn
		if (nopt)
			printf("%d:", lineno)
		print
	}
}' nopt=$nopt pattern="$pattern" myname="$myname" ${files-RCS/*}
