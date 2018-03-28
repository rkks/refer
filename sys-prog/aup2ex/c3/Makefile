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

aupls: aupls.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o aupls aupls.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

bigdir: bigdir.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o bigdir bigdir.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

cwd: cwd.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o cwd cwd.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

feed: feed.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o feed feed.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

inode: inode.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o inode inode.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

sio: sio.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o sio sio.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

slink: slink.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o slink slink.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

x3a: x3a.o $(AUPOBJS) $(PTHREADSTUB)
	$(CC) -o x3a x3a.o $(AUPOBJS) $(LIBS) $(PTHREADSTUB)

sio.o: /aup/c3/sio.c
	$(CC) $(CFLAGS)  -c -o sio.o /aup/c3/sio.c

inode.o: /aup/c3/inode.c
	$(CC) $(CFLAGS)  -c -o inode.o /aup/c3/inode.c

bigdir.o: /aup/c3/bigdir.c
	$(CC) $(CFLAGS)  -c -o bigdir.o /aup/c3/bigdir.c

x3a.o: /aup/c3/x3a.c
	$(CC) $(CFLAGS)  -c -o x3a.o /aup/c3/x3a.c

cwd.o: /aup/c3/cwd.c
	$(CC) $(CFLAGS)  -c -o cwd.o /aup/c3/cwd.c

slink.o: /aup/c3/slink.c
	$(CC) $(CFLAGS)  -c -o slink.o /aup/c3/slink.c

feed.o: /aup/c3/feed.c
	$(CC) $(CFLAGS)  -c -o feed.o /aup/c3/feed.c

aupls.o: /aup/c3/aupls.c
	$(CC) $(CFLAGS)  -c -o aupls.o /aup/c3/aupls.c

all:  slink inode bigdir aupls x3a feed sio cwd
clean:
	rm  slink inode bigdir aupls x3a feed sio cwd *.o
