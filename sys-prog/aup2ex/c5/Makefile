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

aupnice: nice.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o aupnice nice.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

fpe: fpe.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o fpe fpe.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

lim: limit.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o lim limit.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

sh0: sh0.o builtin.o statuspr.o setenv.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o sh0 sh0.o builtin.o statuspr.o setenv.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

t0: t0.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o t0 t0.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

t1: t1.o $(AUPOBJS) 
	$(CC) -o t1 t1.o $(AUPOBJS) $(LIBS) $(TLIBS) 

t2: t2.o $(AUPOBJS) 
	$(CC) -o t2 t2.o $(AUPOBJS) $(LIBS) $(TLIBS) 

t2-mtx: t2-mtx.o $(AUPOBJS) 
	$(CC) -o t2-mtx t2-mtx.o $(AUPOBJS) $(LIBS) $(TLIBS) 

tx1: tx1.o $(AUPOBJS) 
	$(CC) -o tx1 tx1.o $(AUPOBJS) $(LIBS) $(TLIBS) 

tx2: tx2.o $(AUPOBJS) 
	$(CC) -o tx2 tx2.o $(AUPOBJS) $(LIBS) $(TLIBS) 

tx3: tx3.o $(AUPOBJS) 
	$(CC) -o tx3 tx3.o $(AUPOBJS) $(LIBS) $(TLIBS) 

tx4: tx4.o $(AUPOBJS) 
	$(CC) -o tx4 tx4.o $(AUPOBJS) $(LIBS) $(TLIBS) 

tx5: tx5.o $(AUPOBJS) 
	$(CC) -o tx5 tx5.o $(AUPOBJS) $(LIBS) $(TLIBS) 

tx6: tx6.o $(AUPOBJS) 
	$(CC) -o tx6 tx6.o $(AUPOBJS) $(LIBS) $(TLIBS) 

tx7: tx7.o $(AUPOBJS) 
	$(CC) -o tx7 tx7.o $(AUPOBJS) $(LIBS) $(TLIBS) 

uidgrp: uidgrp.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o uidgrp uidgrp.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

waitid: waitid.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o waitid waitid.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

waitpid: waitpid.o statuspr.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o waitpid waitpid.o statuspr.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

x5: x5.o setenv.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o x5 x5.o setenv.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

x5a: x5a.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o x5a x5a.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

waitid.o: /aup/c5/waitid.c
	$(CC) $(CFLAGS)  -c -o waitid.o /aup/c5/waitid.c

uidgrp.o: /aup/c5/uidgrp.c
	$(CC) $(CFLAGS)  -c -o uidgrp.o /aup/c5/uidgrp.c

tx4.o: /aup/c5/tx4.c
	$(CC) $(CFLAGS)  -c -o tx4.o /aup/c5/tx4.c

t2-mtx.o: /aup/c5/t2-mtx.c
	$(CC) $(CFLAGS)  -c -o t2-mtx.o /aup/c5/t2-mtx.c

setenv.o: /aup/c5/setenv.c
	$(CC) $(CFLAGS)  -c -o setenv.o /aup/c5/setenv.c

tx7.o: /aup/c5/tx7.c
	$(CC) $(CFLAGS)  -c -o tx7.o /aup/c5/tx7.c

t1.o: /aup/c5/t1.c
	$(CC) $(CFLAGS)  -c -o t1.o /aup/c5/t1.c

statuspr.o: /aup/c5/statuspr.c
	$(CC) $(CFLAGS)  -c -o statuspr.o /aup/c5/statuspr.c

tx2.o: /aup/c5/tx2.c
	$(CC) $(CFLAGS)  -c -o tx2.o /aup/c5/tx2.c

x5.o: /aup/c5/x5.c
	$(CC) $(CFLAGS)  -c -o x5.o /aup/c5/x5.c

tx5.o: /aup/c5/tx5.c
	$(CC) $(CFLAGS)  -c -o tx5.o /aup/c5/tx5.c

x5a.o: /aup/c5/x5a.c
	$(CC) $(CFLAGS)  -c -o x5a.o /aup/c5/x5a.c

builtin.o: /aup/c5/builtin.c
	$(CC) $(CFLAGS)  -c -o builtin.o /aup/c5/builtin.c

t2.o: /aup/c5/t2.c
	$(CC) $(CFLAGS)  -c -o t2.o /aup/c5/t2.c

limit.o: /aup/c5/limit.c
	$(CC) $(CFLAGS)  -c -o limit.o /aup/c5/limit.c

nice.o: /aup/c5/nice.c
	$(CC) $(CFLAGS)  -c -o nice.o /aup/c5/nice.c

tx3.o: /aup/c5/tx3.c
	$(CC) $(CFLAGS)  -c -o tx3.o /aup/c5/tx3.c

sh0.o: /aup/c5/sh0.c
	$(CC) $(CFLAGS)  -c -o sh0.o /aup/c5/sh0.c

fpe.o: /aup/c5/fpe.c
	$(CC) $(CFLAGS)  -c -o fpe.o /aup/c5/fpe.c

tx6.o: /aup/c5/tx6.c
	$(CC) $(CFLAGS)  -c -o tx6.o /aup/c5/tx6.c

t0.o: /aup/c5/t0.c
	$(CC) $(CFLAGS)  -c -o t0.o /aup/c5/t0.c

waitpid.o: /aup/c5/waitpid.c
	$(CC) $(CFLAGS)  -c -o waitpid.o /aup/c5/waitpid.c

tx1.o: /aup/c5/tx1.c
	$(CC) $(CFLAGS)  -c -o tx1.o /aup/c5/tx1.c

all:  tx6 lim uidgrp tx7 waitpid x5 tx1 t0 fpe x5a tx2 t1 tx3 t2-mtx t2 tx4 aupnice waitid tx5 sh0
clean:
	rm  tx6 lim uidgrp tx7 waitpid x5 tx1 t0 fpe x5a tx2 t1 tx3 t2-mtx t2 tx4 aupnice waitid tx5 sh0 *.o
