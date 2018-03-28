#
# $Header$
#
# Makefile for the examples shown in "Systems Programming for SVR4-Based
# UNIX Systems," by David A. Curry, published by O'Reilly and Associates.
#
# Author:
#	David A. Curry
#
# $Log$
#

#
# Pull in os-specific definitions.
#
include Makedefs.$(OSNAME)

#
# Directories to build.
#
SUBDIRS=	ch02 ch03 ch04 ch05 ch06 ch07 ch08 ch09 ch10 ch11 ch12 ch13 \
		ch14 ch15 ch16 appB appC appD appE

all:		build

build:
	@case '${MFLAGS}' in *[ik]*) set +e;; esac; \
	for i in $(SUBDIRS);\
	do \
		(cd $$i; echo "making in $$i..."; \
			$(MAKE) $(MFLAGS) CC=$(CC) CFLAGS="$(CFLAGS)" \
				OSNAME=$(OSNAME)); \
	done

clean:
	@case '${MFLAGS}' in *[ik]*) set +e;; esac; \
	for i in $(SUBDIRS);\
	do \
		(cd $$i; echo "cleaning in $$i..."; \
			$(MAKE) $(MFLAGS) OSNAME=$(OSNAME) clean); \
	done
