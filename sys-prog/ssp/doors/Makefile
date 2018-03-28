#*******************************************************************************
#
#	Name:			Makefile
#
#	Description:	This is the Makefile for the examples in the Doors chapter
#					of Solaris Systems Programming.
#
#
#			Copyright © 2001-2004 by Rich Teer.  All rights reserved.
#
#*******************************************************************************

include ../Make.defines

CFLAGS += $(MTFLAGS)

TARGETS = desc_door_cli desc_door_srv door_cli1 door_cli2 door_srv1 door_srv2 \
	door_srv3 doorinfo term_door_cli1 term_door_cli2 term_door_srv1 term_door_srv2 \
	term_door_srv3 unref_door_srv1 unref_door_srv2 unref_door_srv3

all sol-2.6 sol-7 sol-8 sol-9: $(TARGETS)

sol-2.5 sol-2.5.1:

clean:
	@rm -f $(TARGETS) $(CLEAN_TARGETS)

desc_door_cli: desc_door_cli.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB) -ldoor

desc_door_srv: desc_door_srv.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB) -ldoor

door_cli1: door_cli1.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB) -ldoor

door_cli2: door_cli1.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB) -ldoor

door_srv1: door_srv1.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB) -ldoor

door_srv2: door_srv2.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB) -ldoor

door_srv3: door_srv3.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB) -ldoor

doorinfo: doorinfo.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB) -ldoor

term_door_cli1: term_door_cli1.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB) -ldoor

term_door_cli2: term_door_cli2.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB) -ldoor

term_door_srv1: term_door_srv1.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB) -ldoor

term_door_srv2: term_door_srv2.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB) -ldoor

term_door_srv3: term_door_srv3.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB) -ldoor

unref_door_srv1: unref_door_srv1.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB) -ldoor

unref_door_srv2: unref_door_srv2.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB) -ldoor

unref_door_srv3: unref_door_srv3.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB) -ldoor
