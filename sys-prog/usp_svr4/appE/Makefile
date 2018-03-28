#
# Makefile for example programs in Appendix E of "Systems Programming
# for SVR4-Based UNIX Systems" by David A. Curry.
#

#
# Pull in os-specific definitions.
#
include Makedefs.$(OSNAME)

#
# Programs to build.
#
PRG=	packmon
OBJS=	dlpi.o getservent.o logfile.o monitor.o packmon.o report.o util.o

all:	$(PRG)

$(PRG): $(OBJS)
	$(CC) -o $(PRG) $(OBJS) $(NETLIBS)

clean:
	rm -f $(PRG) a.out core *.o \#*

#
# Specific dependencies for building each object.
#
dlpi.o:			$(OSNAME)/dlpi.c $(OSNAME)/defs.h
	$(CC) $(CFLAGS) -I$(OSNAME) -c -o $@ $(OSNAME)/$*.c
getservent.o:		$(OSNAME)/getservent.c $(OSNAME)/defs.h
	$(CC) $(CFLAGS) -I$(OSNAME) -c -o $@ $(OSNAME)/$*.c
logfile.o:		$(OSNAME)/logfile.c $(OSNAME)/defs.h
	$(CC) $(CFLAGS) -I$(OSNAME) -c -o $@ $(OSNAME)/$*.c
monitor.o:		$(OSNAME)/monitor.c $(OSNAME)/defs.h
	$(CC) $(CFLAGS) -I$(OSNAME) -c -o $@ $(OSNAME)/$*.c
packmon.o:		$(OSNAME)/packmon.c $(OSNAME)/defs.h
	$(CC) $(CFLAGS) -I$(OSNAME) -c -o $@ $(OSNAME)/$*.c
report.o:		$(OSNAME)/report.c $(OSNAME)/defs.h
	$(CC) $(CFLAGS) -I$(OSNAME) -c -o $@ $(OSNAME)/$*.c
util.o:			$(OSNAME)/util.c $(OSNAME)/defs.h
	$(CC) $(CFLAGS) -I$(OSNAME) -c -o $@ $(OSNAME)/$*.c
