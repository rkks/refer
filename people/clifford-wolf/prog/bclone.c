/*
 *  WARNING: This is work in progress! Please check again later..
 *
 *
 *  A broadcast file transfer tool for large files (such as disc images)
 *
 *  Copyright (C) 2009  Clifford Wolf <clifford@clifford.at>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  -------------------------------------------------------------------------
 *
 *  Copyright for the crc16 implementation (taken from linux kernel 2.6):
 *
 *  Implements the standard CRC-16:
 *    Width 16
 *    Poly  0x8005 (x^16 + x^15 + x^2 + 1)
 *    Init  0
 *
 *  Copyright (c) 2005 Ben Gardner <bgardner@wabtec.com>
 *
 *  This source code is licensed under the GNU General Public License,
 *  Version 2. See the file COPYING for more details.
 *
 *  -------------------------------------------------------------------------
 *
 *  The bclone block size is 512 bytes.
 *
 *  The bclone image file format:
 *
 *    "BCLONE IMAGE FILE v1" 0x00
 *    <master image name without .img> 0x00
 *    <64bit big endian encoded file size>
 *
 *    For each 512 bytes block:
 *       <64bit big endian encoded offset of the block in this image file,
 *       as absolute byte address from the beginning of the file, or zero if
 *       the block should be taken from the master image, or -1 if the block
 *       has not been initialized yet>
 *
 *    Actual block data
 *
 *  The bclone protocol:
 *
 *    All packages are UDP broadcast packages. 
 *
 *    Header for all pkgs:
 *      "BCLONE PKG v1" 0x00
 *      <16bit big endian crc16 for the pkg starting with the opcode>
 *      <16bit big endian pkg size in bytes starting with the opcode>
 *      <16bit big endian opcode number>
 *
 *    Heartbeat (opcode 0x0000):
 *      <server name> 0x00
 *
 *    Create new file (opcode 0x0001):
 *      <new image name without file extension> 0x00
 *      <master image name without file extension> 0x00
 *      <64bit big endian encoded file size>
 *
 *    ACK Create new file (opcode 0x0002):
 *      <new image name without file extension> 0x00
 *      <master image name without file extension> 0x00
 *      <64bit big endian encoded file size>
 *
 *    NACK Create new file (opcode 0x0003):
 *      <new image name without file extension> 0x00
 *      <master image name without file extension> 0x00
 *      <64bit big endian encoded file size>
 *
 *    Request block data (opcode 0x0004):
 *      <image name without file extension> 0x00
 *      <64bit big endian file offset for this package>
 *      <MSB-first bitmap for blocks, set bits for requested blocks>
 *
 *    Block data (opcode 0x0005):
 *      <image name without file extension> 0x00
 *      <64bit big endian file offset for this package>
 *      <512 bytes file data>
 *
 *    Master reference (opcode 0x0006):
 *      <image name without file extension> 0x00
 *      <master image name without file extension> 0x00
 *      <64bit big endian file offset for this package>
 *      <MSB-first bitmap for blocks, set bits for blocks from master>
 *
 *    Request manifest (opcode 0x0007):
 *      <image name without file extension> 0x00
 *
 *    Manifest (opcode 0x0008):
 *      <image name without file extension> 0x00
 *      <master image name without file extension> 0x00
 *      <64bit big endian encoded file size>
 *
 */

#define _GNU_SOURCE

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/utsname.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <poll.h>


#define CHECK_VAL_OPEN (
#define CHECK_VAL_CLOSE )

#define CHECK(result, check)                                                            \
	CHECK_VAL_OPEN{                                                                 \
		typeof(result) _R = (result);                                           \
		if (!(_R check)) {                                                      \
			fprintf(stderr, "Error from '%s' (%d %s) in %s:%d.\n",          \
					#result, (int)_R, #check, __FILE__, __LINE__);  \
			fprintf(stderr, "ERRNO(%d): %s\n", errno, strerror(errno));     \
			abort();                                                        \
		}                                                                       \
		_R;                                                                     \
	}CHECK_VAL_CLOSE

enum bclone_opcode {
	HEARTBEAT = 0x0000,
	NEW_FILE = 0x0001,
	ACK_NEW_FILE = 0x0002,
	NACK_NEW_FILE = 0x0003,
	REQ_BLOCK_DATA = 0x0004,
	BLOCK_DATA = 0x0005,
	MASTER_REF = 0x0006,
	REQ_MANIFEST = 0x0007,
	MANIFEST = 0x0008
};

struct bclone_pkgdesc {
	char *sig;
	uint16_t crc16;
	uint8_t *crc16_data;
	uint16_t length;
	uint16_t opcode;
	char *name;
	char *image;
	char *master;
	uint64_t filesize;
	uint64_t fileoffset;
	uint16_t data_len;
	uint8_t *data;
	uint8_t *endp;
};

static int port = 26950;
static int server_rw_mode = 0;


/**************************************
 * CRC-16 implementation by Ben Gardner
 *************************************/

/** CRC table for the CRC-16. The poly is 0x8005 (x^16 + x^15 + x^2 + 1) */
static uint16_t const crc16_table[256] = {
	0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
	0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
	0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
	0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
	0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
	0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
	0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
	0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
	0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
	0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
	0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
	0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
	0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
	0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
	0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
	0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
	0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
	0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
	0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
	0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
	0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
	0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
	0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
	0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
	0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
	0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
	0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
	0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
	0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
	0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
	0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
	0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
};

static inline uint16_t crc16_byte(uint16_t crc, const uint8_t data)
{
	return (crc >> 8) ^ crc16_table[(crc ^ data) & 0xff];
}

static uint16_t crc16_calc(uint16_t crc, uint8_t const *buffer, uint16_t len)
{
	while (len--)
		crc = crc16_byte(crc, *buffer++);
	return crc;
}


/**********************
 * Low-level Networking
 *********************/

static uint8_t send_buffer[1024];
static int send_buffer_size;

static uint8_t recv_buffer[1024];
static int recv_buffer_size;
static struct bclone_pkgdesc recv_buffer_desc;

static int udp_sockfd;
static struct sockaddr_in udp_addr;

static void udp_init()
{
	udp_sockfd = CHECK(socket(PF_INET, SOCK_DGRAM, 0), != -1);

	memset(&udp_addr, 0, sizeof(udp_addr));
	udp_addr.sin_family = AF_INET;
	udp_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	udp_addr.sin_port = htons(port);

	int yes = 1;
	CHECK(setsockopt(udp_sockfd, SOL_SOCKET, SO_BROADCAST, (void*)&yes, sizeof(yes)), == 0);
	CHECK(setsockopt(udp_sockfd, SOL_SOCKET, SO_REUSEADDR, (void*)&yes, sizeof(yes)), == 0);

	CHECK(bind(udp_sockfd, (struct sockaddr *)&udp_addr, sizeof(udp_addr)), == 0);
}

static void udp_recv()
{
	struct pollfd pfd = { udp_sockfd, POLLIN, 0 };
	if (CHECK(poll(&pfd, 1, 1000), >= 0) == 0) {
		recv_buffer_size = 0;
	} else {
		recv_buffer_size = CHECK(recv(udp_sockfd, recv_buffer, 1024, 0), > 0);
	}
}

static int recv_any()
{
tail_recursion:
	udp_recv();

	if (recv_buffer_size <= 0)
		return 0;

	memset(&recv_buffer_desc, 0, sizeof(recv_buffer_desc));

	uint8_t *p = recv_buffer;

	recv_buffer_desc.sig = (char*)p;
	if (strcmp(recv_buffer_desc.sig, "BCLONE PKG v1")) {
		fprintf(stderr, "got package with invalid signature.\n");
		goto tail_recursion;
	}
	p += strlen((char*)p) + 1;

	recv_buffer_desc.crc16 = p[0] << 8 | p[1];
	recv_buffer_desc.crc16_data = p + 2 + 2;
	p += 2;

	recv_buffer_desc.length = p[0] << 8 | p[1];
	p += 2;

	if (((p + recv_buffer_desc.length) - recv_buffer) != recv_buffer_size) {
		fprintf(stderr, "embedded pkg size (%d) doesn't match udp pkg size (%d).\n",
			(p + recv_buffer_desc.length) - recv_buffer, recv_buffer_size);
		goto tail_recursion;
	}

	uint16_t crc16 = crc16_calc(0, recv_buffer_desc.crc16_data, recv_buffer_desc.length);
	if (crc16 != recv_buffer_desc.crc16) {
		fprintf(stderr, "embedded crc16 (%04x) doesn't match calucated crc16 (%04x).\n",
			recv_buffer_desc.crc16, crc16);
		goto tail_recursion;
	}

	recv_buffer_desc.endp = p + recv_buffer_desc.length;
	recv_buffer_desc.opcode = p[0] << 8 | p[1];
	p += 2;

#define CHECK_FIT_P(_x) do { if (p + _x > recv_buffer_desc.endp) goto pkg_does_not_fit; } while (0)

#define GET_NAME \
  CHECK_FIT_P(1); \
  recv_buffer_desc.name = (char*)p; \
  p += strnlen((char*)p, recv_buffer_desc.endp-p) + 1;

#define GET_IMAGE \
  CHECK_FIT_P(1); \
  recv_buffer_desc.image = (char*)p; \
  p += strnlen((char*)p, recv_buffer_desc.endp-p) + 1;

#define GET_MASTER \
  CHECK_FIT_P(1); \
  recv_buffer_desc.master = (char*)p; \
  p += strnlen((char*)p, recv_buffer_desc.endp-p) + 1;

#define GET_FILESIZE \
  CHECK_FIT_P(8); \
  recv_buffer_desc.filesize = \
      ((uint64_t)p[0] << 56) | ((uint64_t)p[1] << 48) | ((uint64_t)p[2] << 40) | ((uint64_t)p[3] << 32) | \
      ((uint64_t)p[4] << 24) | ((uint64_t)p[5] << 16) | ((uint64_t)p[6] << 8) | (uint64_t)p[7]; \
  p += 8;

#define GET_FILEOFFSET \
  CHECK_FIT_P(8); \
  recv_buffer_desc.fileoffset = \
      ((uint64_t)p[0] << 56) | ((uint64_t)p[1] << 48) | ((uint64_t)p[2] << 40) | ((uint64_t)p[3] << 32) | \
      ((uint64_t)p[4] << 24) | ((uint64_t)p[5] << 16) | ((uint64_t)p[6] << 8) | (uint64_t)p[7]; \
  p += 8;

#define GET_DATA \
  CHECK_FIT_P(0); \
  recv_buffer_desc.data = p; \
  recv_buffer_desc.data_len = recv_buffer_desc.endp-p; \
  p += recv_buffer_desc.data_len;

	switch (recv_buffer_desc.opcode)
	{
	case HEARTBEAT:
		GET_NAME
		break;
	case NEW_FILE:
		GET_IMAGE
		GET_MASTER
		GET_FILESIZE
		break;
	case ACK_NEW_FILE:
		GET_IMAGE
		GET_MASTER
		GET_FILESIZE
		break;
	case NACK_NEW_FILE:
		GET_IMAGE
		GET_MASTER
		GET_FILESIZE
		break;
	case REQ_BLOCK_DATA:
		GET_IMAGE
		GET_FILEOFFSET
		GET_DATA
		break;
	case BLOCK_DATA:
		GET_IMAGE
		GET_FILEOFFSET
		GET_DATA
		break;
	case MASTER_REF:
		GET_IMAGE
		GET_MASTER
		GET_FILEOFFSET
		GET_DATA
		break;
	case REQ_MANIFEST:
		GET_IMAGE
		break;
	case MANIFEST:
		GET_IMAGE
		GET_MASTER
		GET_FILESIZE
		break;
	}

#undef CHECK_FIT_P
#undef GET_NAME
#undef GET_IMAGE
#undef GET_MASTER
#undef GET_FILESIZE
#undef GET_FILEOFFSET
#undef GET_DATA

	if (p != recv_buffer_desc.endp) {
pkg_does_not_fit:
		fprintf(stderr, "pkg does not fit parser expectations.\n");
		goto tail_recursion;
	}

	return 1;
}

static void udp_send()
{
	CHECK(sendto(udp_sockfd, send_buffer, send_buffer_size, 0, (struct sockaddr*)&udp_addr, sizeof(udp_addr)), == send_buffer_size);
}

static uint8_t *crc16_p;
static uint8_t *size_p;
static uint8_t *body_p;

static uint8_t *send_begin(uint16_t opcode)
{
	uint8_t *p = send_buffer;
	strcpy((char*)p, "BCLONE PKG v1");
	p += strlen((char*)p) + 1;

	crc16_p = p;
	p += 2;

	size_p = p;
	p += 2;

	body_p = p;
	p[0] = (opcode & 0xff00) >> 8;
	p[1] = (opcode & 0x00ff) >> 0;
	p += 2;

	return p;
}

static void send_finish(uint8_t *p)
{
	uint16_t size = p - body_p;
	uint16_t crc16 = crc16_calc(0, body_p, size);

	size_p[0] = (size & 0xff00) >> 8;
	size_p[1] = (size & 0x00ff) >> 0;

	crc16_p[0] = (crc16 & 0xff00) >> 8;
	crc16_p[1] = (crc16 & 0x00ff) >> 0;

	send_buffer_size = size;
	udp_send();
}

static void send_heartbeat(const char *name)
{
	uint8_t *p = send_begin(HEARTBEAT);

	strcpy((char*)p, name);
	p += strlen((char*)p) + 1;

	send_finish(p);
}

static void send_new_file(const char *image, const char *master, uint64_t size)
{
	uint8_t *p = send_begin(NEW_FILE);

	strcpy((char*)p, image);
	p += strlen((char*)p) + 1;

	strcpy((char*)p, master);
	p += strlen((char*)p) + 1;

	p[0] = (size & 0xff00000000000000) >> 56;
	p[1] = (size & 0x00ff000000000000) >> 48;
	p[2] = (size & 0x0000ff0000000000) >> 40;
	p[3] = (size & 0x000000ff00000000) >> 32;
	p[4] = (size & 0x00000000ff000000) >> 24;
	p[5] = (size & 0x0000000000ff0000) >> 16;
	p[6] = (size & 0x000000000000ff00) >>  8;
	p[7] = (size & 0x00000000000000ff) >>  0;
	p += 8;

	send_finish(p);
}

static void send_ack_new_file(const char *image, const char *master, uint64_t size)
{
	uint8_t *p = send_begin(ACK_NEW_FILE);

	strcpy((char*)p, image);
	p += strlen((char*)p) + 1;

	strcpy((char*)p, master);
	p += strlen((char*)p) + 1;

	p[0] = (size & 0xff00000000000000) >> 56;
	p[1] = (size & 0x00ff000000000000) >> 48;
	p[2] = (size & 0x0000ff0000000000) >> 40;
	p[3] = (size & 0x000000ff00000000) >> 32;
	p[4] = (size & 0x00000000ff000000) >> 24;
	p[5] = (size & 0x0000000000ff0000) >> 16;
	p[6] = (size & 0x000000000000ff00) >>  8;
	p[7] = (size & 0x00000000000000ff) >>  0;
	p += 8;

	send_finish(p);
}

static void send_nack_new_file(const char *image, const char *master, uint64_t size)
{
	uint8_t *p = send_begin(NACK_NEW_FILE);

	strcpy((char*)p, image);
	p += strlen((char*)p) + 1;

	strcpy((char*)p, master);
	p += strlen((char*)p) + 1;

	p[0] = (size & 0xff00000000000000) >> 56;
	p[1] = (size & 0x00ff000000000000) >> 48;
	p[2] = (size & 0x0000ff0000000000) >> 40;
	p[3] = (size & 0x000000ff00000000) >> 32;
	p[4] = (size & 0x00000000ff000000) >> 24;
	p[5] = (size & 0x0000000000ff0000) >> 16;
	p[6] = (size & 0x000000000000ff00) >>  8;
	p[7] = (size & 0x00000000000000ff) >>  0;
	p += 8;

	send_finish(p);
}

static void send_req_block_data(const char *image, uint64_t offset, uint8_t *bitmap, uint16_t bitmap_len)
{
	uint8_t *p = send_begin(REQ_BLOCK_DATA);

	strcpy((char*)p, image);
	p += strlen((char*)p) + 1;

	p[0] = (offset & 0xff00000000000000) >> 56;
	p[1] = (offset & 0x00ff000000000000) >> 48;
	p[2] = (offset & 0x0000ff0000000000) >> 40;
	p[3] = (offset & 0x000000ff00000000) >> 32;
	p[4] = (offset & 0x00000000ff000000) >> 24;
	p[5] = (offset & 0x0000000000ff0000) >> 16;
	p[6] = (offset & 0x000000000000ff00) >>  8;
	p[7] = (offset & 0x00000000000000ff) >>  0;
	p += 8;

	for (int i=0; i<bitmap_len; i++)
		p[i] = bitmap[i];
	p += bitmap_len;

	send_finish(p);
}

static void send_block_data(const char *image, uint64_t offset, uint8_t *data, uint16_t data_len)
{
	uint8_t *p = send_begin(BLOCK_DATA);

	strcpy((char*)p, image);
	p += strlen((char*)p) + 1;

	p[0] = (offset & 0xff00000000000000) >> 56;
	p[1] = (offset & 0x00ff000000000000) >> 48;
	p[2] = (offset & 0x0000ff0000000000) >> 40;
	p[3] = (offset & 0x000000ff00000000) >> 32;
	p[4] = (offset & 0x00000000ff000000) >> 24;
	p[5] = (offset & 0x0000000000ff0000) >> 16;
	p[6] = (offset & 0x000000000000ff00) >>  8;
	p[7] = (offset & 0x00000000000000ff) >>  0;
	p += 8;

	for (int i=0; i<data_len; i++)
		p[i] = data[i];
	p += data_len;

	send_finish(p);
}

static void send_master_ref(const char *image, const char *master, uint64_t offset, uint8_t *bitmap, uint16_t bitmap_len)
{
	uint8_t *p = send_begin(MASTER_REF);

	strcpy((char*)p, image);
	p += strlen((char*)p) + 1;

	strcpy((char*)p, master);
	p += strlen((char*)p) + 1;

	p[0] = (offset & 0xff00000000000000) >> 56;
	p[1] = (offset & 0x00ff000000000000) >> 48;
	p[2] = (offset & 0x0000ff0000000000) >> 40;
	p[3] = (offset & 0x000000ff00000000) >> 32;
	p[4] = (offset & 0x00000000ff000000) >> 24;
	p[5] = (offset & 0x0000000000ff0000) >> 16;
	p[6] = (offset & 0x000000000000ff00) >>  8;
	p[7] = (offset & 0x00000000000000ff) >>  0;
	p += 8;

	for (int i=0; i<bitmap_len; i++)
		p[i] = bitmap[i];
	p += bitmap_len;

	send_finish(p);
}

static void send_req_manifest(const char *image)
{
	uint8_t *p = send_begin(REQ_MANIFEST);

	strcpy((char*)p, image);
	p += strlen((char*)p) + 1;

	send_finish(p);
}

static void send_manifest(const char *image, const char *master, uint64_t size)
{
	uint8_t *p = send_begin(MANIFEST);

	strcpy((char*)p, image);
	p += strlen((char*)p) + 1;

	strcpy((char*)p, master);
	p += strlen((char*)p) + 1;

	p[0] = (size & 0xff00000000000000) >> 56;
	p[1] = (size & 0x00ff000000000000) >> 48;
	p[2] = (size & 0x0000ff0000000000) >> 40;
	p[3] = (size & 0x000000ff00000000) >> 32;
	p[4] = (size & 0x00000000ff000000) >> 24;
	p[5] = (size & 0x0000000000ff0000) >> 16;
	p[6] = (size & 0x000000000000ff00) >>  8;
	p[7] = (size & 0x00000000000000ff) >>  0;
	p += 8;

	send_finish(p);
}


/*********************************
 * High-level Server Functionality
 ********************************/

struct server_image_desc_s {
	struct server_image_desc_s *next;
	char *image, *master;
	uint64_t size;
	uint8_t *todo_blocks_bitmap;
	uint8_t *master_blocks_bitmap;
	int todo_manifest;
	int fd;
};

static int todo_heartbeat;
static struct server_image_desc_s *server_image_desc_list = NULL;

static struct server_image_desc_s *server_get_fdesc(const char *image)
{
	struct server_image_desc_s *s = server_image_desc_list;
	while (s) {
		if (!strcmp(image, s->image))
			return s;
		s = s->next;
	}
	s = calloc(1, sizeof(struct server_image_desc_s));
	s->image = strdup(image);
	/* FIXME: Load image data */
	s->next = server_image_desc_list;
	server_image_desc_list = s;
}

static pthread_t server_listen_th;
static pthread_mutex_t big_lock;
static pthread_cond_t big_cond;

static void *server_main_listen(void *arg __attribute__((unused)))
{
	while (1) {
		if (!recv_any()) {
			pthread_mutex_lock(&big_lock);
			todo_heartbeat = 1;
			pthread_cond_signal(&big_cond);
			pthread_mutex_unlock(&big_lock);
		} else {
			pthread_mutex_lock(&big_lock);
			/* FIXME: Schedule task(s) */
			pthread_cond_signal(&big_cond);
			pthread_mutex_unlock(&big_lock);
		}
	}
	return NULL;
}

static int server_main()
{
	udp_init();
	pthread_create(&server_listen_th, NULL, server_main_listen, NULL);

	pthread_mutex_lock(&big_lock);
	while (1) {
		int found_jobs_to_do = 0;
		if (todo_heartbeat) {
			fprintf(stderr, "Sending heartbeat pkg.\n");
			struct utsname utsbuf;
			uname(&utsbuf);
			send_heartbeat(utsbuf.nodename);
			found_jobs_to_do = 1;
			todo_heartbeat = 0;
		}
		struct server_image_desc_s *s = server_image_desc_list;
		while (s) {
			/* FIXME: Do scheduled tasks */
			s = s->next;
		}
		if (!found_jobs_to_do)
			pthread_cond_wait(&big_cond, &big_lock);
	}

	return 0;
}


/************************
 * Command-line interface
 ***********************/

static void help()
{
	fprintf(stderr, "\n");
	fprintf(stderr, "  bclone [-P port] -s\n");
	fprintf(stderr, "    Starts an bclone server in read-only mode. All *.img files in the\n");
	fprintf(stderr, "    current working directory are exported.\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "  bclone [-P port] -S\n");
	fprintf(stderr, "    Starts an bclone server in read-write mode. All *.img files in the\n");
	fprintf(stderr, "    current working directory are exported. New images are stored as *.new\n");
	fprintf(stderr, "    files in the current working directory.\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "  bclone [-P port] -c filename imagename [ master-imagename ]\n");
	fprintf(stderr, "    Upload a new image to the server.\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "  bclone [-P port] -g filename imagename\n");
	fprintf(stderr, "    Fetch an image from the server.\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "  bclone -q image-filename\n");
	fprintf(stderr, "    Print information about an image file.\n");
	fprintf(stderr, "\n");
	exit(1);
}

int main(int argc, char **argv)
{
	int opt, mode = 0;
	while ((opt = getopt(argc, argv, "P:sScgq")) != -1) {
		switch (opt)
		{
		case 'P':
			port = atoi(optarg);
			break;
		case 's':
		case 'S':
			server_rw_mode = opt == 'S';
			return server_main();
		case 'c':
		case 'g':
		case 'q':
			if (mode)
				help();
			mode = opt;
			break;
		default:
			help();
		}
	}

	return 1;
}

