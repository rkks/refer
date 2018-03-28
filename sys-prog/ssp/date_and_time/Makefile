#*******************************************************************************
#
#	Name:			Makefile
#
#	Description:	This is the Makefile for the examples in the Date and Time
#					Operations chapter of Solaris Systems Programming.
#
#
#			Copyright © 2001-2004 by Rich Teer.  All rights reserved.
#
#*******************************************************************************

include ../Make.defines

TARGETS = gmtime leap_year mktime strftime tzset

all sol-2.5 sol-2.5.1 sol-2.6 sol-7 sol-8 sol-9: $(TARGETS)

clean:
	@rm -f $(TARGETS) $(CLEAN_TARGETS)

gmtime: gmtime.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c

leap_year: leap_year.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

mktime: mktime.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c

strftime: strftime.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c

tzset: tzset.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)
