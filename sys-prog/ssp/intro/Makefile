#*******************************************************************************
#
#	Name:			Makefile
#
#	Description:	This is the Makefile for the examples in the Introduction
#					chapter of Solaris Systems Programming.
#
#
#			Copyright © 2001-2004 by Rich Teer.  All rights reserved.
#
#*******************************************************************************

include ../Make.defines

TARGETS = copy1 copy2 getpid getuid large_file sign_ext ssp_ls sspsh1 sspsh2 times

all sol-2.5 sol-2.5.1 sol-2.6 sol-7 sol-8 sol-9: $(TARGETS)

clean:
	@rm -f $(TARGETS) $(CLEAN_TARGETS)

copy1: copy1.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

copy2: copy2.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

getpid: getpid.c
	$(CC) $(CFLAGS) -o $@ $@.c

getuid: getuid.c
	$(CC) $(CFLAGS) -o $@ $@.c

large_file: large_file.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

sign_ext: sign_ext.c
	$(CC) $(CFLAGS) -o $@ $@.c

ssp_ls: ssp_ls.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

sspsh1: sspsh1.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

sspsh2: sspsh2.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

times: times.c
	$(CC) $(CFLAGS) -o $@ $@.c
