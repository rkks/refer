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

auptty: auptty.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o auptty auptty.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

pginfo: pginfo.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o pginfo pginfo.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

record: record.o ptutil.o tc_setraw.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o record record.o ptutil.o tc_setraw.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

scrapp: scrapp.o tc_setraw.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o scrapp scrapp.o tc_setraw.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

scrappc: scrappc.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o scrappc scrappc.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

streamlist: streamlist.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o streamlist streamlist.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

x4: x4.o bufio.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o x4 x4.o bufio.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

x4a: x4a.o tc_setraw.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o x4a x4a.o tc_setraw.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

x4.o: /aup/c4/x4.c
	$(CC) $(CFLAGS)  -c -o x4.o /aup/c4/x4.c

tc_setraw.o: /aup/c4/tc_setraw.c
	$(CC) $(CFLAGS)  -c -o tc_setraw.o /aup/c4/tc_setraw.c

ptutil.o: /aup/c4/ptutil.c
	$(CC) $(CFLAGS)  -c -o ptutil.o /aup/c4/ptutil.c

scrapp.o: /aup/c4/scrapp.c
	$(CC) $(CFLAGS)  -c -o scrapp.o /aup/c4/scrapp.c

x4a.o: /aup/c4/x4a.c
	$(CC) $(CFLAGS)  -c -o x4a.o /aup/c4/x4a.c

bufio.o: /aup/c2/bufio.c
	$(CC) $(CFLAGS)  -c -o bufio.o /aup/c2/bufio.c

scrappc.o: /aup/c4/scrappc.c
	$(CC) $(CFLAGS)  -c -o scrappc.o /aup/c4/scrappc.c

pginfo.o: /aup/c4/pginfo.c
	$(CC) $(CFLAGS)  -c -o pginfo.o /aup/c4/pginfo.c

streamlist.o: /aup/c4/streamlist.c
	$(CC) $(CFLAGS)  -c -o streamlist.o /aup/c4/streamlist.c

record.o: /aup/c4/record.c
	$(CC) $(CFLAGS)  -c -o record.o /aup/c4/record.c

auptty.o: /aup/c4/auptty.c
	$(CC) $(CFLAGS)  -c -o auptty.o /aup/c4/auptty.c

all:  record x4 streamlist pginfo x4a scrapp auptty scrappc
clean:
	rm  record x4 streamlist pginfo x4a scrapp auptty scrappc *.o
