/*
 *  TUN - Universal TUN/TAP device driver.
 *  Copyright (C) 1999-2000 Maxim Krasnyansky <max_mk@yahoo.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 * 
 *  $Id: if_tun.h,v 1.1.1.1 2000/04/22 00:40:28 maxk Exp $
 */

#ifndef __IF_TUN_H
#define __IF_TUN_H

/* Uncomment to enable debugging */
/* #define TUN_DEBUG 1 */

#ifndef TUN_MAJOR
#define TUN_MAJOR 90
#endif

#ifdef __KERNEL__

#ifdef TUN_DEBUG
#define DBG  if(tun->debug)printk
#define DBG1 if(debug==2)printk
#else
#define DBG( a... )
#define DBG1( a... )
#endif

struct tun_struct {
	char 			name[8];
	unsigned long 		flags;

	struct fasync_struct    fasync;
	struct wait_queue	*read_wait;

	struct device 		dev;
	struct sk_buff_head	txq;
        struct enet_statistics 	stats;

#ifdef TUN_DEBUG	
	int debug;
#endif  
};

#ifndef MIN
#define MIN(a,b) ( (a)<(b) ? (a):(b) ) 
#endif

#endif /* __KERNEL__ */

/* Number of devices */
#define TUN_MAX_DEV	255

#define TUN_TUN_MINOR	0
#define TUN_TAP_MINOR   128
#define TUN_MINOR_MASK  0x7f

/* TX queue size */
#define TUN_TXQ_SIZE	10

/* Max frame size */
#define TUN_MAX_FRAME	4096

/* TUN device flags */
#define TUN_TUN_DEV 	0x01	
#define TUN_TAP_DEV	0x02
#define TUN_TYPE_MASK   0x0f

#define TUN_FASYNC	0x10
#define TUN_NOCHECKSUM	0x20

/* Ioctl defines */
#define TUNSETNOCSUM (('T'<< 8) | 200) 
#define TUNSETDEBUG  (('T'<< 8) | 201) 

#endif /* __IF_TUN_H */
