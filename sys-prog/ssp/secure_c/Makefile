#*******************************************************************************
#
#	Name:			Makefile
#
#	Description:	This is the Makefile for the examples in the Secure C
#					Programming chapter of Solaris Systems Programming.
#
#
#			Copyright © 2001-2004 by Rich Teer.  All rights reserved.
#
#*******************************************************************************

include ../Make.defines

TARGETS = bracketing break_chroot

all sol-2.5 sol-2.5.1 sol-2.6 sol-7 sol-8 sol-9: $(TARGETS)

clean:
	@rm -f $(TARGETS) $(CLEAN_TARGETS)

bracketing: bracketing.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

break_chroot: break_chroot.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)
