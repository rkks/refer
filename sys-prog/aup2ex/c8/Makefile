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

adi: adi.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o adi adi.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

br: br.o ssi.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o br br.o ssi.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

byteorder: byteorder.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o byteorder byteorder.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

minibr: minibr.o ssi.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o minibr minibr.o ssi.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

miniws: miniws.o ssi.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o miniws miniws.o ssi.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

ndb: ndb.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o ndb ndb.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

protoent: protoent.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o protoent protoent.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

sktcl: sktcl.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o sktcl sktcl.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

sktex: sktex.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o sktex sktex.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

sktexdg: sktexdg.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o sktexdg sktexdg.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

sktexdgmult: sktexdgmult.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o sktexdgmult sktexdgmult.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

sktexdgmultmsg: sktexdgmultmsg.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o sktexdgmultmsg sktexdgmultmsg.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

sktexm: sktexm.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o sktexm sktexm.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

sktopt: sktopt.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o sktopt sktopt.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

aupuname: uname.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o aupuname uname.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

ws: ws.o ssi.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o ws ws.o ssi.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

ws.o: /aup/c8/ws.c
	$(CC) $(CFLAGS)  -c -o ws.o /aup/c8/ws.c

minibr.o: /aup/c8/minibr.c
	$(CC) $(CFLAGS)  -c -o minibr.o /aup/c8/minibr.c

adi.o: /aup/c8/adi.c
	$(CC) $(CFLAGS)  -c -o adi.o /aup/c8/adi.c

sktexdgmult.o: /aup/c8/sktexdgmult.c
	$(CC) $(CFLAGS)  -c -o sktexdgmult.o /aup/c8/sktexdgmult.c

sktexm.o: /aup/c8/sktexm.c
	$(CC) $(CFLAGS)  -c -o sktexm.o /aup/c8/sktexm.c

ndb.o: /aup/c8/ndb.c
	$(CC) $(CFLAGS)  -c -o ndb.o /aup/c8/ndb.c

sktexdgmultmsg.o: /aup/c8/sktexdgmultmsg.c
	$(CC) $(CFLAGS)  -c -o sktexdgmultmsg.o /aup/c8/sktexdgmultmsg.c

byteorder.o: /aup/c8/byteorder.c
	$(CC) $(CFLAGS)  -c -o byteorder.o /aup/c8/byteorder.c

sktex.o: /aup/c8/sktex.c
	$(CC) $(CFLAGS)  -c -o sktex.o /aup/c8/sktex.c

br.o: /aup/c8/br.c
	$(CC) $(CFLAGS)  -c -o br.o /aup/c8/br.c

uname.o: /aup/c8/uname.c
	$(CC) $(CFLAGS)  -c -o uname.o /aup/c8/uname.c

sktcl.o: /aup/c8/sktcl.c
	$(CC) $(CFLAGS)  -c -o sktcl.o /aup/c8/sktcl.c

sktopt.o: /aup/c8/sktopt.c
	$(CC) $(CFLAGS)  -c -o sktopt.o /aup/c8/sktopt.c

sktexdg.o: /aup/c8/sktexdg.c
	$(CC) $(CFLAGS)  -c -o sktexdg.o /aup/c8/sktexdg.c

protoent.o: /aup/c8/protoent.c
	$(CC) $(CFLAGS)  -c -o protoent.o /aup/c8/protoent.c

miniws.o: /aup/c8/miniws.c
	$(CC) $(CFLAGS)  -c -o miniws.o /aup/c8/miniws.c

ssi.o: /aup/c8/ssi.c
	$(CC) $(CFLAGS)  -c -o ssi.o /aup/c8/ssi.c

all:  ws aupuname sktexdg byteorder adi sktexm br sktexdgmult sktcl sktex ndb miniws protoent sktexdgmultmsg minibr sktopt
clean:
	rm  ws aupuname sktexdg byteorder adi sktexm br sktexdgmult sktcl sktex ndb miniws protoent sktexdgmultmsg minibr sktopt *.o
