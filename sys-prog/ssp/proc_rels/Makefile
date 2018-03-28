#*******************************************************************************
#
#	Name:			Makefile
#
#	Description:	This is the Makefile for the examples in the Process
#					Relationships chapter of Solaris Systems Programming.
#
#
#			Copyright © 2001-2004 by Rich Teer.  All rights reserved.
#
#*******************************************************************************

include ../Make.defines

TARGETS = pg_orphan

all sol-2.5 sol-2.5.1 sol-2.6 sol-7 sol-8 sol-9: $(TARGETS)

clean:
	@rm -f $(TARGETS) $(CLEAN_TARGETS)

pg_orphan: pg_orphan.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)
