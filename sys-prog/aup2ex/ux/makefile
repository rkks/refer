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
CFLAGS = $(INCLUDES) -I$(AUPSRC)/include -I$(AUPSRC)/c6 -Wimplicit -Wstrict-prototypes -Wall -D$(OS) -D_REENTRANT -D_THREAD_SAFE
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

mkmacrostr.o: $(AUPSRC)/mkmacrostr.c
	$(CC) $(CFLAGS)  -c -o mkmacrostr.o $(AUPSRC)/mkmacrostr.c

options.o: $(AUPSRC)/common/options.c
	$(CC) $(CFLAGS)  -c -o options.o $(AUPSRC)/common/options.c

pthreadstub.o: $(AUPSRC)/common/pthreadstub.c
	$(CC) $(CFLAGS)  -c -o pthreadstub.o $(AUPSRC)/common/pthreadstub.c

ec.o: $(AUPSRC)/common/ec.c
	$(CC) $(CFLAGS)  -c -o ec.o $(AUPSRC)/common/ec.c

logf.o: $(AUPSRC)/common/logf.c
	$(CC) $(CFLAGS)  -c -o logf.o $(AUPSRC)/common/logf.c

setblock.o: $(AUPSRC)/c4/setblock.c
	$(CC) $(CFLAGS)  -c -o setblock.o $(AUPSRC)/c4/setblock.c

extio.o: $(AUPSRC)/c2/extio.c
	$(CC) $(CFLAGS)  -c -o extio.o $(AUPSRC)/c2/extio.c

syserr.o: $(AUPSRC)/c1/syserr.c
	$(CC) $(CFLAGS)  -c -o syserr.o $(AUPSRC)/c1/syserr.c

time.o: $(AUPSRC)/c1/time.c
	$(CC) $(CFLAGS)  -c -o time.o $(AUPSRC)/c1/time.c

getblksize.o: $(AUPSRC)/c2/getblksize.c
	$(CC) $(CFLAGS)  -c -o getblksize.o $(AUPSRC)/c2/getblksize.c
	
UXHDRS = \
	$(AUPSRC)/nu/ux.hpp \
	$(AUPSRC)/nu/uxaio.hpp \
	$(AUPSRC)/nu/uxdir.hpp \
	$(AUPSRC)/nu/uxdirstream.hpp \
	$(AUPSRC)/nu/uxerr.hpp \
	$(AUPSRC)/nu/uxexitstatus.hpp \
	$(AUPSRC)/nu/uxfile.hpp \
	$(AUPSRC)/nu/uxnetdb.hpp \
	$(AUPSRC)/nu/uxposixipc.hpp \
	$(AUPSRC)/nu/uxprocess.hpp \
	$(AUPSRC)/nu/uxsigset.hpp \
	$(AUPSRC)/nu/uxsocket.hpp \
	$(AUPSRC)/nu/uxsystem.hpp \
	$(AUPSRC)/nu/uxsysvipc.hpp \
	$(AUPSRC)/nu/uxterminal.hpp \
	$(AUPSRC)/nu/uxtime.hpp \
	$(AUPSRC)/nu/uxtimetm.hpp

OBJS = \
	uxsystem.o\
	uxsocket.o \
	uxnetdb.o \
	uxtime.o \
	uxterminal.o\
	uxprocess.o\
	uxposixipc.o\
	uxtestfile.o\
	uxsysvipc.o\
	ux.o\
	uxaio.o\
	uxdir.o\
	uxdirstream.o\
	uxerr.o\
	uxexitstatus.o\
	uxfile.o\
	uxtimetm.o \
	$(AUPOBJS) $(PTHREADSTUB)

tf: $(OBJS)
	g++ -o tf $(OBJS) $(LIBS)

uxtestfile.o: $(AUPSRC)/nu/uxtestfile.cpp $(UXHDRS)
	$(CC) $(CFLAGS)  -c -o uxtestfile.o $(AUPSRC)/nu/uxtestfile.cpp

uxfile.o: $(AUPSRC)/nu/uxfile.cpp $(UXHDRS)
	$(CC) $(CFLAGS)  -c -o uxfile.o $(AUPSRC)/nu/uxfile.cpp

uxerr.o: $(AUPSRC)/nu/uxerr.cpp $(UXHDRS)
	$(CC) $(CFLAGS)  -c -o uxerr.o $(AUPSRC)/nu/uxerr.cpp

uxdir.o: $(AUPSRC)/nu/uxdir.cpp $(UXHDRS)
	$(CC) $(CFLAGS)  -c -o uxdir.o $(AUPSRC)/nu/uxdir.cpp

uxdirstream.o: $(AUPSRC)/nu/uxdirstream.cpp $(UXHDRS)
	$(CC) $(CFLAGS)  -c -o uxdirstream.o $(AUPSRC)/nu/uxdirstream.cpp

uxprocess.o: $(AUPSRC)/nu/uxprocess.cpp $(UXHDRS)
	$(CC) $(CFLAGS)  -c -o uxprocess.o $(AUPSRC)/nu/uxprocess.cpp

uxaio.o: $(AUPSRC)/nu/uxaio.cpp $(UXHDRS)
	$(CC) $(CFLAGS)  -c -o uxaio.o $(AUPSRC)/nu/uxaio.cpp

uxsystem.o: $(AUPSRC)/nu/uxsystem.cpp $(UXHDRS)
	$(CC) $(CFLAGS)  -c -o uxsystem.o $(AUPSRC)/nu/uxsystem.cpp

ux.o: $(AUPSRC)/nu/ux.cpp $(UXHDRS)
	$(CC) $(CFLAGS)  -c -o ux.o $(AUPSRC)/nu/ux.cpp

uxexitstatus.o: $(AUPSRC)/nu/uxexitstatus.cpp $(UXHDRS)
	$(CC) $(CFLAGS)  -c -o uxexitstatus.o $(AUPSRC)/nu/uxexitstatus.cpp

uxtimetm.o: $(AUPSRC)/nu/uxtimetm.cpp $(UXHDRS)
	$(CC) $(CFLAGS)  -c -o uxtimetm.o $(AUPSRC)/nu/uxtimetm.cpp

uxtime.o: $(AUPSRC)/nu/uxtime.cpp $(UXHDRS)
	$(CC) $(CFLAGS)  -c -o uxtime.o $(AUPSRC)/nu/uxtime.cpp

uxsocket.o: $(AUPSRC)/nu/uxsocket.cpp $(UXHDRS)
	$(CC) $(CFLAGS)  -c -o uxsocket.o $(AUPSRC)/nu/uxsocket.cpp

uxsysvipc.o: $(AUPSRC)/nu/uxsysvipc.cpp $(UXHDRS)
	$(CC) $(CFLAGS)  -c -o uxsysvipc.o $(AUPSRC)/nu/uxsysvipc.cpp

uxposixipc.o: $(AUPSRC)/nu/uxposixipc.cpp $(UXHDRS)
	$(CC) $(CFLAGS)  -c -o uxposixipc.o $(AUPSRC)/nu/uxposixipc.cpp

uxterminal.o: $(AUPSRC)/nu/uxterminal.cpp $(UXHDRS)
	$(CC) $(CFLAGS)  -c -o uxterminal.o $(AUPSRC)/nu/uxterminal.cpp

uxnetdb.o: $(AUPSRC)/nu/uxnetdb.cpp $(UXHDRS)
	$(CC) $(CFLAGS)  -c -o uxnetdb.o $(AUPSRC)/nu/uxnetdb.cpp

all:  tf

clean:
	rm  tf *.o
