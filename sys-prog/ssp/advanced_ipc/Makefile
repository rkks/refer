#*******************************************************************************
#
#	Name:			Makefile
#
#	Description:	This is the Makefile for the examples in the Advanced
#					Interprocess Communication chapter of Solaris Systems
#					Programming.
#
#
#			Copyright © 2001-2004 by Rich Teer.  All rights reserved.
#
#*******************************************************************************

include ../Make.defines

TARGETS = cs_conn.o cs_open1 cs_open2 opend1 opend2 pass_fd.o

all sol-2.5 sol-2.5.1 sol-2.6 sol-7 sol-8 sol-9: $(TARGETS)

clean:
	@rm -f $(TARGETS) $(CLEAN_TARGETS)

cs_conn.o: cs_conn.c
	$(CC) $(CFLAGS) -c cs_conn.c

cs_open1: cs_open1.c $(SSP_LIB) pass_fd.o
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB) pass_fd.o

cs_open2: cs_open2.c $(SSP_LIB) cs_conn.o pass_fd.o
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB) cs_conn.o pass_fd.o

opend1: opend1.c $(SSP_LIB) pass_fd.o
	$(CC) $(CFLAGS) -o $@ $@.c pass_fd.o $(SSP_LIB)

opend2: opend2.c $(SSP_LIB) cs_conn.o pass_fd.o ../daemons/daemon_init.o
	$(CC) $(CFLAGS) -o $@ $@.c cs_conn.o pass_fd.o ../daemons/daemon_init.o $(SSP_LIB)

../daemons/daemon_init.o: ../daemons/daemon_init.c
	$(CC) $(CFLAGS) -c -o $@ ../daemons/daemon_init.c

pass_fd.o: pass_fd.c
	$(CC) $(CFLAGS) -c pass_fd.c
