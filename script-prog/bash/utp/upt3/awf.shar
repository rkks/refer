Subject:  v23i027:  nroff -man/-ms clone written in (old) awk
Newsgroups: comp.sources.unix
Approved: rsalz@uunet.UU.NET
X-Checksum-Snefru: 8f01fd90 8fe678dc 00c31377 965ef65e

Submitted-by: henry@zoo.toronto.edu
Posting-number: Volume 23, Issue 27
Archive-name: awf

This is awf, the Amazingly Workable Formatter -- a "nroff -man" or
(subset) "nroff -ms" clone written entirely in (old) awk.

I can't believe I really wrote this.
[  You're not alone in that sentiment, Henry.  --r$  ]

                                         Henry Spencer at U of Toronto Zoology
                                          henry@zoo.toronto.edu   utzoo!henry

#! /bin/sh
echo 'README':
sed 's/^X//' >'README' <<'!'
XThis is awf, the Amazingly Workable Formatter -- a "nroff -man" or
X(subset) "nroff -ms" clone written entirely in (old) awk.
X
XIt is slow and has many restrictions, but does a decent job on most
Xmanual pages and simple -ms documents, and isn't subject to AT&T's
Xbrain-damaged licensing that denies many System V users any text
Xformatter at all.  It is also a text formatter that is simple enough
Xto be tinkered with, for people who want to experiment.
X
XType "make r" to run a regression test, formatting the manual page
X(awf.1) and comparing it to a preformatted copy (awf.1.out).  Type
X"make install" to install it.  Pathnames may need changing.
X
XI don't know whether awf will run on 16-bit machines.  Data requirements
Xare modest, but I fear the programs are probably big enough to run awk
Xout of space.
X
XI can't believe I really wrote this.
X
X                                         Henry Spencer at U of Toronto Zoology
X                                          henry@zoo.toronto.edu   utzoo!henry
X						13 July 1990
!
echo 'COPYRIGHT':
sed 's/^X//' >'COPYRIGHT' <<'!'
X/*
X * Copyright 1990 University of Toronto.  All rights reserved.
X * Written by Henry Spencer.
X * This software is not subject to any license of the American Telephone
X * and Telegraph Company or of the Regents of the University of California.
X *
X * Permission is granted to anyone to use this software for any purpose on
X * any computer system, and to alter it and redistribute it freely, subject
X * to the following restrictions:
X *
X * 1. The author is not responsible for the consequences of use of this
X *    software, no matter how awful, even if they arise from flaws in it.
X *
X * 2. The origin of this software must not be misrepresented, either by
X *    explicit claim or by omission.  Since few users ever read sources,
X *    credits must appear in the documentation.
X *
X * 3. Altered versions must be plainly marked as such, and must not be
X *    misrepresented as being the original software.  Since few users
X *    ever read sources, credits must appear in the documentation.
X *
X * 4. This notice may not be removed or altered.
X */
!
echo 'Makefile':
sed 's/^X//' >'Makefile' <<'!'
XAWFLIB = /usr/lib/awf	# beware, awf itself knows this
XBIN = /usr/bin
XMAN = /usr/man/man1
XCP = common dev.dumb mac.man mac.ms pass1 pass2.base pass2.man pass2.ms pass3
XDTR = README COPYRIGHT Makefile awf awf.1 awf.1.out common dev.dumb mac.man \
X	mac.ms pass1 pass2.base pass2.man pass2.ms pass3
X# System V brain damage
XSHELL = /bin/sh
X
Xr:	awf.1
X	chmod +x awf
X	AWFLIB=. awf -man awf.1 >tmp
X	cmp tmp awf.1.out
X	rm tmp
X
Xinstall:
X	-if test ! -d $(AWFLIB) ; then mkdir $(AWFLIB) ; fi
X	cp $(CP) $(AWFLIB)
X	cp awf $(BIN)
X	cp awf.1 $(MAN)
X
Xrr:	r testm tests.Z tests.out.Z
X	AWFLIB=. awf -man testm >tmp
X	cmp tmp testm.out
X	rm tmp
X	uncompress <tests.Z | AWFLIB=. awf -ms >tmp
X	uncompress <tests.out.Z | cmp - tmp
X	rm tmp
X
Xdtr:	$(DTR)
X	makedtr $(DTR) >$@
X
Xtar:	$(DTR)
X	tar cvf awf.tar $(DTR)
X	compress -v awf.tar
X
Xclean:
X	rm -f tmp tests tests.out dtr awf.tar awf.tar.Z j*
!
echo 'awf':
sed 's/^X//' >'awf' <<'!'
X#! /bin/sh
XPATH=/bin:/usr/bin ; export PATH
X# AWFLIB is where the pieces live
XAWFLIB=${AWFLIB-/usr/lib/awf}
X
Xtmp=/tmp/awp$$			# tempfile for building pass 2
Xerrs=/tmp/awe$$		# error messages (awk can't send to stderr)
X
Xcase "$1" in
X-ms)	mac=ms	;;
X-man)	mac=man	;;
X*)	echo "$0: must specify -ms or -man" >&2
X	exit 2
X	;;
Xesac
Xshift
X
Xdev="$AWFLIB/dev.$TERM"
Xif test ! -r $dev
Xthen
X	dev="$AWFLIB/dev.dumb"
Xfi
X
Xtrap "rm -f $tmp $errs ; exit 0" 0 1 2
X
X# build the full, macro-set-dependent, pass-2 awk program
X(
X	sed -n '1,/^#include/p' $AWFLIB/pass2.base
X	cat $AWFLIB/pass2.$mac
X	sed -n '/^#include/,$p' $AWFLIB/pass2.base
X) >$tmp
X
X# do it
X(
X	echo ".^x $errs"
X	echo ".^b"
X	echo ".^# 1 <prolog>"
X	cat $dev $AWFLIB/common $AWFLIB/mac.$mac
X	if test " $*" = " "
X	then
X		echo ".^# 1 <stdin>"
X		cat
X	else
X		for f
X		do
X			echo ".^# 1 $f"
X			cat $f
X		done
X	fi
X	echo ".^e"
X) | awk -f $AWFLIB/pass1 | awk -f $tmp | awk -f $AWFLIB/pass3
X
X# error messages, if any
Xif test -s $errs
Xthen
X	cat $errs >&2
X	exit 1
Xelse
X	exit 0
Xfi
!
echo 'awf.1':
sed 's/^X//' >'awf.1' <<'!'
X.\" Some of the stuff in this file is a bit contorted, because it's also
X.\" the regression-test input.
X.nr ES 5n
X.de ES
X.PP
X.in +\\n(ESu
X.nf
X..
X.de EE
X.in -\\n(ESu
X.fi
X.PP
X..
X.de PT
X.ie \\n(.$>1 .TP "\\$2"
X.el .TP
X.ie !'\\$1'' \\$1
X.el \(bu
X..
X.ds Nr \fInroff\fR
X.TH AWF 1 "13 July 1990"
X.BY "U of Toronto"
X.SH NAME
Xawf \- amazingly workable (text) formatter
X.SH SYNOPSIS
X.B awf
X.BI \-m acros
X[ file ] ...
X.SH DESCRIPTION
X.if t .tm OOPS -- AWF THINKS IT'S TROFF!!!
X.I Awf
Xformats the text from the input \fIfile\fR(s)
X(standard input if none)
Xin an imitation of
X\*(Nr's style with the \fB\-man\fR or \fB\-ms\fR macro packages.
XThe
X.BI \-m acro
Xoption is mandatory and must be `\-man' or `\-ms'.
X.PP
X.I Awf
Ximplements the following raw \*(Nr requests:
X.LP
X	.\e"	.ce	.fi	.in	.ne	.pl	.sp
X	.ad	.de	.ft	.it	.nf	.po	.ta
X	.bp	.ds	.ie	.ll	.nr	.ps	.ti
X	.br	.el	.if	.na	.ns	.rs	.tm
X.LP
Xand the following in-text codes:
X.ES
X\e$	\e%	\e*	\ec	\ef	\en	\es
X.EE
Xplus the full list of \*(Nr/\c
X.I troff
Xspecial characters in
Xthe original V7 \fItroff\fR manual.
X.PP
XMany restrictions are present; the behavior in general is a subset of
X\*(Nr's.  Of particular note are the following:
X.IP \(bu 2
XPoint sizes do not exist;
X.B .ps
Xand
X.B \es
Xare ignored.
X.PT
XConditionals implement only numeric comparisons on
X.BR \en(.$ ,
Xstring com\%par\%isons between a macro parameter and a literal,
Xand
X.B n
X(always true)
Xand
X.BR t
X(always false).
X.PT
XThe implementation of strings is generally primitive.
X.IP \(bu
XExpressions in (e.g.)\&
X.B .sp
Xare fairly general, but the
X.BR | ,
X.BR & ,
Xand
X.BR :\&
Xoperators do not exist, and the implementation of
X.B \ew
Xrequires that quote (') be used as the delimiter and
Xsimply counts the characters inside (so that, e.g.,
X\ew'\e(bu'
Xequals 4).
X.P
XWhite space at the beginning of lines,
Xand imbedded white space within lines, is dealt with properly.
XSentence terminators at ends of lines are understood to imply
Xextra space afterward in filled lines.
XTabs are im\%plemented crudely and not quite correctly, although
Xin most cases they work as expected.
XHyphenation is done only at explicit hyphens, em-dashes, and \*(Nr
Xdiscretionary hyphens.
X.SH "MAN MACROS"
XThe
X.B \-man
Xmacro set implements the full V7 manual macros,
Xplus a few semi-random oddballs.
XThe full list is:
X.ES
X\&.B	.DT	.IP	.P	.RE	.SM
X\&.BI	.HP	.IR	.PD	.RI	.TH
X\&.BR	.I	.LP	.PP	.RS	.TP
X\&.BY	.IB	.NB	.RB	.SH	.UC
X.EE
X.B .BY
Xand
X.B .NB
Xeach take a single string argument (respectively, an indi\%cation of
Xauthorship and a note about the status of the manual page) and arrange
Xto place it in the page footer.
X.SH "MS MACROS"
XThe
X.B \-ms
Xmacro set is a substantial subset of the V7 manuscript macros.
XThe implemented macros are:
X.ES
X\&.AB	.CD	.ID	.ND	.QP	.RS	.UL
X\&.AE	.DA	.IP	.NH	.QS	.SH	.UX
X\&.AI	.DE	.LD	.NL	.R	.SM
X\&.AU	.DS	.LG	.PP	.RE	.TL
X\&.B	.I	.LP	.QE	.RP	.TP
X.EE
XSize changes are recognized but ignored, as are
X.B .RP
Xand
X.BR .ND .
X.B .UL
Xjust prints its argument in italics.
X.BR .DS / .DE
Xdoes not do a keep,
Xnor do any of the other macros that normally imply keeps.
X.PP
XAssignments to the header/footer string variables are recognized and
Ximplemented, but there is otherwise no control over header/footer
Xformatting.
XThe
X.B DY
Xstring variable is available.
XThe
X.BR PD ,
X.BR PI ,
Xand
X.BR LL
Xnumber registers exist and can be changed.
X.SH OUTPUT
XThe only output format supported by
X.IR awf ,
Xin its distributed form,
Xis that appropriate to a dumb terminal,
Xusing overprinting for italics (via underlining) and bold.
XThe \*(Nr special characters are printed as some vague approximation
X(it's sometimes very vague) to their correct appearance.
X.PP
X.IR Awf 's
Xknowledge of the output device is established by a device file,
Xwhich is read before the user's input.
XIt is sought in
X.IR awf 's
Xlibrary directory, first as
X.BI dev. term
X(where \fIterm\fR is the value of the TERM environment variable)
Xand, failing that, as
X.BR dev.dumb .
XThe device file
Xuses special internal commands
Xto set up resolution, special characters, fonts, etc.,
Xand more normal \*(Nr commands to set up page length etc.
X.SH FILES
XAll in \fI/usr/lib/awf\fR (this can be overridden by the AWFLIB
Xenvironment variable):
X
X.ta \w'pass2.base'u+((3n-1n)/2u)
X.nf
Xcommon	common device-independent initialization
Xdev.*	device-specific initialization
Xmac.m*	macro packages
Xpass1	macro substituter
Xpass2.base	central formatter
Xpass2.m*	macro-package-specific bits of formatter
Xpass3	line and page composer
X.SH SEE ALSO
Xawk(1), nroff(1), man(7), ms(7)
X.SH DIAGNOSTICS
X.na
XUnlike
X.IR nroff ,
X.I awf
Xcomplains whenever it sees unknown commands and macros.
XAll diagnostics (these and some internal ones) appear on standard error
Xat the end of the run.
X.ad
X.SH HISTORY
XWritten at University of Toronto by Henry Spencer,
Xmore or less as a supplement to the C News project.
X.LP
X.ce 99
X\(rh None of the above really want to admit it. \(lh
X.ce 0
X.SH BUGS
XThere are plenty, but what do you expect for a text formatter
Xwritten entirely in (old) \fIawk\fR?
X.PP
XThe
X.B \-ms
Xstuff has not been checked out very thoroughly.
!
echo 'awf.1.out':
sed 's/^X//' >'awf.1.out' <<'!'
X
X
XAWF(1)                     Unix Programmer's Manual                     AWF(1)
X
X
XNNNAAAMMMEEE
X     awf - amazingly workable (text) formatter
X
XSSSYYYNNNOOOPPPSSSIIISSS
X     aaawwwfff -mmm_a_c_r_o_s [ file ] ...
X
XDDDEEESSSCCCRRRIIIPPPTTTIIIOOONNN
X     _A_w_f formats the text from the input _f_i_l_e(s) (standard input if  none)  in
X     an  imitation  of _n_r_o_f_f's style with the -mmmaaannn or -mmmsss macro packages.  The
X     -mmm_a_c_r_o option is mandatory and must be `-man' or `-ms'.
X
X     _A_w_f implements the following raw _n_r_o_f_f requests:
X
X          .\"  .ce  .fi  .in  .ne  .pl  .sp
X          .ad  .de  .ft  .it  .nf  .po  .ta
X          .bp  .ds  .ie  .ll  .nr  .ps  .ti
X          .br  .el  .if  .na  .ns  .rs  .tm
X
X     and the following in-text codes:
X
X          \$   \%   \*   \c   \f   \n   \s
X
X     plus the full list of _n_r_o_f_f/_t_r_o_f_f special characters in the  original  V7
X     _t_r_o_f_f manual.
X
X     Many restrictions are present; the behavior in general  is  a  subset  of
X     _n_r_o_f_f's.  Of particular note are the following:
X
X     +o Point sizes do not exist; ...pppsss and \sss are ignored.
X
X     +o Conditionals implement only numeric comparisons on \nnn(((...$$$,  string  com-
X       parisons  between  a macro parameter and a literal, and nnn (always true)
X       and ttt (always false).
X
X     +o The implementation of strings is generally primitive.
X
X     +o Expressions in (e.g.) ...sssppp are fairly general,  but  the  |||,  &&&,  and  :::
X       operators  do  not  exist,  and  the implementation of \www requires that
X       quote (') be used as the delimiter and  simply  counts  the  characters
X       inside (so that, e.g., \w'\(bu' equals 4).
X
X     White space at the beginning of lines, and imbedded  white  space  within
X     lines, is dealt with properly.  Sentence terminators at ends of lines are
X     understood to imply extra space afterward in filled lines.  Tabs are  im-
X     plemented  crudely  and  not quite correctly, although in most cases they
X     work as expected.  Hyphenation is done  only  at  explicit  hyphens,  em-
X     dashes, and _n_r_o_f_f discretionary hyphens.
X
XMMMAAANNN MMMAAACCCRRROOOSSS
X     The -mmmaaannn macro set implements the full V7 manual macros, plus a few semi-
X     random oddballs.  The full list is:
X
X          .B   .DT  .IP  .P   .RE  .SM
X          .BI  .HP  .IR  .PD  .RI  .TH
X          .BR  .I   .LP  .PP  .RS  .TP
X          .BY  .IB  .NB  .RB  .SH  .UC
X
X
XU of Toronto                          13 July 1990                           1
X
X
X
X
XAWF(1)                     Unix Programmer's Manual                     AWF(1)
X
X
X     ...BBBYYY and ...NNNBBB each take a single string argument  (respectively,  an  indi-
X     cation  of authorship and a note about the status of the manual page) and
X     arrange to place it in the page footer.
X
XMMMSSS MMMAAACCCRRROOOSSS
X     The -mmmsss macro set is a substantial subset of the  V7  manuscript  macros.
X     The implemented macros are:
X
X          .AB  .CD  .ID  .ND  .QP  .RS  .UL
X          .AE  .DA  .IP  .NH  .QS  .SH  .UX
X          .AI  .DE  .LD  .NL  .R   .SM
X          .AU  .DS  .LG  .PP  .RE  .TL
X          .B   .I   .LP  .QE  .RP  .TP
X
X     Size changes are recognized but ignored, as are ...RRRPPP and  ...NNNDDD.   ...UUULLL  just
X     prints  its  argument in italics.  ...DDDSSS/...DDDEEE does not do a keep, nor do any
X     of the other macros that normally imply keeps.
X
X     Assignments to the header/footer  string  variables  are  recognized  and
X     implemented,  but  there  is  otherwise  no  control  over  header/footer
X     formatting.  The DDDYYY string variable is available.  The  PPPDDD,  PPPIII,  and  LLLLLL
X     number registers exist and can be changed.
X
XOOOUUUTTTPPPUUUTTT
X     The only output format supported by _a_w_f, in its distributed form, is that
X     appropriate  to  a  dumb  terminal,  using  overprinting for italics (via
X     underlining) and bold.  The _n_r_o_f_f special characters are printed as  some
X     vague   approximation  (it's  sometimes  very  vague)  to  their  correct
X     appearance.
X
X     _A_w_f's knowledge of the output device is established  by  a  device  file,
X     which  is  read  before  the user's input.  It is sought in _a_w_f's library
X     directory, first as dddeeevvv..._t_e_r_m  (where  _t_e_r_m  is  the  value  of  the  TERM
X     environment  variable)  and,  failing that, as dddeeevvv...ddduuummmbbb.  The device file
X     uses special internal commands to set up resolution, special  characters,
X     fonts, etc., and more normal _n_r_o_f_f commands to set up page length etc.
X
XFFFIIILLLEEESSS
X     All in /_u_s_r/_l_i_b/_a_w_f (this can be overridden  by  the  AWFLIB  environment
X     variable):
X
X     common     common device-independent initialization
X     dev.*      device-specific initialization
X     mac.m*     macro packages
X     pass1      macro substituter
X     pass2.base central formatter
X     pass2.m*   macro-package-specific bits of formatter
X     pass3      line and page composer
X
XSSSEEEEEE AAALLLSSSOOO
X     awk(1), nroff(1), man(7), ms(7)
X
X
X
X
X
X
X
XU of Toronto                          13 July 1990                           2
X
X
X
X
XAWF(1)                     Unix Programmer's Manual                     AWF(1)
X
X
XDDDIIIAAAGGGNNNOOOSSSTTTIIICCCSSS
X     Unlike _n_r_o_f_f, _a_w_f complains whenever it sees unknown commands and macros.
X     All diagnostics (these and some internal ones) appear on standard error
X     at the end of the run.
X
XHHHIIISSSTTTOOORRRYYY
X     Written at University of Toronto by Henry Spencer,  more  or  less  as  a
X     supplement to the C News project.
X
X                     => None of the above really want to admit it. =<
X
XBBBUUUGGGSSS
X     There are plenty, but what do you expect for  a  text  formatter  written
X     entirely in (old) _a_w_k?
X
X     The -mmmsss stuff has not been checked out very thoroughly.
X
X
X
X
X
X
X
X
X
X
X
X
X
X
X
X
X
X
X
X
X
X
X
X
X
X
X
X
X
X
X
X
X
X
X
X
X
X
X
X
X
X
XU of Toronto                          13 July 1990                           3
X
X
!
echo 'common':
sed 's/^X//' >'common' <<'!'
X.\" Common startup code, fully device-independent.
X.\" --------------------------------
X.fi
X.ce 0
X.ta +5 +5 +5 +5 +5 +5 +5 +5 +5 +5 +5 +5 +5 +5 +5 +5 +5 +5 +5 +5 +5 +5 +5 +5 +5
X.in 0
X.ti 0
!
echo 'dev.dumb':
sed 's/^X//' >'dev.dumb' <<'!'
X.\" Device-dependent but not macro-set-dependent definitions.
X.\" --------------------------------
X.\" overall formatting initialization
X.\" 12 cpi horizontal exploits 80-column terminal well (6.5i@12 = 78)
X.^r cpi 12 6
X.\" call margin adjustment device-dependent for sake of some unusual cases
X.ad
X.\" page parameters
X.pl 11i
X.ll 6.5i
X.po 0
X.\" --------------------------------
X.\" fonts, and their hyphens, last font change doubled to set up \fP
X.^f R
X.ft R
X.^c hy 1 -
X.^f I
X.ft I
X.^c hy 1 -
X.^f B
X.ft B
X.^c hy 1 -\b-\b-
X.ft R
X.ft R
X.\" --------------------------------
X.\" definitions of nroff special characters
X.\" The character definitions here operate on the "better ugly than invisible"
X.\" principle, and try to approximate the character *somehow*.  They were
X.\" tuned for a Teletype 40 line printer, but should give vaguely plausible
X.\" results on any overprinting ASCII device.
X.\"
X.\" first, things that nroff considered builtins
X.^c \ 1 \\
X.^c e 1 \\
X.^c ' 1 '
X.^c ` 1 `
X.^c - 1 -
X.\" some things seem to assume that \+ is like \-
X.^c + 1 +
X.\" we do not do backslash-space here, it can't be done with .^c, but the
X.\" other forms of space we can do
X.^c 0 1 " 
X.^c | 0
X.^c ^ 0
X.^c & 0
X.\"
X.\" and more normal characters
X.\" note, the hyphenation logic knows about em
X.^c em 2 --
X.\" hy is a special case, see above
X.^c bu 1 +\bo
X.^c sq 2 []
X.^c ru 1 _
X.^c 12 3 1/2
X.^c 14 3 1/4
X.^c 34 3 3/4
X.^c de 1 '\b`
X.^c dg 1 -\b!
X.^c fm 1 '
X.^c ct 1 /\bc
X.^c rg 3 (R)
X.^c co 3 (c)
X.^c pl 1 +
X.^c mi 1 -
X.^c eq 1 =
X.^c ** 1 *
X.^c sc 1 j\bf
X.^c aa 1 '
X.^c ga 1 `
X.^c ul 1 _
X.^c sl 1 /
X.^c *a 1 <\ba
X.^c *b 1 ,\bB
X.^c *g 1 ,\by
X.^c *d 1 S\bo
X.^c *e 1 -\bc
X.^c *z 1 ,\bL
X.^c *y 1 ,\bn
X.^c *h 1 -\b0
X.^c *i 1 ,\bi
X.^c *k 1 <\bK
X.^c *l 1 \\\b>
X.^c *m 1 ,\bu
X.^c *n 1 ,\bv
X.^c *c 1 ,\b3
X.^c *o 1 o
X.^c *p 1 -\bn
X.^c *r 1 p
X.^c *s 1 -\bo
X.^c ts 1 s
X.^c *t 1 ~\bt
X.^c *u 1 u
X.^c *f 1 /\bo
X.^c *x 1 /\b\\
X.^c *q 1 |\bu
X.^c *w 1 u\bw
X.^c *G 2 ~\b|~
X.^c *D 2 _\b/_\b\\
X.^c *H 1 -\bO
X.^c *L 2 /\\
X.^c *C 1 _\b-\b~
X.^c *P 2 ~\b|~\b|
X.^c *S 1 ~\b_\b>
X.^c *U 1 Y
X.^c *F 1 |\bO
X.^c *Q 1 |\bU
X.^c *W 2 _\b(_\b)
X.^c sr 2 \\/
X.^c rn 1 ~
X.^c >= 1 _\b>
X.^c <= 1 _\b<
X.^c == 1 _\b=
X.^c ~= 1 ~\b=
X.^c ap 1 ~
X.^c != 1 /\b=
X.^c -> 2 ->
X.^c <- 2 <-
X.^c ua 1 |\b^
X.^c da 1 |\bv
X.^c mu 1 x
X.^c di 1 -\b:
X.^c +- 1 _\b+
X.^c cu 1 U
X.^c ca 3 (^)
X.^c sb 2 (_\b~
X.^c sp 2 _\b~)
X.^c ib 2 (~\b_\b=
X.^c ip 2 ~\b_\b=)
X.^c if 2 oo
X.^c pd 1 3\bo
X.^c gr 1 ~\bV
X.^c no 1 -
X.^c is 1 '\b,\bI
X.^c pt 2 oc
X.^c es 1 /\bO
X.^c mo 1 -\bC
X.^c br 1 |
X.^c dd 1 I\b|
X.^c rh 1 =\b>
X.^c lh 1 =\b<
X.^c bs 4 (:-)
X.^c or 1 |
X.^c ci 1 O
X.^c lt 1 ~\b(
X.^c lb 1 _\b(
X.^c rt 1 ~\b)
X.^c rb 1 _\b)
X.^c lk 1 -\b(
X.^c rk 1 -\b)
X.^c bv 1 |
X.^c lf 1 _\b[
X.^c rf 1 _\b]
X.^c lc 1 ~\b[
X.^c rc 1 ~\b]
!
echo 'mac.man':
sed 's/^X//' >'mac.man' <<'!'
X.\"-----------------
X.de TH
X.ds LH "\\$1(\\$2)
X.ds CH "Unix Programmer's Manual
X.ds RH "\\$1(\\$2)
X.ds LF "
X.ds CF "\\$3
X.ds RF "%
X..
X.\"-----------------
X.de NB
X.ds nb "\\$1
X.lF
X..
X.\"-----------------
X.de BY
X.ds by "\\$1
X.lF
X..
X.\"-----------------
X.de UC
X.BY "\\$1BSD"
X..
X.\"-----------------
X.\" common initialization for headers and paragraphs:  .In need
X.de In
X.ne \\$1
X.sp \\n(PDu
X.fi
X.in \\n(inu
X.ti 0
X.it
X.ft R
X.ns
X..
X.\"-----------------
X.de SH
X.In 6
X.in 0
X.ft B
X\&\\$1 \\$2 \\$3 \\$4 \\$5 \\$6 \\$7 \\$8 \\$9
X.ft R
X.nr in 5n
X.in \\n(inu
X..
X.\"-----------------
X.de LP
X.In 4
X..
X.\"-----------------
X.de PP
X.LP
X..
X.\"-----------------
X.de P
X.LP
X..
X.\"-----------------
X.de HP
X.In 4
X.ti 0-\\n(tpu
X..
X.\"-----------------
X.de TP
X.In 4
X.if \\n(.$>0 .nr tp \\$1n
X.in \\n(inu+\\n(tpu
X.ti 0-\\n(tpu
X.it 1 tP
X..
X.\"-----------------
X.de IP
X.ie \\n(.$>1 .TP "\\$2"
X.el .TP
X\&\\$1
X..
X.\"-----------------
X.de I
X.ft I
X.it 1 fP
X.if \\n(.$>0 \&\\$1 \\$2 \\$3 \\$4 \\$5 \\$6 \\$7 \\$8 \\$9
X..
X.\"-----------------
X.de B
X.ft B
X.it 1 fP
X.if \\n(.$>0 \&\\$1 \\$2 \\$3 \\$4 \\$5 \\$6 \\$7 \\$8 \\$9
X..
X.\"-----------------
X.de IR
X\&\\fI\\$1\\fR\\$2\\fI\\$3\\fR\\$4\\fI\\$5\\fR\\$6\\fI\\$7\\fR\\$8\\fI\\$9\\fR
X..
X.\"-----------------
X.de RI
X\&\\$1\\fI\\$2\\fR\\$3\\fI\\$4\\fR\\$5\\fI\\$6\\fR\\$7\\fI\\$8\\fR\\$9
X..
X.\"-----------------
X.de BR
X\&\\fB\\$1\\fR\\$2\\fB\\$3\\fR\\$4\\fB\\$5\\fR\\$6\\fB\\$7\\fR\\$8\\fB\\$9\\fR
X..
X.\"-----------------
X.de RB
X\&\\$1\\fB\\$2\\fR\\$3\\fB\\$4\\fR\\$5\\fB\\$6\\fR\\$7\\fB\\$8\\fR\\$9
X..
X.\"-----------------
X.de BI
X\&\\fB\\$1\\fI\\$2\\fB\\$3\\fI\\$4\\fB\\$5\\fI\\$6\\fB\\$7\\fI\\$8\\fB\\$9\\fR
X..
X.\"-----------------
X.de IB
X\&\\fI\\$1\\fB\\$2\\fI\\$3\\fB\\$4\\fI\\$5\\fB\\$6\\fI\\$7\\fB\\$8\\fI\\$9\\fR
X..
X.\"-----------------
X.de SM
X.\" no-op
X.if \\n(.$>0 \&\\$1 \\$2 \\$3 \\$4 \\$5 \\$6 \\$7 \\$8 \\$9
X..
X.\"-----------------
X.de DT
X.ta +5 +5 +5 +5 +5 +5 +5 +5 +5 +5 +5 +5 +5 +5 +5 +5 +5 +5 +5 +5 +5 +5 +5 +5 +5
X..
X.\"-----------------
X.de RS
X.in +5n
X..
X.\"-----------------
X.de RE
X.in -5n
X..
X.\"-----------------
X.de PD
X.ie \\n(.$=0 .nr PD 0.3v
X.el .nr PD \\$1n
X..
X.\"-----------------
X.\" misc. initialization
X.nr tp 5n
X.PD
X.ds R \(rg
X.ds S "
X.ds Tm (TM)
!
echo 'mac.ms':
sed 's/^X//' >'mac.ms' <<'!'
X.\"-----------------
X.de R
X.ie \\n(.$=0 .ft R
X.el \\fR\\$1\\fP\\$2
X..
X.\"-----------------
X.de I
X.ie \\n(.$=0 .ft I
X.el \\fI\\$1\\fP\\$2
X..
X.\"-----------------
X.de B
X.ie \\n(.$=0 .ft B
X.el \\fB\\$1\\fP\\$2
X..
X.\"-----------------
X.de UX
X.\" old-fashioned capitalization -- I've never gotten used to the new one
X\\$2Unix\\$1
X..
X.\"-----------------
X.de DA
X.ds DY "\\$1 \\$2 \\$3
X.\" keep trailing spaces out of CF
X.if \\n(.$=1 .ds CF "\\$1
X.if \\n(.$=2 .ds CF "\\$1 \\$2
X.if \\n(.$>2 .ds CF "\\$1 \\$2 \\$3
X..
X.\"-----------------
X.de ND
X.\" it's our default, ignore it
X..
X.\"-----------------
X.de TL
X.rs
X.sp 5
X.ft B
X.ce 9999
X..
X.\"-----------------
X.de AU
X.sp 2
X.ft R
X..
X.\"-----------------
X.de AI
X.sp
X.ft R
X..
X.\"-----------------
X.de AB
X.sp 2
X.ce 0
X.ll -7n
X.in +7n
X..
X.\"-----------------
X.de AE
X.sp
X.ll
X.in
X..
X.\"-----------------
X.\" common initialization for headers and paragraphs:  .In need extraspace
X.de In
X.ne \\$1
X.sp \\n(Tsu
X.nr Ts 0
X.ie \\n(.$>1 .nr iN \\$2v
X.el .nr iN 0
X.sp \\n(PDu+\\n(iNu
X.ce 0
X.ft R
X.in \\n(inu
X.ll \\n(LLu
X.ns
X.fi
X.ti 0
X..
X.\"-----------------
X.de SH
X.nr in 0
X.In 6 1
X.ft B
X..
X.\"-----------------
X.de NH
X.nr in 0
X.In 6 1
X.ft B
X.\" punt to awk code to get the header numbering right
X.nH \\$1
X.\" and pick up the result
X\&\\*(Nh
X..
X.\"-----------------
X.de LP
X.In 4
X..
X.\"-----------------
X.de PP
X.In 4
X.ti \\n(PIu
X..
X.\"-----------------
X.de TP
X.In 4
X.if \\n(.$>0 .nr tp \\$1n
X.in \\n(inu+\\n(tpu
X.ti 0-\\n(tpu
X.it 1 tP
X..
X.\"-----------------
X.de IP
X.ie \\n(.$>1 .TP "\\$2"
X.el .TP
X\&\\$1
X..
X.\"-----------------
X.de QP
X.In 4
X.in +5n
X.ll -5n
X..
X.\"-----------------
X.de QS
X.nr in +5n
X.nr LL -5n
X.In 4
X..
X.\"-----------------
X.de QE
X.nr in -5n
X.nr LL +5n
X.In 4
X..
X.\"-----------------
X.de DS
X.In 5
X.if '\\$1'C' .ce 9999
X.if '\\$1'' .in +5n
X.nf
X..
X.\"-----------------
X.de CD
X.In 5
X.ce 9999
X.nf
X..
X.\"-----------------
X.de LD
X.In 5
X.nf
X..
X.\"-----------------
X.de ID
X.In 5
X.in +5n
X.nf
X..
X.\"-----------------
X.de DE
X.In 3
X..
X.\"-----------------
X.de RS
X.nr in +5n
X.in \\n(inu
X..
X.\"-----------------
X.de RE
X.nr in -5n
X.in \\n(inu
X..
X.\"-----------------
X.de UL
X\&\\fI$1\\fP
X..
X.\"-----------------
X.de RP
X..
X.\"-----------------
X.de LG
X..
X.\"-----------------
X.de SM
X..
X.\"-----------------
X.de NL
X..
X.\"-----------------
X.\" the -ms accent strings
X.ds ' "'\b
X.ds ` "`\b
X.ds : ":\b
X.ds ^ "^\b
X.ds ~ "~\b
X.ds C "v\b
X.ds , ",\b
X.\" post-title spacing
X.nr Ts 4v
X.\" and parameter setup
X.nr LL 6i
X.ll \n(LLu
X.nr PD 0.3v
X.nr PI 5n
!
echo 'pass1':
sed 's/^X//' >'pass1' <<'!'
X# first pass:  macro expansion and .if
X# We support macros, conditionals (of three quite limited forms), and macro
X# argument substitution.
XBEGIN {
X	curmacro = ""
X	macros[""] = 0		# just to make it an array
X	macrolen[""] = 0	# just to make it an array
X	macrotext[0] = ""	# just to make it an array
X	args[""] = ""		# just to make it an array
X	ntext = 1		# first slot in macrotext; cannot be 0
X	nroffset = 0		# offset between NR and "real" line numbers
X	inname = "?"		# input filename
X	sp = 0			# stack "pointer" (number of stacked macros)
X	maxsp = 25		# limit on nesting depth
X	macrostack[sp] = ""	# to make it an array
X	nleftstack[sp] = ""	# to make it an array
X	ptrstack[sp] = ""	# to make it an array
X	nargstack[sp] = ""	# to make it an array
X	argstack[sp] = ""	# to make it an array
X	condstack[sp] = ""	# to make it an array
X}
X/^\.\^#/ {			# filename and line no of next line: .^# no fn
X	nroffset = (NR+1) - $2
X	inname = $3
X	print
X	next
X}
X/^\.de/ {			# macro start
X	curmacro = "." $2
X	macros[curmacro] = ntext
X	macrostart = ntext
X	next
X}
Xcurmacro != "" && $0 !~ /^\.\.$/ {	# macro text - \\ becomes \
X	if ($0 !~ /\\/)		# quick case, no backslashes
X		line = $0
X	else {
X		line = ""
X		for (n = 1; n <= length; n++) {
X			if (substr($0, n, 2) == "\\\\")
X				n++
X			line = line substr($0, n, 1)
X		}
X	}
X	macrotext[ntext++] = line
X	next
X}
Xcurmacro != "" && $0 ~ /^\.\.$/ {	# macro end
X	macrolen[curmacro] = ntext - macrostart
X	curmacro = ""
X	print ".^#", NR - nroffset + 1, inname
X	next
X}
X$0 ~ /^\./ && ( macros[$1] != 0 || $0 ~ /^\.(i[ef]|el)/ ) {
X	# something that needs attention
X	print ".^=", NR - nroffset, inname
X	line = $0
X	nleft = 0
X	macro = "<none>"
X	nargs = 0
X
X	while (line != "") {
X		# conditionals; note that 1-n is !n (awk doesn't have !)
X		invert = 0
X		if (line ~ /^\.i[ef] !/)
X			invert = 1
X		prevcond = cond
X		cond = 0
X		if (line !~ /^\.(i[ef]|el)/) {		# not conditional
X			cond = 1
X			iflen = 0
X		} else if (line ~ /^\.i[ef] !?\\n\(\.\$[<=>][0-9] /) {
X			# arithmetic comparison on arg count
X			iflen = length(".if .n(.$=x ") + invert
X			n = substr(line, iflen-1, 1) + 0
X			op = substr(line, iflen-2, 1)
X			if (op == "=" && nargs == n)
X				cond = 1
X			else if (op == "<" && nargs < n)
X				cond = 1
X			else if (op == ">" && nargs > n)
X				cond = 1
X		} else if (line ~ /^\.i[ef] !?'\\\$[0-9]'[^']*' /) {
X			# string equality on argument
X			iflen = length(".if '.$n'") + invert
X			n = substr(line, iflen-1, 1)+0
X			if (n <= nargs)
X				s1 = args[n]
X			else
X				s1 = ""
X			i = index(substr(line, iflen+1), "'")
X			s2 = substr(line, iflen+1, i-1)
X			iflen += i+1
X			if (s1 == s2)
X				cond = 1
X		} else if (line ~ /^\.i[ef] !?[nt] /) {
X			# nroff vs troff
X			iflen = length(".if n ") + invert
X			if (substr(line, iflen-1, 1) == "n")
X				cond = 1
X		} else if (line ~ /^\.el /) {
X			cond = 1 - prevcond
X			iflen = length(".el ")
X		} else {
X			line = ".tm unknown .if/.ie form: " line
X			cond = 1
X			iflen = 0
X		}
X		if (invert)
X			cond = 1 - cond
X		if (cond && iflen > 0)		# trim true conditional off
X			line = substr(line, iflen+1)
X
X		# do argument substitution, if necessary
X		if (cond && line ~ /\\\$/) {
X			orig = line
X			line = ""
X			for (pos = index(orig, "\\$"); pos > 0; \
X						pos = index(orig, "\\$")) {
X				if (pos > 1)
X					line = line substr(orig, 1, pos-1)
X				c = substr(orig, pos+2, 1)
X				if (c ~ /[0-9]/ && c+0 <= nargs)
X					line = line args[c+0]
X				orig = substr(orig, pos+3)
X			}
X			line = line orig	# the remnant
X		}
X
X		# is it an nroff command?
X		if (cond && line ~ /^\./) {
X			cmd = substr(line, 1, 3)
X			while (cmd ~ / $/)
X				cmd = substr(cmd, 1, length(cmd)-1)
X		} else
X			cmd = ""
X
X		# deal with it
X		if (!cond)
X			nop = 0		# nothing
X		else if (cmd == "" || macros[cmd] == 0)
X			print line	# not a nested macro
X		else if (sp >= maxsp)
X			print ".tm macros nested too deeply (" sp " levels)"
X		else {			# nesting
X			# stack old one
X			sp++
X			nleftstack[sp] = nleft
X			ptrstack[sp] = ptr
X			macrostack[sp] = macro
X			nargstack[sp] = nargs
X			condstack[sp] = cond
X			for (i = 1; i <= nargs; i++)
X				argstack[sp ":" i] = args[i]
X
X			# start new one, mostly pulling arguments apart
X			macro = cmd
X			nleft = macrolen[macro]
X			ptr = macros[macro]
X			cond = 0
X			argno = 1
X			pos = length(macro) + 1
X			for (;;) {
X				while (substr(line, pos, 1) ~ /[ \t]/)
X					pos++
X				if (pos > length(line))
X					break		# NOTE BREAK OUT
X				arg = ""
X				if (substr(line, pos, 1) == "\"") {
X					pos++
X					while (substr(line, pos, 1) ~ /[^"]/) {
X						arg = arg substr(line, pos, 1)
X						pos++
X					}
X					pos++
X				} else
X					while (substr(line, pos, 1) ~ /[^ \t]/) {
X						arg = arg substr(line, pos, 1)
X						pos++
X					}
X				args[argno++] = arg
X			}
X			nargs = argno - 1
X		}
X
X		# clean up any completed macros
X		while (nleft <= 0 && sp > 0) {
X			nleft = nleftstack[sp]
X			ptr = ptrstack[sp]
X			macro = macrostack[sp]
X			nargs = nargstack[sp]
X			cond = condstack[sp]
X			for (i = 1; i <= nargs; i++)
X				args[i] = argstack[sp ":" i]
X			sp--
X		}
X
X		# finally, get next line
X		if (nleft > 0) {
X			line = macrotext[ptr++]
X			nleft--
X		} else
X			line = ""	# signal loop to terminate
X	}
X
X	print ".^#", NR - nroffset + 1, inname
X	next
X}
X{
X	# it's ordinary
X	print
X}
!
echo 'pass2.base':
sed 's/^X//' >'pass2.base' <<'!'
X# second pass:  interpretation of directives
X# A macro-set-specific portion gets interpolated into this at the "#include"
X# line.  As a minimum, it must deal with .^b and .^e and with any input traps.
XBEGIN {
X	# stuff for output to third pass
X	nobreak = -1
X	dobreak = -2
X	message = -3
X	OFS = "\t"
X
X	# special-character table; this one character needs to be hardcoded
X	chars[" "] = " " ; charwidths[" "] = 1
X
X	debug["e"] = 0		# just to make it an array
X	strings["a"] = ""	# just to make it an array
X	numbers["a"] = 0	# just to make it an array
X	hyphens["a"] = ""	# just to make it an array
X	hyphenwidths["a"] = ""	# just to make it an array
X
X	# stuff for expression decoding
X	signfactor["+"] = 1
X	signfactor["-"] = -1
X	scale["i"] = 240
X	scale["c"] = 240*50/127
X	scale["P"] = 240/6
X	# we get m, n, and v when we see .^r
X	scale["p"] = 240/72
X	scale["u"] = 1
X
X	# stuff for basic parameters that just get passed to third pass
X	parms["in"] = 0		# just to make it an array
X	prevparms["in"] = 0	# just to make it an array
X	setcmd["ll"] = "linelen"
X	setcmd["in"] = "indent"
X	setcmd["ti"] = "tempindent"
X	setcmd["po"] = "pageoffset"
X	setcmd["pl"] = "pagelen"
X
X	# did last word end with \c ?  (in which case, it's still in "word")
X	backc = 0
X
X	# stuff for error reporting
X	il = 0			# input line number
X	lockil = 0		# il is locked, we're inside a macro
X	inname = "?"		# input filename
X	msg = message "\t"	# later picks up filename etc.
X
X	# current input trap
X	afternext = ""
X}
X{
X	if (!lockil)
X		il++
X	msg = message "\t" inname "," il ": "
X	# fallthrough
X}
X/^[ \t]*$/ {			# empty line
X	print dobreak, "space"
X	next
X}
X/^[ \t]/ {			# line starting with white space
X	print dobreak, "flush"
X	print 0, ""		# empty word
X	# fallthrough
X}
X/^[^.]/ {			# text
X	# dispose of the easy case
X	if (font == "R" && $0 !~ /\\|\t|-|  / && !backc && afternext == "") {
X		for (i = 1; i <= NF; i++)
X			print length($i), $i
X		if ($0 ~ /[.!?:][\])'"*]*$/)
X			print nobreak, "gap", 2
X		if (centering > 0) {
X			print dobreak, "center"
X			centering--
X		} else if (!fill)
X			print dobreak, "flush"
X		next
X	}
X
X	# the hard case, needs a character-by-character scan
X	s = $0 " "		# the space flushes the last word
X	n = 1			# current position in s
X	inword = 0		# have we been processing a word?
X	period = ""		# "." if this word ended a sentence
X	nsp = 0			# count of spaces seen so far
X	tabpos = 0		# which tab position was used last
X	while (n <= length(s)) {
X		c = substr(s, n, 1)
X
X		# handle state transitions
X		if (c == " " || c == "\t") {
X			if (inword) {		# ends word
X				if (!backc) {
X					print wordlen, word
X					if (usedhyphen)
X						print nobreak, "nohyphen"
X				}
X				inword = 0
X				nsp = 0
X			}
X		} else {
X			if (!inword) {		# begins word
X				if (!backc) {
X					word = ""
X					wordlen = 0
X					usedhyphen = 0
X				}
X				backc = 0
X				inword = 1
X				if (nsp > 1)
X					print nobreak, "gap", nsp
X			}
X		}
X
X		# deal with the character
X		if (c == " ") {
X			nsp++
X			if (n != length(s))	# not the trailing flusher
X				period = ""
X		} else if (c == "\t") {
X			# not really right, should be based on input position
X			# also, one space following tab gets ignored
X			tabpos++
X			if (tabpos <= ntabs)
X				print nobreak, "tabto", tabs[tabpos]
X			nsp = 0
X			period = ""
X		} else if (c == "-" && wordlen > 0) {
X			# hyphen within word
X			print wordlen, word, hyphenwidths[font]
X			print nobreak, "userhyphen", hyphens[font], hyphenwidths[font]
X			word = ""
X			wordlen = 0
X			period = ""
X			usedhyphen = 1
X		} else if (c != "\\") {
X			# ordinary character
X			if (font == "B")
X				word = word c "\b" c "\b" c
X			else if (font == "I" && c ~ /[a-zA-Z0-9]/)
X				word = word "_\b" c
X			else
X				word = word c
X			wordlen++
X			if (c ~ /[.!?:]/)
X				period = "."
X			else if (c !~ /[\])'"*]/)
X				period = ""
X		} else {			# backslash
X			n++
X			c = substr(s, n, 1)
X			if (c == "f") {
X				# font change
X				n++
X				code = substr(s, n, 1)
X				if (code == "(") {
X					n++
X					code = substr(s, n, 2)
X					n++
X				}
X				if (code == "P")
X					font = prevfont
X				else if (fontok[code] == "")
X					print msg "unknown font `" code "'"
X				else {
X					prevfont = font
X					font = code
X				}
X			} else if (c == "n") {
X				# number-register value
X				n++
X				code = substr(s, n, 1)
X				if (code == "(") {
X					n++
X					code = substr(s, n, 2)
X					n++
X				}
X				s = substr(s, 1, n) numbers[code] substr(s, n+1)
X			} else if (c == "s") {
X				# size change
X				n++
X				if (substr(s, n, 1) ~ /[0-9]/)
X					n++
X				# just ignore it
X			} else if (c == "c")
X				# word continuation
X				backc = 1
X			else if (c == "*") {
X				# string-variable value
X				n++
X				code = substr(s, n, 1)
X				if (code == "(") {
X					n++
X					code = substr(s, n, 2)
X					n++
X				}
X				s = substr(s, 1, n) strings[code] substr(s, n+1)
X			} else if (c == "%") {
X				# discretionary hyphen
X				if (wordlen > 0) {
X					print wordlen, word, hyphenwidths[font]
X					print nobreak, "hyphen", hyphens[font], hyphenwidths[font]
X					word = ""
X					wordlen = 0
X					usedhyphen = 1
X				}
X			} else if (c == "(" && substr(s, n+1, 2) == "em" && \
X							chars["em"] != "") {
X				# em-dash, special case due to hyphenation
X				n += 2
X				emw = charwidths["em"]
X				print wordlen, word, emw
X				print nobreak, "userhyphen", chars["em"], emw
X				word = ""
X				wordlen = 0
X				period = ""
X				usedhyphen = 1
X			} else {
X				# special-character name
X				code = c
X				if (code == "(") {
X					n++
X					code = substr(s, n, 2)
X					n++
X				}
X				word = word chars[code]
X				wordlen += charwidths[code]
X				period = ""
X			}
X		}
X
X		# on to the next character, at last
X		n++
X	}
X
X	# end-of-line processing
X	if (!backc) {
X		if (period == ".")
X			print nobreak, "gap", 2
X		if (centering > 0) {
X			print dobreak, "center"
X			centering--
X		} else if (!fill)
X			print dobreak, "flush"
X	}
X
X	# if no input trap, we're done
X	if (afternext == "")
X		next
X
X	# if there is an input trap, fall into the macro-dependent section
X}
X#
X#
X#
X# at this point we plug in the macro-specific stuff, keyed on the next line
X#include			note that this is an awk comment
X#
X#
X#
X/^\.it/ {			# plant an input trap, sort of
X	if (NF > 1 && $2 != 1)
X		print msg ".it first argument must be 1"
X	if (NF > 2)
X		afternext = afternext "," $3
X	else
X		afternext = ""
X	next
X}
X/^\.\^r cpi / {			# set resolutions, in cpi:  .^r cpi hor vert
X	scale["m"] = 240/$3
X	scale["n"] = 240/$3
X	scale["v"] = 240/$4
X	next
X}
X/^\.(ta|ll|in|ti|po|ne|sp|pl|nr)/ {	# expression processing
X	# sort out default scale factor
X	if ($1 ~ /^\.(ne|sp|pl)/)
X		exprscale = "v"
X	else if ($1 ~ /^\.(nr)/)
X		exprscale = "u"
X	else
X		exprscale = "n"
X
X	# which argument should we start with?
X	offset = length($1) + 1
X	if ($1 == ".nr")
X		offset += length($2) + 1
X
X	# beginning of debugging message
X	if (debug["e"])
X		printf "%s ", msg substr($0, 1, offset)
X
X	# do the expressions
X	s = substr($0, offset+1)
X	n = 1
X	while (s != "") {
X		while (s ~ /^[ \t]/)
X			s = substr(s, 2)
X
X		# an initial sign is magic
X		ssign = ""
X		if (s ~ /^[+-]/) {
X			ssign = substr(s, 1, 1)
X			s = substr(s, 2)
X		}
X		s = "+" s	# this is an un-magic addition operator
X
X		# process terms
X		sval = 0
X		# there is no portable way to put a slash in a regexp
X		while (s ~ /^[+*%)-]/ || substr(s, 1, 1) == "/") {
X			# figure out what it is and what it contributes
X			if (debug["e"] > 1)
X				print "s=`" s "'"
X			termop = substr(s, 1, 1)
X			s = substr(s, 2)
X			termscale = exprscale
X			if (termop == ")") {
X				if (debug["e"] > 1)
X					print "pop " valstack[vsp] " " opstack[vsp] " with " sval
X				termval = sval
X				sval = valstack[vsp]
X				termop = opstack[vsp]
X				vsp--
X				termscale = "u"
X			} else if (s ~ /^\(/) {
X				if (debug["e"] > 1)
X					print "push " sval " " termop
X				vsp++
X				valstack[vsp] = sval
X				opstack[vsp] = termop
X				sval = 0
X				termop = "+"	# dummy op and value
X				termval = 0
X				s = "+" substr(s, 2)
X			} else if (s ~ /^\\w/) {
X				delim = substr(s, 3, 1)
X				s = substr(s, 4)
X				endp = index(s, delim)
X				if (endp == 0)
X					endp = length(s) + 1
X				termval = (endp - 1) * scale["n"]	# crude
X				s = substr(s, endp+1)
X			} else if (s ~ /^\\n/) {
X				if (s ~ /^\\n\(/) {
X					code = substr(s, 4, 2)
X					s = substr(s, 6)
X				} else {
X					code = substr(s, 3, 1)
X					s = substr(s, 4)
X				}
X				termval = numbers[code]
X			} else if (s ~ /^[0-9.]/) {
X				for (endp = 1; endp <= length(s); endp++)
X					if (substr(s, endp, 1) !~ /[0-9.]/)
X						break
X				termval = substr(s, 1, endp-1) + 0
X				s = substr(s, endp)
X			}
X
X			# add it in, with scaling factor
X			c = substr(s, 1, 1)
X			if (scale[c] != "") {
X				termval *= scale[c]
X				s = substr(s, 2)
X			} else
X				termval *= scale[termscale]
X			if (termop == "+")
X				sval += termval
X			else if (termop == "-")
X				sval -= termval
X			else if (termop == "*")
X				sval *= termval
X			else if (termop == "/")
X				sval = int(sval) / int(termval)
X			else if (termop == "%")
X				sval = int(sval) % int(termval)
X		}
X
X		# remember the value, print if debugging
X		expr[n] = sval
X		exprsign[n] = ssign
X		iexpr[n] = signfactor[ssign] * sval	# convenience
X		if (debug["e"])
X			printf "%s ", ssign "(" sval ")"
X
X		# proceed to next, skipping trash if necessary
X		while (s ~ /^[^ \t]/)
X			s = substr(s, 2)
X		n++
X	}
X
X	# final cleanup
X	nexprs = n - 1
X	if (debug["e"])
X		printf "\n"
X
X	# fallthrough
X}
X/^\.(ll|in|ti|po|pl)/ {		# common code for set-parameter requests
X	# relies on expression processing, including setting of exprscale
X	name = substr($1, 2, 2)
X	n = parms[name]
X	if (nexprs == 0)
X		n = prevparms[name]
X	else if (exprsign[1] == "" || name == "ti")
X		n = expr[1] / scale[exprscale]
X	else
X		n += iexpr[1] / scale[exprscale]
X	prevparms[name] = parms[name]
X	parms[name] = int(n)
X	print dobreak, setcmd[name], parms[name]
X	next
X}
X/^\.nr/ {
X	# relies on expression processing
X	n = numbers[$2]
X	if (exprsign[1] == "")
X		n = expr[1]
X	else
X		n += iexpr[1]
X	numbers[$2] = int(n)
X	next
X}
X/^\.ne/ {
X	# relies on expression processing
X	print dobreak, "need", int(expr[1]/scale["v"] + 0.99)
X	next
X}
X/^\.sp/ {
X	# relies on expression processing
X	if (nexprs == 0)
X		i = 1
X	else
X		i = int(expr[1]/scale["v"] + 0.99)
X	for (; i > 0; i--)
X		print dobreak, "space"
X	next
X}
X/^\.ta/ {
X	# relies on expression processing
X	tabstop = 0
X	for (n = 1; n <= nexprs; n++) {
X		if (exprsign[n] == "")
X			tabstop = expr[n]
X		else
X			tabstop += iexpr[n]
X		tabs[n] = int(tabstop/scale["n"])
X	}
X	ntabs = nexprs
X	next
X}
X/^\.ft/ {
X	if (NF > 1)
X		code = $2
X	else
X		code = "P"
X
X	if (code == "P")
X		font = prevfont
X	else if (fontok[code] == "")
X		print msg "unknown font `" code "'"
X	else {
X		prevfont = font
X		font = code
X	}
X	next
X}
X/^\.br/ {
X	print dobreak, "flush"
X	next
X}
X/^\.ds/ {
X	# note, macro-set-specific code often looks at .ds as well
X	if ($3 !~ /^"/)
X		strings[$2] = $3
X	else
X		strings[$2] = substr($0, index($0, "\"")+1)
X	next
X}
X/^\.ns/ {
X	print nobreak, "nospace"
X	next
X}
X/^\.rs/ {
X	print nobreak, "yesspace"
X	next
X}
X/^\.ad/ {
X	print nobreak, "both"
X	next
X}
X/^\.na/ {
X	print nobreak, "left"
X	next
X}
X/^\.nf/ {
X	fill = 0
X	print dobreak, "flush"
X	next
X}
X/^\.fi/ {
X	fill = 1
X	print dobreak, "flush"
X	next
X}
X/^\.\^x/ {			# direct errors to this file:  .^x filename
X	print nobreak, "errsto", $2
X	next
X}
X/^\.\^c/ {			# define character:  .^c name width text
X	if ($4 ~ /^"/)
X		s = substr($0, index($0, "\"")+1)
X	else
X		s = $4
X	ns = ""
X	while ((n = index(s, "\\")) > 0) {
X		if (n > 1)
X			ns = ns substr(s, 1, n-1)
X		n++
X		c = substr(s, n, 1)
X		if (c == "\\")
X			ns = ns "\\"
X		else if (c == "b")
X			ns = ns "\b"
X		s = substr(s, n+1)
X	}
X	ns = ns s
X	if ($2 == "hy") {
X		hyphens[font] = ns
X		hyphenwidths[font] = $3
X	} else {
X		chars[$2] = ns
X		charwidths[$2] = $3
X	}
X	next
X}
X/^\.\^f/ {			# this font is okay:  .^f fontname
X	# someday, this might take font-change codes as further arguments
X	fontok[$2] = "yes"
X	next
X}
X/^\.tm/ {
X	print msg $0
X	next
X}
X/^\.ps/ {
X	# ignore
X	next
X}
X/^\.ce/ {
X	if (NF > 1)
X		centering = $2
X	else
X		centering = 1
X	next
X}
X/^\.bp/ {
X	print dobreak, "need", 999
X	next
X}
X/^\.\^d/ {			# debug control:  .^d debugvar value
X	debug[$2] = $3
X	next
X}
X/^\.\^#/ {			# set line number of next line: .^# no file
X	il = $2 - 1
X	lockil = 0
X	inname = $3
X	next
X}
X/^\.\^=/ {			# lock line number to value:  .^= no file
X	il = $2
X	lockil = 1
X	inname = $3
X	next
X}
X/^\.\\"/ {			# comment
X	next
X}
X/^\./ {
X	print msg "command `" $1 "' unsupported or unknown"
X}
XEND {
X	print dobreak, "need", 999	# flush page
X}
!
echo 'pass2.man':
sed 's/^X//' >'pass2.man' <<'!'
X/^\.\^b/ {			# initialization
X	print nobreak, "fph", 1
X	next
X}
X/^[^.]/ {			# text line -- reached only for input traps
X	if (afternext ~ /,fP/)
X		font = prevfont
X	if (afternext ~ /,tP/)
X		print dobreak, "toindent"
X	afternext = ""
X	next
X}
X/^\.ds/ {			# to catch special strings
X	if ($3 !~ /^"/)
X		v = $3
X	else
X		v = substr($0, index($0, "\"")+1)
X	if ($2 ~ /^[LCR][HF]$/)
X		print nobreak, $2, v
X	# fallthrough to normal .ds processing in macro-independent stuff
X}
X/^\.lF/ {
X	# special footer fiddling
X	if (strings["by"] != "" && strings["nb"] != "")
X		lf = strings["by"] "; " strings["nb"]
X	else
X		lf = strings["by"] strings["nb"]
X	print nobreak, "LF", lf
X	next
X}
X/^\.\^e/ {			# finalization
X	next
X}
!
echo 'pass2.ms':
sed 's/^X//' >'pass2.ms' <<'!'
X/^\.\^b/ {			# initialization
X	nhnos[1] = 0
X	next
X}
X/^[^.]/ {			# text line -- reached only for input traps
X	if (afternext == ",tP")
X		print dobreak, "toindent"
X	afternext = ""
X	next
X}
X/^\.nH/ {			# fooling around for numbered headings
X	no = 1
X	if (NF > 1)
X		no = $2
X	if (no == 0) {
X		nhnos[1] = 0
X		no = 1
X	}
X	nhnos[no]++
X	for (n in nhnos)
X		if (n > no)
X			nhnos[n] = 0
X	s = ""
X	for (n = 1; n <= no; n++)
X		s = s nhnos[n] "."
X	strings["Nh"] = s		# result in string for macro to grab
X	next
X}
X/^\.ds/ {			# to catch special strings
X	if ($3 !~ /^"/)
X		v = $3
X	else
X		v = substr($0, index($0, "\"")+1)
X	if ($2 ~ /^[LCR][HF]$/)
X		print nobreak, $2, v
X	# fallthrough to normal .ds processing in macro-independent stuff
X}
X/^\.\^e/ {			# finalization
X	next
X}
!
echo 'pass3':
sed 's/^X//' >'pass3' <<'!'
X# third pass:  setting lines and pages
X# The input language of this pass is basically <width, word> pairs, where
X# "word" may have imbedded strangeness (backspaces, etc.) for font changes
X# and special characters.  Zero width is not special.  A third field may
X# appear to indicate that this is a fragment of a word that can be
X# hyphenated; the third field is the width of the hyphen that would have
X# to be added if the line broke after this fragment.
X# Negative widths denote special operations.  -3 is an error message, the
X# second field being the message.  -1 and -2 are control messages to this
X# pass, the difference being whether a break is implied or not.  The second
X# field is a message type string; more fields may appear as arguments.  The
X# semantics of control messages are often -- but not always! -- similar to
X# those of troff commands.  For example, "linelen" is .ll, but "center" is
X# not semantically equivalent to .ce -- it is related but more primitive.
XBEGIN {
X	# input and output details
X	FS = "\t"
X	nobreak = -1
X	dobreak = -2
X	message = -3
X	errs = "awf.errs"	# default only, normally changed by "errsto"
X
X	# page setup -- some are defaults only, normally altered by pass 2
X	nextlineno = 1
X	thispageno = 1
X	topmargin = 5
X	botmargin = 5
X	ind = 0			# current indent
X	tmpind = 0
X	pageoffset = ""		# string to emit at start of each line
X	nospacemode = 1
X	hdrs["CH"] = "- % -"
X	nop = split("LH,CH,RH,LF,CF,RF", hdrnames, ",")
X	fph = 0			# print header on first page?
X	fill = 1
X	adj = "both"
X	pagelen = 66
X	linelen = 78
X
X	# line-builder setup
X	line = ""		# line so far, without padding
X	paddable = ""		# x means corresp. char in "line" is paddable
X	thislinelen = -1	# -1 means nothing there
X	cont = " "		# thing to append to continue line
X	contlen = 1
X	eol = ""		# thing to append to break line
X	eollen = 0
X	padfrom = "R"		# "L" or "R", alternating for river avoidance
X
X	# many spaces, so we can use substr to get any number we need
X	sps = "                                                     "
X	sps = sps sps sps sps sps sps sps sps sps sps
X}
X{
X	# process word, if any, causing a break if appropriate
X	if ($1 >= 0 && thislinelen < 0) {	# word, and first on line
X		line = $2
X		paddable = substr(sps, 1, length($2))
X		thislinelen = $1
X	} else if ($1 >= 0 && thislinelen+contlen+$1+$3 <= linelen-ind-tmpind) {
X		# word, and it fits on line
X		line = line cont $2
X		if (cont == " ")
X			paddable = paddable "x"
X		else
X			paddable = paddable substr(sps, 1, length(cont))
X		paddable = paddable substr(sps, 1, length($2))
X		thislinelen += contlen + $1
X	} else if ($1 == nobreak || $1 == message)
X		nop = 0		# no attention (i.e. break) needed here
X	else if ($1 == dobreak && $2 == "need" && \
X				nextlineno + $3 < pagelen + 1 - botmargin)
X		nop = 0		# enough space is available, no action needed
X	else if ($1 == dobreak && $2 == "toindent" && \
X					ind + tmpind + thislinelen < ind) {
X		# move to indent position within line; there is room
X		n = ind - (ind + tmpind + thislinelen)
X		line = line substr(sps, 1, n)
X		# nothing before this is paddable
X		paddable = substr(sps, 1, length(line))
X		thislinelen += n
X		# prevent padding immediately after this point
X		cont = ""
X		contlen = 0
X	} else if (thislinelen >= 0 || ($1 == dobreak && $2 == "need")) {
X		# must emit output, either due to break or "need"
X
X		# if at top of page, header
X		if (nextlineno == 1) {
X			for (i = int((topmargin-1)/2); i > 0; i--) {
X				print ""
X				nextlineno++
X			}
X			for (hno in hdrnames) {
X				h = hdrnames[hno]
X				if (hdrs[h] ~ /%/) {
X					n = split(hdrs[h], t, "%")
X					thispagehdrs[h] = t[1] thispageno t[2]
X				} else
X					thispagehdrs[h] = hdrs[h]
X			}
X			if (fph || thispageno > 1) {
X				lh = thispagehdrs["LH"]
X				ch = thispagehdrs["CH"]
X				rh = thispagehdrs["RH"]
X				lsp = int((linelen - length(lh ch rh)) / 2)
X				rsp = linelen - length(lh ch rh) - lsp
X				print pageoffset lh substr(sps, 1, lsp) ch substr(sps, 1, rsp) rh
X			} else
X				print ""
X			nextlineno++
X			while (nextlineno <= topmargin) {
X				print ""
X				nextlineno++
X			}
X		}
X
X		# the current line
X		# first, add a trailing hyphen if any
X		line = line eol
X		paddable = paddable substr(sps, 1, length(eol))
X		thislinelen += eollen
X
X		# trim trailing spaces if any
X		while (line ~ / $/) {
X			line = substr(line, 1, length(line)-1)
X			paddable = substr(paddable, 1, length(line))
X			thislinelen--
X		}
X
X		# print it in a suitable way
X		if (line == "")		# empty always prints as nothing
X			print ""
X		else if ($1 < 0 && $2 == "center") {
X			# center it
X			hsp = int((linelen - thislinelen) / 2)
X			if (hsp < 0)
X				hsp = 0
X			print pageoffset substr(sps, 1, ind+tmpind+hsp) line
X		} else if (adj == "left" || (adj == "both" && \
X					($1 < 0 || index(paddable, "x") == 0)))
X			# no right-margin adjustment (disabled, inappropriate
X			# (line ended by break), or impossible)
X			print pageoffset substr(sps, 1, ind+tmpind) line
X		else if (adj == "both") {
X			# hard case -- adjust right margin
X			# sanity check
X			if (length(paddable) != length(line))	# aieeeee
X				printf "awf: %f != %f!\n", length(paddable), \
X							length(line) >errs
X
X			# compute parameters
X			textlen = linelen - (ind+tmpind)
X			mustadd = textlen - thislinelen
X			npad = 0	# number of paddable spaces
X			for (tmp = paddable; (i = index(tmp, "x")) > 0; \
X							tmp = substr(tmp, i+1))
X				npad++
X			addatall = int(mustadd/npad)	# all grow this much
X			spall = substr(sps, 1, addatall)
X			nmore = mustadd - addatall*npad	# this many grow more
X
X			# build padded output text
X			out = substr(sps, 1, ind+tmpind)
X			padno = 0
X			while ((i = index(paddable, "x")) > 0) {
X				out = out substr(line, 1, i)
X				padno++
X				out = out spall
X				if (padfrom == "L") {
X					if (padno <= nmore)
X						out = out " "
X				} else {
X					if (padno > npad-nmore)
X						out = out " "
X				}
X				line = substr(line, i+1)
X				paddable = substr(paddable, i+1)
X			}
X
X			# print it, plus remnant not processed by loop
X			print pageoffset out line
X
X			# tidy up
X			if (padfrom == "L")
X				padfrom = "R"
X			else
X				padfrom = "L"
X		}
X
X		# tidy up after output line
X		nextlineno++
X		line = ""
X		paddable = ""
X		thislinelen = -1
X		tmpind = 0
X		nospacemode = 0
X
X		# if we broke from a "need", go to bottom of page
X		if ($1 == dobreak && $2 == "need")
X			while (nextlineno < pagelen + 1 - botmargin) {
X				print ""
X				nextlineno++
X			}
X
X		# footer, if at bottom of page
X		if (nextlineno >= pagelen + 1 - botmargin) {
X			for (i = int((botmargin-1)/2); i > 0; i--) {
X				print ""
X				nextlineno++
X			}
X			# header code prepared thispagehdrs
X			lf = thispagehdrs["LF"]
X			cf = thispagehdrs["CF"]
X			rf = thispagehdrs["RF"]
X			lsp = int((linelen - length(lf cf rf)) / 2)
X			rsp = linelen - length(lf cf rf) - lsp
X			print pageoffset lf substr(sps, 1, lsp) cf substr(sps, 1, rsp) rf
X			nextlineno++
X			while (nextlineno <= pagelen) {
X				print ""
X				nextlineno++
X			}
X			nextlineno = 1
X			thispageno++
X
X			# after page break, should not space unnecessarily,
X			# and should pad first line from right
X			nospacemode = 1
X			padfrom = "R"
X		}
X
X		# we are finally done with emitting output
X		# pick up input word, if any
X		if ($1 >= 0) {
X			line = $2
X			paddable = substr(sps, 1, length($2))
X			thislinelen = $1
X		}
X	}
X
X	# if we broke, next line should pad from right
X	if ($1 == dobreak)
X		padfrom = "R"
X
X	# cleanup and post-break command processing
X	if ($1 >= 0 || $2 == "nohyphen") {
X		# reset hyphenation trickery after each word (fragment)
X		cont = " "
X		contlen = 1
X		eol = ""
X		eollen = 0
X	} else if ($2 == "need" || $2 == "toindent")
X		nop = 0		# dealt with above
X	else if ($2 == "flush" || $2 == "center")
X		nop = 0		# exist only to cause break
X	else if ($1 == message)
X		print "awf: " $2 >errs
X	else if ($2 == "gap") {
X		# gap between last word and next one should be >= $3
X		if (thislinelen >= 0) {
X			line = line substr(sps, 1, $3-1)
X			paddable = paddable substr(sps, 1, $3-1)
X			thislinelen += $3-1
X		}
X	} else if ($2 == "tabto") {
X		# move to tab stop at $3
X		if (thislinelen < 0)
X			thislinelen = 0		# make line exist
X		n = $3 - thislinelen
X		if (n > 0) {			# must emit some space
X			line = line substr(sps, 1, n)
X			# nothing before a tab is paddable
X			paddable = substr(sps, 1, length(line))
X			thislinelen += n
X			# suppress space following
X			cont = ""
X			contlen = 0
X		}
X	} else if ($2 == "errsto")
X		errs = $3
X	else if ($2 ~ /^[LCR][HF]$/)
X		hdrs[$2] = $3
X	else if ($2 == "fph")
X		fph = $3
X	else if ($2 == "space") {
X		if (!nospacemode) {
X			# generate an empty line, which will be flushed by
X			# the next word; NB we know "space" caused a flush
X			line = ""
X			paddable = ""
X			thislinelen = linelen + 1
X			nospacemode = 0
X		}
X	} else if ($2 == "left")
X		adj = "left"
X	else if ($2 == "both")
X		adj = "both"
X	else if ($2 == "indent")
X		ind = $3
X	else if ($2 == "tempindent")
X		tmpind = $3
X	else if ($2 == "linelen")
X		linelen = $3
X	else if ($2 == "pagelen")
X		pagelen = $3
X	else if ($2 == "nospace")
X		nospacemode = 1
X	else if ($2 == "yesspace")
X		nospacemode = 0
X	else if ($2 == "hyphen") {
X		# discretionary hyphen at this point
X		cont = ""
X		contlen = 0
X		eol = $3
X		eollen = $4
X	} else if ($2 == "userhyphen") {
X		# user-supplied hyphen at this point
X		cont = $3
X		contlen = $4
X		eol = $3
X		eollen = $4
X	} else if ($2 == "pageoffset")
X		pageoffset = substr(sps, 1, $3)
X	else
X		print "awf: URK -- INTERNAL OPCODE `" $2 "' UNKNOWN" >errs
X}
XEND {
X	# second pass is supposed to fake a .ne to flush the last page
X	if (nextlineno != 1)
X		print "awf: last page not flushed!" >errs
X}
!
echo done

exit 0 # Just in case...
