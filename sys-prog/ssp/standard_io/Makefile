#*******************************************************************************
#
#	Name:			Makefile
#
#	Description:	This is the Makefile for the example in the Standard I/O
#					Library chapter of Solaris Systems Programming.
#
#
#			Copyright © 2001-2004 by Rich Teer.  All rights reserved.
#
#*******************************************************************************

include ../Make.defines

S7_TARGETS = flockfile1 flockfile2 cp_fgetc cp_fgets cp_getc cp_getc_unlocked \
	printf1 printf2 printf3 printf4 printf5

S25_TARGETS = cp_fgetc cp_fgets cp_getc cp_getc_unlocked \
	printf1 printf2 printf3 printf4 printf5

TARGETS = $(S7_TARGETS)

all sol-7 sol-8 sol-9: $(TARGETS)

sol-2.5 sol-2.5.1 sol-2.6: $(S25_TARGETS)

clean:
	@rm -f $(TARGETS) $(CLEAN_TARGETS)

flockfile1: flockfile1.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB) -lpthread

flockfile2: flockfile2.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB) -lpthread

cp_fgetc: cp_fgetc.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

cp_fgets: cp_fgets.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

cp_getc: cp_getc.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

cp_getc_unlocked: cp_getc_unlocked.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

printf1: printf1.c
	$(CC) $(CFLAGS) -o $@ $@.c

printf2: printf2.c
	$(CC) $(CFLAGS) -o $@ $@.c

printf3: printf3.c
	$(CC) $(CFLAGS) -o $@ $@.c

printf4: printf4.c
	$(CC) $(CFLAGS) -o $@ $@.c

printf5: printf5.c
	$(CC) $(CFLAGS) -o $@ $@.c
