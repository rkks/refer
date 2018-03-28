#*******************************************************************************
#
#	Name:			Makefile
#
#	Description:	This is the Makefile for the examples in the Terminal I/O
#					chapter of Solaris Systems Programming.
#
#
#			Copyright © 2001-2004 by Rich Teer.  All rights reserved.
#
#*******************************************************************************

include ../Make.defines

TARGETS = cfsetispeed change_char char_size isatty modes.o readpass test_modes \
	ttyname winsize

all sol-2.5 sol-2.5.1 sol-2.6 sol-7 sol-8 sol-9: $(TARGETS)

clean:
	@rm -f $(TARGETS) $(CLEAN_TARGETS)

cfsetispeed: cfsetispeed.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

change_char: change_char.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

char_size: char_size.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

isatty: isatty.c
	$(CC) $(CFLAGS) -o $@ $@.c

modes.o: modes.c
	$(CC) $(CFLAGS) -c modes.c

readpass: readpass.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

test_modes: test_modes.c modes.o $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c modes.o $(SSP_LIB)

ttyname: ttyname.c
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

winsize: winsize.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)
