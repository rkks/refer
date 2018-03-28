#*******************************************************************************
#
#	Name:			Makefile
#
#	Description:	This is the Makefile for the examples in the Daemon
#					Processes chapter of Solaris Systems Programming.
#
#
#			Copyright © 2001-2004 by Rich Teer.  All rights reserved.
#
#*******************************************************************************

include ../Make.defines

TARGETS = daemon_init.o genlog one_copy.o only_one syslog test_daemon

all sol-2.5 sol-2.5.1 sol-2.6 sol-7 sol-8 sol-9: $(TARGETS)

clean:
	@rm -f $(TARGETS) $(CLEAN_TARGETS)

daemon_init.o: daemon_init.c
	$(CC) $(CFLAGS) -c daemon_init.c

genlog: genlog.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

one_copy.o: one_copy.c
	$(CC) $(CFLAGS) -c one_copy.c

only_one: only_one.c one_copy.o $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c one_copy.o $(SSP_LIB)

syslog: syslog.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

test_daemon: test_daemon.c daemon_init.o $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c daemon_init.o $(SSP_LIB)
