#!/bin/sh

#tmp="/tmp/$$-fifo"
#mkfifo $tmp
tmphtm="/tmp/$$-htm"
wgetops="-U netscape"

#close stdin
0<&-

noclobber=0
pat='[hH][rR][eE][fF]'

running=1
while [ $running = "1" ]
do
	running=0
	case $1 in
		'-n' )
			noclobber=1
		;;
		'-l' )
			pat='[hH][rR][eE][fF]'
			shift
			running=1
		;;

		'-e' )
			pat='[sS][rR][cC]'
			shift
			running=1
		;;

		* )
			running=0
		;;
	esac
done

for f in $@
do
	if [ "${f:0:7}" = "http://" ]; then
		# Convert things like %7E into their ASCII equivalents
		f=`echo ${f} | sed 's/%/\\\\x/g'`
		f=`echo -e ${f}`;
		# Extract path and filename from URL
		file=`echo ${f} | sed -r 's/^.*\/([^\/]*)$/\1/g;'`
		path=`echo ${f} | sed -r 's/http:\/\///g;'`
		if [[ -z ${file} ]] ; then
			fname="${path}index.html"
		else
			fname="${path}";
		fi

		# Read URL and feed into file
		wget ${wgetops} --force-html --no-clobber -x -k $f -P $tmphtm

		# Check for file
		if [[ -f "${tmphtm}/${fname}" ]] ; then
			# Attach file to stdin
			exec 0< "${tmphtm}/${fname}"
		else
			echo "File not found:" "${tmphtm}/${fname}"
			ls -lR "${tmphtm}"
			exit 1			
		fi

		# Delete file
		#rm "${tmphtm}/${fname}"
	else
		# Open file as standard input
		exec 0< $f
	fi

#	sed 's/>/>\n/g' | grep --binary-files=text -e "[hH][rR][eE][fF]=\"[^\"]*" -o \
#	| sed -r "s/[hH][rR][eE][fF]=\"//"
	sed 's/>/>\n/g' | grep --binary-files=text -e "${pat}=\"[^\"]*" -o \
	| sed -r "s/${pat}=\"//"

	#close stdin
	0<&-
done

rm -Rf $tmphtm
