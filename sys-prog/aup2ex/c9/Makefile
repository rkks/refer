# Do not edit this makefile. It was generated automatically 
# with commands like
#
#     $ AUPSRC=/aup # (for example)
#     $ export AUPSRC
#     $ awk -f $AUPSRC/makebuild.awk $AUPSRC/c1/makebuild.spec >$AUPSRC/c1/Makefile
#
# It is incomplete in the sense that there are no header-file
# dependencies.
#
# Use the shell-procedure makebuild to run awk. Then compile
# with a shell-procedure like this one for Linux:
#
# 	AUPSRC=/aup # (for example)
# 	OS=LINUX
# 	LIBS="-lncurses -lutil"
# 	TLIBS=-pthread
# 	export OS LIBS TLIBS
# 	make $2 -f $AUPSRC/$1/Makefile $3
#

defaulttarget: all
# Following must be defined in the environment before you run make:
#
#	OS = SOLARIS or LINUX or FREEBSD
#	LIBS = regular (non-thread) libraries (eg, "-lncurses -lutil")
#	TLIBS = thread libraries (eg, "-llthread -llgcc_r")
#	PTHREADSTUB = stub for thread functions (eg, pthreadstub.o), if needed
#	INCLUDES = additional include directory (eg, "-I/usr/local/include/pthread/linuxthreads")
#	Don't forget:
#		export OS LIBS TLIBS PTHREADSTUB INCLUDES
#	Suggestions:
#		Solaris:
#			OS=SOLARIS
#			LIBS="-lrt -lcurses -lsocket -lnsl"
#			TLIBS="-lpthread"
#
#		FreeBSD:
#			OS=FREEBSD
#			LIBS="-lncurses -lutil"
#			TLIBS="-llthread -llgcc_r"
#			PTHREADSTUB=pthreadstub.o
#			INCLUDES="-I/usr/local/include/pthread/linuxthreads"
#
#		Linux:
#			OS=LINUX
#			LIBS="-lncurses -lutil"
#			TLIBS="-pthread"
CFLAGS = $(INCLUDES) -I$(AUPSRC)/include -I$(AUPSRC)/common -I$(AUPSRC)/c6 -Wimplicit -Wstrict-prototypes -Wall -D$(OS) -D_REENTRANT -D_THREAD_SAFE
CC = gcc
# Same objects are used all the time, sometimes linking in unnecessary files.
AUPOBJS = syserr.o time.o extio.o getblksize.o setblock.o ec.o logf.o options.o macrostr.o

mms: mkmacrostr.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o mms mkmacrostr.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)
#############################################
# Utility to build macro-string program
#############################################
macrostr.o: $(AUPSRC)/common/macrostr.c $(AUPSRC)/common/macrostr.incl
	$(CC) $(CFLAGS) -c  -o $*.o $(AUPSRC)/common/macrostr.c

mkmacrostr.o: /aup/common/mkmacrostr.c
	$(CC) $(CFLAGS)  -c -o mkmacrostr.o /aup/common/mkmacrostr.c

options.o: /aup/common/options.c
	$(CC) $(CFLAGS)  -c -o options.o /aup/common/options.c

pthreadstub.o: /aup/common/pthreadstub.c
	$(CC) $(CFLAGS)  -c -o pthreadstub.o /aup/common/pthreadstub.c

ec.o: /aup/common/ec.c
	$(CC) $(CFLAGS)  -c -o ec.o /aup/common/ec.c

logf.o: /aup/common/logf.c
	$(CC) $(CFLAGS)  -c -o logf.o /aup/common/logf.c

setblock.o: /aup/c4/setblock.c
	$(CC) $(CFLAGS)  -c -o setblock.o /aup/c4/setblock.c

extio.o: /aup/c2/extio.c
	$(CC) $(CFLAGS)  -c -o extio.o /aup/c2/extio.c

syserr.o: /aup/c1/syserr.c
	$(CC) $(CFLAGS)  -c -o syserr.o /aup/c1/syserr.c

time.o: /aup/c1/time.c
	$(CC) $(CFLAGS)  -c -o time.o /aup/c1/time.c

getblksize.o: /aup/c2/getblksize.c
	$(CC) $(CFLAGS)  -c -o getblksize.o /aup/c2/getblksize.c

alarm_test: alarm_test.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o alarm_test alarm_test.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

ctx: ctx.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o ctx ctx.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

intro: intro.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o intro intro.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

intro2: intro2.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o intro2 intro2.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

jmp: jmp.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o jmp jmp.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

prcssync: prcssync.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o prcssync prcssync.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

sighandler: sighandler.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o sighandler sighandler.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

siginfo: siginfo.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o siginfo siginfo.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

sigmin: sigmin.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o sigmin sigmin.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

sigminthr: sigminthr.o $(AUPOBJS) 
	$(CC) -o sigminthr sigminthr.o $(AUPOBJS) $(LIBS) $(TLIBS) 

slp: slp.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o slp slp.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

thrq: thrq.o $(AUPOBJS) 
	$(CC) -o thrq thrq.o $(AUPOBJS) $(LIBS) $(TLIBS) 

tmr: tmr.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o tmr tmr.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

thrq.o: /aup/c9/thrq.c
	$(CC) $(CFLAGS)  -c -o thrq.o /aup/c9/thrq.c

prcssync.o: /aup/c9/prcssync.c
	$(CC) $(CFLAGS)  -c -o prcssync.o /aup/c9/prcssync.c

slp.o: /aup/c9/slp.c
	$(CC) $(CFLAGS)  -c -o slp.o /aup/c9/slp.c

siginfo.o: /aup/c9/siginfo.c
	$(CC) $(CFLAGS)  -c -o siginfo.o /aup/c9/siginfo.c

ctx.o: /aup/c9/ctx.c
	$(CC) $(CFLAGS)  -c -o ctx.o /aup/c9/ctx.c

intro2.o: /aup/c9/intro2.c
	$(CC) $(CFLAGS)  -c -o intro2.o /aup/c9/intro2.c

slp-simple.o: /aup/slp-simplec9/slp-simple.c
	$(CC) $(CFLAGS)  -c -o slp-simple.o /aup/slp-simplec9/slp-simple.c

sigminthr.o: /aup/c9/sigminthr.c
	$(CC) $(CFLAGS)  -c -o sigminthr.o /aup/c9/sigminthr.c

sigmin.o: /aup/c9/sigmin.c
	$(CC) $(CFLAGS)  -c -o sigmin.o /aup/c9/sigmin.c

tmr.o: /aup/c9/tmr.c
	$(CC) $(CFLAGS)  -c -o tmr.o /aup/c9/tmr.c

jmp.o: /aup/c9/jmp.c
	$(CC) $(CFLAGS)  -c -o jmp.o /aup/c9/jmp.c

intro.o: /aup/c9/intro.c
	$(CC) $(CFLAGS)  -c -o intro.o /aup/c9/intro.c

sighandler.o: /aup/c9/sighandler.c
	$(CC) $(CFLAGS)  -c -o sighandler.o /aup/c9/sighandler.c

alarm_test.o: /aup/c9/alarm_test.c
	$(CC) $(CFLAGS)  -c -o alarm_test.o /aup/c9/alarm_test.c

all:  siginfo sighandler intro slp ctx sigmin sigminthr prcssync tmr thrq jmp intro2 alarm_test
clean:
	rm  siginfo sighandler intro slp ctx sigmin sigminthr prcssync tmr thrq jmp intro2 alarm_test *.o
