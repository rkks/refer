#
# Makefile for example programs in Chapter 3 of "Systems Programming
# for SVR4-Based UNIX Systems" by David A. Curry.
#

#
# Pull in os-specific definitions.
#
include	Makedefs.$(OSNAME)

#
# Programs to build.
#
PRG=	append seeker

#
# Build all the programs.  To build a single program, issue the command
# "../build-examples name-of-program".
#
all:	$(PRG)

$(PRG):	$$@.o
	$(CC) -o $@ $@.o

#
# Issue the command "../build-examples clean" to get rid of object files,
# core files, and executables.
#
clean:
	rm -f $(PRG) a.out core *.o \#*

#
# Specific dependencies for building each object.
#
append.o:		common/append.c
	$(CC) $(CFLAGS) -c -o $@ $?
seeker.o:		common/seeker.c
	$(CC) $(CFLAGS) -c -o $@ $?
