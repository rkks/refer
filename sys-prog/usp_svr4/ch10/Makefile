#
# Makefile for example programs in Chapter 10 of "Systems Programming
# for SVR4-Based UNIX Systems" by David A. Curry.
#

#
# Pull in os-specific definitions.
#
include Makedefs.$(OSNAME)

#
# Programs to build.
#
PRG=	signal1 signal2 signal3 signal4 timeout1 timeout2 timeout3

#
# Build all the programs.  To build a single program, issue the command
# "../build-examples name-of-program".
#
all:	$(PRG)

$(PRG):	$$@.o
	$(CC) -o $@ $@.o $(SIGLIBS)

#
# Issue the command "../build-examples clean" to get rid of object files,
# core files, and executables.
#
clean:
	rm -f $(PRG) a.out core *.o \#*

#
# Specific dependencies for building each object.
#
signal1.o:		$(OSNAME)/signal1.c
	$(CC) $(CFLAGS) -c -o $@ $?
signal2.o:		$(OSNAME)/signal2.c
	$(CC) $(CFLAGS) -c -o $@ $?
signal3.o:		$(OSNAME)/signal3.c
	$(CC) $(CFLAGS) $(EXTRACFLAGS) -c -o $@ $?
signal4.o:		$(OSNAME)/signal4.c
	$(CC) $(CFLAGS) -c -o $@ $?
timeout1.o:		common/timeout1.c
	$(CC) $(CFLAGS) -c -o $@ $?
timeout2.o:		common/timeout2.c
	$(CC) $(CFLAGS) -c -o $@ $?
timeout3.o:		common/timeout3.c
	$(CC) $(CFLAGS) -c -o $@ $?
