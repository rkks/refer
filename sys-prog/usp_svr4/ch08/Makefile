#
# Makefile for example programs in Chapter 8 of "Systems Programming
# for SVR4-Based UNIX Systems" by David A. Curry.
#

#
# Pull in os-specific definitions.
#
include Makedefs.$(OSNAME)

#
# Programs to build.
#
PRG=	lastlog newlistfiles whom

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
lastlog.o:		$(OSNAME)/lastlog.c
	$(CC) $(CFLAGS) -c -o $@ $?
newlistfiles.o:		$(OSNAME)/newlistfiles.c
	$(CC) $(CFLAGS) -c -o $@ $?
whom.o:			$(OSNAME)/whom.c
	$(CC) $(CFLAGS) -c -o $@ $?
