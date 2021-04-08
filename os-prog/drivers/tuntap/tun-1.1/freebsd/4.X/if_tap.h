/*
 * Copyright (c) 1999 Maksim Yevmenkin <m_evmenkin@yahoo.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * BASED ON:
 * -------------------------------------------------------------------------
 *
 *	$NetBSD: if_tun.h,v 1.5 1994/06/29 06:36:27 cgd Exp $
 *
 * Copyright (c) 1988, Julian Onions <jpo@cs.nott.ac.uk>
 * Nottingham University 1987.
 */

/*
 *
 *	$Id: if_tap.h,v 1.1 2000/04/24 20:47:33 maxk Exp $
 *
 */

#ifndef _NET_IF_TAP_H_
#define _NET_IF_TAP_H_

/* Maximum receive packet size (hard limit) */
#define	TAPMRU		16384

struct tapinfo {
	int	baudrate;		/* linespeed */
	short	mtu;			/* maximum transmission unit */
	u_char	type;			/* ethernet, tokenring, etc. */
	u_char	dummy;			/* place holder */
};

/* ioctl's for get/set debug */
#define	TAPSDEBUG	_IOW('t', 90, int)
#define	TAPGDEBUG	_IOR('t', 89, int)
#define	TAPSIFINFO	_IOW('t', 91, struct tapinfo)
#define	TAPGIFINFO	_IOR('t', 92, struct tapinfo)

#ifdef _KERNEL
struct tap_softc {
	struct arpcom	arpcom;			/* ethernet common data      */
#define tap_if		arpcom.ac_if

	u_short		tap_flags;		/* misc flags                */
#define	TAP_OPEN	0x0001
#define	TAP_INITED	0x0002
#if 0 /* XXX: not used */
#define	TAP_RCOLL	0x0004
#define	TAP_IASET	0x0008
#define	TAP_DSTADDR	0x0010
#endif
#define	TAP_RWAIT	0x0040
#define	TAP_ASYNC	0x0080
#define TAP_READY       (TAP_OPEN|TAP_INITED)

	pid_t		 tap_pid;		/* PID of process to open    */
	struct sigio	*tap_sigio;		/* information for async I/O */
	struct selinfo	 tap_rsel;		/* read select               */
	struct selinfo	 tap_wsel;		/* write select (not used)   */

	struct ifqueue	 if_rcv;		/* receive queue             */
};
#endif /* _KERNEL */


#endif /* !_NET_IF_TAP_H_ */
