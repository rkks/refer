#*******************************************************************************
#
#	Name:			Makefile
#
#	Description:	This is the Makefile for the examples in the Advanced I/O
#					chapter of Solaris Systems Programming.
#
#
#			Copyright © 2001-2004 by Rich Teer.  All rights reserved.
#
#*******************************************************************************

include ../Make.defines

S9_TARGETS = copy deadlock devpoll devzero fchmodat fd_set get_acl isastream \
	list_attrs list_mods lock_perf mcopy mlock nonblock poll sendfile1 sendfile2 \
	sendfilev set_acl ssp_lock_test.o writev

S7_TARGETS = copy deadlock devpoll devzero fd_set get_acl isastream list_mods \
	lock_perf mcopy mlock nonblock poll set_acl ssp_lock_test.o writev

S25_TARGETS = copy deadlock devzero fd_set get_acl isastream list_mods \
	lock_perf mcopy mlock nonblock poll set_acl ssp_lock_test.o writev

TARGETS = $(S9_TARGETS)

all sol-9: $(TARGETS)

sol-7 sol-8: $(S7_TARGETS)

sol-2.5 sol-2.5.1 sol-2.6: $(S25_TARGETS)

clean:
	@rm -f $(TARGETS) $(CLEAN_TARGETS)

copy: copy.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

deadlock: deadlock.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

devpoll: devpoll.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

devzero: devzero.c $(SSP_LIB) ../signals/tell_wait.o
	$(CC) $(CFLAGS) -D_POSIX_C_SOURCE=199309L -o $@ $@.c $(SSP_LIB) ../signals/tell_wait.o

../signals/tell_wait.o: ../signals/tell_wait.c
	$(CC) $(CFLAGS) -c -D_POSIX_PTHREAD_SEMANTICS -o $@ ../signals/tell_wait.c

fchmodat: fchmodat.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

fd_set: fd_set.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

get_acl: get_acl.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB) -lsec

isastream: isastream.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

list_attrs: list_attrs.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

list_mods: list_mods.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

lock_perf: lock_perf.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

mcopy: mcopy.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

mlock: mlock.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

nonblock: nonblock.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

poll: poll.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

sendfile1: sendfile1.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB) -lsendfile

sendfile2: sendfile2.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB) -lsendfile

sendfilev: sendfilev.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB) -lsendfile

set_acl: set_acl.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB) -lsec

ssp_lock_test.o: ssp_lock_test.c
	$(CC) $(CFLAGS) -c ssp_lock_test.c

writev: writev.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)
