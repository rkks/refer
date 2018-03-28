#*******************************************************************************
#
#	Name:			Makefile
#
#	Description:	This is the Makefile for the examples in the Pseudo
#					Terminals chapter of Solaris Systems Programming.
#
#
#			Copyright © 2001-2004 by Rich Teer.  All rights reserved.
#
#*******************************************************************************

include ../Make.defines

S26_TARGETS = pckt pty pty_fork.o tflags

S25_TARGETS = pty pty_fork.o tflags

TARGETS = $(S26_TARGETS)

all sol-2.6 sol-7 sol-8 sol-9: $(TARGETS)

sol-2.5 sol-2.5.1: $(S25_TARGETS)

clean:
	@rm -f $(TARGETS) $(CLEAN_TARGETS)

pckt: pckt.c $(SSP_LIB) ../terminal_io/modes.o pty_fork.o
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB) ../terminal_io/modes.o pty_fork.o

pty: pty.c $(SSP_LIB) ../terminal_io/modes.o pty_fork.o
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB) ../terminal_io/modes.o pty_fork.o

../terminal_io/modes.o: ../terminal_io/modes.c
	$(CC) $(CFLAGS) -c -o $@ ../terminal_io/modes.c

pty_fork.o: pty_fork.c
	$(CC) $(CFLAGS) -c pty_fork.c

tflags: tflags.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)
