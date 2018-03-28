#
# Makefile for example programs in Chapter 11 of "Systems Programming
# for SVR4-Based UNIX Systems" by David A. Curry.
#

#
# Pull in os-specific definitions.
#
include Makedefs.$(OSNAME)

#
# Programs to build.
#
STD=	fork forkexec forkexecwait shellcmd system

GEN=	shell

#
# Build all the programs.  To build a single program, issue the command
# "../build-examples name-of-program".
#
all:	$(STD) $(GEN)

$(STD):	$$@.o
	$(CC) -o $@ $@.o

$(GEN): $$@.o
	$(CC) -o $@ $@.o $(GENLIBS)

#
# Issue the command "../build-examples clean" to get rid of object files,
# core files, and executables.
#
clean:
	rm -f $(STD) $(GEN) a.out core *.o \#*

#
# Specific dependencies for building each object.
#
fork.o:			common/fork.c
	$(CC) $(CFLAGS) -c -o $@ $?
forkexec.o:		common/forkexec.c
	$(CC) $(CFLAGS) -c -o $@ $?
forkexecwait.o:		common/forkexecwait.c
	$(CC) $(CFLAGS) -c -o $@ $?
shell.o:		$(OSNAME)/shell.c
	$(CC) $(CFLAGS) -c -o $@ $?
shellcmd.o:		$(OSNAME)/shellcmd.c
	$(CC) $(CFLAGS) -c -o $@ $?
system.o:		common/system.c
	$(CC) $(CFLAGS) -c -o $@ $?
