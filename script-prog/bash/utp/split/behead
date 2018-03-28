#! /bin/sh
#
###	behead - remove header line(s) from news or email messages
###	Usage: behead [files]
##
##	behead removes header lines from news article(s) or mail messages
##	named on the command line (or from standard input if no arguments).
##
##	Typical use from rn, when you're saving an article into a file:
##		End of article 234 -- what next? [npq] | behead > ~/filename
##	or, from readnews:
##		What now? [ynq] s- | behead > filename
##	or at a shell prompt:
##		% behead savecopy | mail -s "Did you see this?" freds
##
##	Taken from Usenet; written by ado@elsie.UUCP (Arthur David Olson),
##	..decvax!seismo!elsie!ado    ARPA: elsie!ado@seismo.ARPA

case $# in
0)	exec sed '1,/^$/d' ;;
*)	for i
	do
		sed '1,/^$/d' "$i"
	done
	;;
esac
