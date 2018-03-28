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

piperead: piperead.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o piperead piperead.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

search0: search0.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o search0 search0.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

search1: search1.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o search1 search1.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

sh3: sh3.o builtin.o statuspr.o setenv.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o sh3 sh3.o builtin.o statuspr.o setenv.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

x6: x6.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o x6 x6.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

setenv.o: /aup/c5/setenv.c
	$(CC) $(CFLAGS)  -c -o setenv.o /aup/c5/setenv.c

piperead.o: /aup/c6/piperead.c
	$(CC) $(CFLAGS)  -c -o piperead.o /aup/c6/piperead.c

statuspr.o: /aup/c5/statuspr.c
	$(CC) $(CFLAGS)  -c -o statuspr.o /aup/c5/statuspr.c

search0.o: /aup/c6/search0.c
	$(CC) $(CFLAGS)  -c -o search0.o /aup/c6/search0.c

builtin.o: /aup/c5/builtin.c
	$(CC) $(CFLAGS)  -c -o builtin.o /aup/c5/builtin.c

search1.o: /aup/c6/search1.c
	$(CC) $(CFLAGS)  -c -o search1.o /aup/c6/search1.c

x6.o: /aup/c6/x6.c
	$(CC) $(CFLAGS)  -c -o x6.o /aup/c6/x6.c

sh3.o: /aup/c6/sh3.c
	$(CC) $(CFLAGS)  -c -o sh3.o /aup/c6/sh3.c

all:  sh3 search0 x6 search1 piperead
clean:
	rm  sh3 search0 x6 search1 piperead *.o
