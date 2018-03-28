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

ckstdvers: ckstdvers.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o ckstdvers ckstdvers.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

ckvers: ckvers.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o ckvers ckvers.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

err: errorhandling.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o err errorhandling.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

opttest: opttest.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o opttest opttest.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

suvreq: suvreq.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o suvreq suvreq.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

sysconf: sysconf.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o sysconf sysconf.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

timetst: timetst.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o timetst timetst.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

version: version.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o version version.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

ckvers.o: /aup/c1/ckvers.c
	$(CC) $(CFLAGS) -DCKVERS_UNIX98 -c -o ckvers.o /aup/c1/ckvers.c

version.o: /aup/c1/version.c
	$(CC) $(CFLAGS)  -c -o version.o /aup/c1/version.c

timetst.o: /aup/c1/timetst.c
	$(CC) $(CFLAGS)  -c -o timetst.o /aup/c1/timetst.c

sysconf.o: /aup/c1/sysconf.c
	$(CC) $(CFLAGS)  -c -o sysconf.o /aup/c1/sysconf.c

errorhandling.o: /aup/c1/errorhandling.c
	$(CC) $(CFLAGS)  -c -o errorhandling.o /aup/c1/errorhandling.c

ckstdvers.o: /aup/c1/ckstdvers.c
	$(CC) $(CFLAGS)  -c -o ckstdvers.o /aup/c1/ckstdvers.c

opttest.o: /aup/c1/opttest.c
	$(CC) $(CFLAGS)  -c -o opttest.o /aup/c1/opttest.c

suvreq.o: /aup/c1/suvreq.c
	$(CC) $(CFLAGS) -DCKVERS_UNIX98x -c -o suvreq.o /aup/c1/suvreq.c

all:  opttest timetst err version ckvers sysconf ckstdvers suvreq
clean:
	rm  opttest timetst err version ckvers sysconf ckstdvers suvreq *.o
