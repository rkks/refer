#
# Makefile for example programs in Chapter 15 of "Systems Programming
# for SVR4-Based UNIX Systems" by David A. Curry.
#

#
# Pull in os-specific definitions.
#
include Makedefs.$(OSNAME)

#
# Programs to build.
#
PRG=	client daytime server

#
# Build all the programs.  To build a single program, issue the command
# "../build-examples name-of-program".
#
all:	$(PRG)

$(PRG):	$$@.o
	$(CC) -o $@ $@.o $(NETLIBS)

#
# Issue the command "../build-examples clean" to get rid of object files,
# core files, and executables.
#
clean:
	rm -f $(PRG) a.out core *.o \#*

#
# Specific dependencies for building each object.
#
client.o:		$(OSNAME)/client.c
	$(CC) $(CFLAGS) -c -o $@ $?
daytime.o:		$(OSNAME)/daytime.c
	$(CC) $(CFLAGS) -c -o $@ $?
server.o:		$(OSNAME)/server.c
	$(CC) $(CFLAGS) -c -o $@ $?
