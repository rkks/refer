#
# Makefile for example programs in Chapter 16 of "Systems Programming
# for SVR4-Based UNIX Systems" by David A. Curry.
#

#
# Pull in os-specific definitions.
#
include Makedefs.$(OSNAME)

#
# Programs to build.
#
# STD have all their functions defined in the standard C library.
# GEN have some of their functions defined in the -lgen library.
#
STD=	assert bsearch ftw hsearch lsearch nftw printenv qsort \
	tsearch

DBM=	ndbm

GEN=	gmatch pathfind regexp

#
# Build all the programs.  To build a single program, issue the command
# "../build-examples name-of-program".
#
all:	$(STD) $(GEN)

$(STD):	$$@.o
	$(CC) -o $@ $@.o

$(DBM): $$@.o
	$(CC) -o $@ $@.o $(DBMLIBS)

$(GEN): $$@.o
	$(CC) -o $@ $@.o $(GENLIBS)

#
# Issue the command "../build-examples clean" to get rid of object files,
# core files, and executables.
#
clean:
	rm -f $(STD) $(DBM) $(GEN) a.out core people.dir people.pag *.o \#*

#
# Specific dependencies for building each object.
#
assert.o:		common/assert.c
	$(CC) $(CFLAGS) -c -o $@ $?
bsearch.o:		common/bsearch.c
	$(CC) $(CFLAGS) -c -o $@ $?
ftw.o:			common/ftw.c
	$(CC) $(CFLAGS) -c -o $@ $?
gmatch.o:		$(OSNAME)/gmatch.c
	$(CC) $(CFLAGS) -c -o $@ $?
hsearch.o:		common/hsearch.c
	$(CC) $(CFLAGS) -c -o $@ $?
lsearch.o:		common/lsearch.c
	$(CC) $(CFLAGS) -c -o $@ $?
ndbm.o:			common/ndbm.c
	$(CC) $(CFLAGS) -c -o $@ $?
nftw.o:			$(OSNAME)/nftw.c
	$(CC) $(CFLAGS) -c -o $@ $?
pathfind.o:		$(OSNAME)/pathfind.c
	$(CC) $(CFLAGS) -c -o $@ $?
printenv.o:		common/printenv.c
	$(CC) $(CFLAGS) -c -o $@ $?
qsort.o:		common/qsort.c
	$(CC) $(CFLAGS) -c -o $@ $?
regexp.o:		common/regexp.c
	$(CC) $(CFLAGS) -c -o $@ $?
tsearch.o:		common/tsearch.c
	$(CC) $(CFLAGS) -c -o $@ $?
