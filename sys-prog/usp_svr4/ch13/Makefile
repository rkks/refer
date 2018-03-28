#
# Makefile for example programs in Chapter 13 of "Systems Programming
# for SVR4-Based UNIX Systems" by David A. Curry.
#

#
# Pull in os-specific definitions.
#
include Makedefs.$(OSNAME)

#
# Programs to build.
#
STD=	fifo-clnt fifo-srvr msq-clnt msq-srvr pipedate pipemail popen

NET=	socket-clnt socket-srvr

#
# Build all the programs.  To build a single program, issue the command
# "../build-examples name-of-program".
#
all:	$(STD) $(NET)

$(STD):	$$@.o
	$(CC) -o $@ $@.o

$(NET):	$$@.o
	$(CC) -o $@ $@.o $(NETLIBS)

#
# Issue the command "../build-examples clean" to get rid of object files,
# core files, and executables.
#
clean:
	rm -f $(STD) $(NET) a.out core *.o mysocket myfifo \#*

#
# Specific dependencies for building each object.
#
fifo-clnt.o:		common/fifo-clnt.c
	$(CC) $(CFLAGS) -c -o $@ $?
fifo-srvr.o:		common/fifo-srvr.c
	$(CC) $(CFLAGS) -c -o $@ $?
msq-clnt.o:		common/msq-clnt.c
	$(CC) $(CFLAGS) -c -o $@ $?
msq-srvr.o:		common/msq-srvr.c
	$(CC) $(CFLAGS) -c -o $@ $?
pipedate.o:		common/pipedate.c
	$(CC) $(CFLAGS) -c -o $@ $?
pipemail.o:		common/pipemail.c
	$(CC) $(CFLAGS) -c -o $@ $?
popen.o:		common/popen.c
	$(CC) $(CFLAGS) -c -o $@ $?
socket-clnt.o:		common/socket-clnt.c
	$(CC) $(CFLAGS) -c -o $@ $?
socket-srvr.o:		common/socket-srvr.c
	$(CC) $(CFLAGS) -c -o $@ $?
