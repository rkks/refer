#*******************************************************************************
#
#	Name:			Makefile
#
#	Description:	This is the Makefile for the examples in the Working With
#					File Systems chapter of Solaris Systems Programming.
#
#
#			Copyright © 2001-2004 by Rich Teer.  All rights reserved.
#
#*******************************************************************************

include ../Make.defines

S8_TARGETS = diskuse hasmntopt mnttab settag sspmount sspumount statvfs ustat vfstab

S7_TARGETS = diskuse hasmntopt mnttab statvfs ustat vfstab

S25_TARGETS = hasmntopt mnttab statvfs ustat vfstab

TARGETS = $(S8_TARGETS)

all sol-8 sol-9: $(TARGETS)

sol-7: $(S7_TARGETS)

sol-2.5 sol-2.5.1 sol-2.6: $(S25_TARGETS)

clean:
	@rm -f $(TARGETS) $(CLEAN_TARGETS)

diskuse: diskuse.c $(SSP_LIB)
	$(CC) $(CFLAGS) -D_FILE_OFFSET_BITS=64 -o $@ $@.c $(SSP_LIB)

hasmntopt: hasmntopt.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

mnttab: mnttab.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

settag: settag.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

sspmount: sspmount.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

sspumount: sspumount.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

statvfs: statvfs.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

ustat: ustat.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)

vfstab: vfstab.c $(SSP_LIB)
	$(CC) $(CFLAGS) -o $@ $@.c $(SSP_LIB)
