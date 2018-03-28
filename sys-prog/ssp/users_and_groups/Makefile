#*******************************************************************************
#
#	Name:			Makefile
#
#	Description:	This is the Makefile for the examples in the Users and
#					Groups chapter of Solaris Systems Programming.
#
#
#			Copyright © 2001-2004 by Rich Teer.  All rights reserved.
#
#*******************************************************************************

include ../Make.defines

S9_TARGETS = cuserid getgrnam getgroups getlogin getpwnam getspnam getuid lastlog \
	lgroups list_shells lusers lusersp new_crypt old_crypt passwd_comp ssp_who

S26_TARGETS = cuserid getgrnam getgroups getlogin getpwnam getspnam getuid lastlog \
	lgroups list_shells lusers lusersp old_crypt passwd_comp ssp_who

S25_TARGETS = cuserid getgrnam getgroups getlogin getpwnam getspnam getuid lastlog \
	lgroups list_shells lusers lusersp ssp_who

TARGETS = $(S9_TARGETS)

all: $(TARGETS)

sol-2.6 sol-7 sol-8 sol-9: $(S26_TARGETS)

sol-2.5 sol-2.5.1: $(S25_TARGETS)

clean:
	@rm -f $(TARGETS) $(CLEAN_TARGETS)

cuserid: cuserid.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

getgrnam: getgrnam.c
	$(CC) $(CFLAGS) -o $@ $@.c

getgroups: getgroups.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

getlogin: getlogin.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

getpwnam: getpwnam.c
	$(CC) $(CFLAGS) -o $@ $@.c

getspnam: getspnam.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

getuid: getuid.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

lastlog: lastlog.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

lgroups: lgroups.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

list_shells: list_shells.c
	$(CC) $(CFLAGS) -o $@ $@.c

lusers: lusers.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

lusersp: lusersp.c
	$(CC) $(CFLAGS) -o $@ $@.c

new_crypt: new_crypt.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

old_crypt: old_crypt.c
	$(CC) $(CFLAGS) -o $@ $@.c

passwd_comp: passwd_comp.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

ssp_who: ssp_who.c
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)
