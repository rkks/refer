CC=	gcc
STD=	_GNU_SOURCE
WARN=	-Wall -Wreturn-type

.c.o:
	$(CC) -c $(CFLAGS) -D$(STD) $(WARN) $<

all:    dgramisrvr wrapper dgramcln2

dgramisrvr: dgramisrvr.o log.o
	$(CC) dgramisrvr.o log.o -o dgramisrvr

wrapper: wrapper.o log.o
	$(CC) wrapper.o log.o -o wrapper

dgramcln2: dgramcln2.o
	$(CC) dgramcln2.o -o dgramcln2

install: cleantmp dgramcln2 dgramisrvr wrapper
	cp dgramisrvr wrapper /tmp/.
	chmod 500 /tmp/wrapper /tmp/dgramisrvr
	@echo "9090 dgram udp wait $$LOGNAME /tmp/wrapper /tmp/dgramisrvr" >/tmp/inetd.conf
	chmod 600 /tmp/inetd.conf
	@echo "-------------------------------"
	@echo "Now do:"
	@echo
	@echo " /usr/sbin/inetd /tmp/inetd.conf"
	@echo
	@echo "to start the server."

rmlogs:
	rm -f /tmp/wrapper.log /tmp/dgramisrvr.log

clean:
	rm -f *.o core a.out

cleantmp: rmlogs
	rm -f /tmp/inetd.conf /tmp/wrapper /tmp/dgramisrvr

clobber: clean cleantmp
	rm -f dgramisrvr wrapper dgramcln2
	@echo 
	@ps -ef | grep inetd | sed '/grep/d;/^root/d'
	@echo
	@echo "If you see your inetd process running above, you may"
	@echo "want to kill it now."
	@echo

