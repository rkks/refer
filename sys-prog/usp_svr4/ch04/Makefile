#
# Makefile for example programs in Chapter 4 of "Systems Programming
# for SVR4-Based UNIX Systems" by David A. Curry.
#

#
# Pull in os-specific definitions.
#
include Makedefs.$(OSNAME)

#
# Programs to build.
#
PRG=	append-buf append-char append-line cost-fmt cost printf-float \
	printf-int scanf-int seeker

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
append-buf.o:		common/append-buf.c
	$(CC) $(CFLAGS) -c -o $@ $?
append-char.o:		common/append-char.c
	$(CC) $(CFLAGS) -c -o $@ $?
append-line.o:		common/append-line.c
	$(CC) $(CFLAGS) -c -o $@ $?
cost-fmt.o:		common/cost-fmt.c
	$(CC) $(CFLAGS) -c -o $@ $?
cost.o:			common/cost.c
	$(CC) $(CFLAGS) -c -o $@ $?
printf-float.o:		common/printf-float.c
	$(CC) $(CFLAGS) -c -o $@ $?
printf-int.o:		common/printf-int.c
	$(CC) $(CFLAGS) -c -o $@ $?
scanf-int.o:		common/scanf-int.c
	$(CC) $(CFLAGS) -c -o $@ $?
seeker.o:		$(OSNAME)/seeker.c
	$(CC) $(CFLAGS) -c -o $@ $?
