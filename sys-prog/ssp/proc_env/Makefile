#*******************************************************************************
#
#	Name:			Makefile
#
#	Description:	This is the Makefile for the examples in the Environment
#					of a UNIX Process chapter of Solaris Systems Programming.
#
#
#			Copyright © 2001-2004 by Rich Teer.  All rights reserved.
#
#*******************************************************************************

include ../Make.defines

S26_TARGETS = atexit getenv getexecname hello longjmp nested setjmp ssp_echo

S25_TARGETS = atexit getenv hello longjmp nested setjmp ssp_echo

TARGETS = $(S26_TARGETS)

all sol-2.6 sol-7 sol-8 sol-9: $(TARGETS)

sol-2.5 sol-2.5.1: $(S25_TARGETS)

clean:
	@rm -f $(TARGETS) $(CLEAN_TARGETS)

atexit: atexit.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

getenv: getenv.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

getexecname: getexecname.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

hello: hello.c
	$(CC) $(CFLAGS) -o $@ $@.c

longjmp: longjmp.c
	$(CC) $(CFLAGS) -o $@ $@.c

nested: nested.c
	$(CC) $(CFLAGS) -o $@ $@.c

setjmp: setjmp.c
	$(CC) $(CFLAGS) -o $@ $@.c

ssp_echo: ssp_echo.c
	$(CC) $(CFLAGS) -o $@ $@.c
