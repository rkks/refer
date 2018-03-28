#	$NetBSD: Makefile.ioctl-c,v 1.16 2004/07/10 12:12:49 tron Exp $

# NOTE: <bsd.own.mk> needs to be previously .included for NETBSDSRCDIR

DEPFILEGLOB=	${DESTDIR}/usr/include/*/*.h

.if !make(cleandir) && !make(obj) && !make(includes)
DEPFILES != egrep -l '(_IO\(|_IOR\(|_IOW\(|_IORW\()' ${DEPFILEGLOB}
.endif

ioctl.c: mkioctls Makefile ${DEPFILES}
	${_MKTARGET_CREATE}
	CC="${CC}" DESTDIR=${DESTDIR} \
	    ${HOST_SH} ./mkioctls \
	    ${DEPFILES} >${.TARGET}

SRCS+=		ioctl.c
CLEANFILES+=	ioctl.c

${DEPFILES}: .PRECIOUS
