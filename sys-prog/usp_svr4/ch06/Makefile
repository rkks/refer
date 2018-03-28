#
# Makefile for example programs in Chapter 6 of "Systems Programming
# for SVR4-Based UNIX Systems" by David A. Curry.
#

#
# Pull in os-specific definitions.
#
include Makedefs.$(OSNAME)

#
# Programs to build.
#
PRG=	catmap poll select

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
catmap.o:		$(OSNAME)/catmap.c
	$(CC) $(CFLAGS) -c -o $@ $?
poll.o:			common/poll.c
	$(CC) $(CFLAGS) -c -o $@ $?
select.o:		$(OSNAME)/select.c
	$(CC) $(CFLAGS) -c -o $@ $?
