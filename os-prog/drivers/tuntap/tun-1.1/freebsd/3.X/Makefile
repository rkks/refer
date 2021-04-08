# if_tap module Makefile
# (c) Copyright 1999 Maksim Yevmenkin <m_evmenkin@yahoo.com>
#
# Adjust number of TAP devices (NTAP variable)
# To enable Ethernet bridging support adjust BRIDGE variable before compiling
# To enable BPF support adjust NBPFILTER variable before compiling
#
#	$Id: Makefile,v 1.1 2000/04/24 20:47:33 maxk Exp $

##
# Name of the module
##
KMOD=		if_tap

##
# number of TAP devices
##
NTAP?=		4

##
# enable/disable BPF on TAP set it to 1/0
##
NBPFILTER?=	1

##
# C flags. ${COPTS} and ${CWARNFLAGS} are defined in bsd.kern.mk.
##
CFLAGS+=	${COPTS} -DKERNEL ${CWARNFLAGS} -DKLD_MODULE
CFLAGS+=	-nostdinc -I- -I. -I@
CFLAGS+=	${DEBUG_FLAGS}

##
# path to the kernel source tree. adjust if needed.
##
KERN=		/usr/src/sys

##
# destination directory (where to install). adjust if needed.
##
DESTDIR?=	/modules

##
# set to ``1'' to enable ethernet bridging support. it also must be
# enabled in kernel (see ``bridge(4)'' and ``options BRIDGE'' for details).
##
BRIDGE?=	0

############################################################################

SRCS=		if_tap.c 
SRCS+=		bpfilter.h opt_bdg.h opt_inet.h opt_devfs.h
SRCS+=		tap.h vnode_if.h
ILINKS=		@ machine
OBJS=		${SRCS:N*.h:R:S/$/.o/g}	

CLEANFILES+=	${ILINKS} ${OBJS} 
CLEANFILES+=	bpfilter.h opt_bdg.h opt_inet.h opt_devfs.h 
CLEANFILES+=	tap.h vnode_if.h vnode_if.c
CLEANFILES+=	${KMOD}.kld ${KMOD}.ko
.if ${OBJFORMAT} == elf
CLEANFILES+=	setdef0.c setdef1.c setdefs.h
CLEANFILES+=	setdef0.o setdef1.o
.endif

all:		${SRCS} ${KMOD}.ko

clean:
.if defined(CLEANFILES) && !empty(CLEANFILES)
	rm -f ${CLEANFILES}
.endif

install:	all
	${INSTALL} ${COPY} -o ${KMODOWN} -g ${KMODGRP} -m ${KMODMODE} \
				${_INSTALLFLAGS} ${KMOD}.ko ${DESTDIR}

${ILINKS}:
	@case ${.TARGET} in						\
		@)		P=${KERN} ;;				\
		machine)	P=${KERN}/${MACHINE_ARCH}/include ;;	\
	esac ;								\
	echo "${.TARGET} -> $$P" ;					\
	ln -s $$P ${.TARGET}

bpfilter.h:
	echo "#define NBPFILTER ${NBPFILTER}" > bpfilter.h

opt_bdg.h:
.if ${BRIDGE} == 1
	echo "#define BRIDGE 1" > opt_bdg.h
.else
	echo "" > opt_bdg.h
.endif

opt_inet.h:
	echo "#define INET 1" > opt_inet.h

opt_devfs.h:
	touch opt_devfs.h

tap.h:
	echo "#define NTAP ${NTAP}" > tap.h

vnode_if.h:	${ILINKS}
	sh @/kern/vnode_if.sh @/kern/vnode_if.src

${KMOD}.kld:	${OBJS}
.if ${OBJFORMAT} == elf
	gensetdefs ${OBJS}
	${CC} ${CFLAGS} -c setdef0.c
	${CC} ${CFLAGS} -c setdef1.c
	${LD} ${LDFLAGS} -r -o ${.TARGET} setdef0.o ${OBJS} setdef1.o
.else
	${LD} ${LDFLAGS} -r -o ${.TARGET} ${OBJS}
.endif

${KMOD}.ko:	${KMOD}.kld
	${LD} -Bshareable ${LDFLAGS} -o ${.TARGET} ${KMOD}.kld

.include <bsd.kern.mk>
