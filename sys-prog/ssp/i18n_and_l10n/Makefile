#*******************************************************************************
#
#	Name:			Makefile
#
#	Description:	This is the Makefile for the examples in the
#					An Internationalisation and Localisation Primer
#					appendix of Solaris Systems Programming.
#
#
#			Copyright © 2001-2004 by Rich Teer.  All rights reserved.
#
#*******************************************************************************

include ../Make.defines

TARGETS = hello

all sol-2.6 sol-7 sol-8 sol-9: $(TARGETS)

sol-2.5 sol-2.5.1:

clean:
	@rm -f $(TARGETS) $(CLEAN_TARGETS)

hello: hello.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)
