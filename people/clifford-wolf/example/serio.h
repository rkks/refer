/*
 *     !!!!!!!!!!!!! WARNING: This is work in progress !!!!!!!!!!!!!
 *
 *  serio.h  -  A simple header-only library for serial port I/O
 *
 *  Copyright (C) 2009  Clifford Wolf <clifford@clifford.at>
 *  Copyright (C) 2009  Stefan Farthofer <s@w23.at> (win32 support)
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    1. Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *    2. Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in
 *       the documentation and/or other materials provided with the
 *       distribution.
 *    3. The names of the authors may not be used to endorse or promote
 *       products derived from this software without specific prior
 *       written permission.
 *
 *  THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 *  IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 *  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#ifndef SERIO_H
#define SERIO_H

#ifdef  __cplusplus
extern "C" {
#endif

#include <sys/types.h>
#include <string.h>

enum {
	/* read/write modes */
	SERIO_ALL = 100,
	SERIO_SOME,
	SERIO_ANY,
	/* rw line attributes */
	SERIO_BAUDRATE = 200,
	SERIO_RTS,
	SERIO_DTR,
	/* ro line attributes */
	SERIO_CTS = 300,
	SERIO_DSR,
	SERIO_DCD,
	/* flush modes */
	SERIO_IN = 400,
	SERIO_OUT
};

struct serio_fd;
typedef struct serio_fd serio_fd;

static inline char **serio_list();
static inline void serio_list_free(char **list);
static inline int serio_open(const char *name, serio_fd *fd);
static inline void serio_close(serio_fd *fd);
static inline size_t serio_write(serio_fd *fd, const void *data, size_t len, int mode);
static inline size_t serio_read(serio_fd *fd, void *data, size_t len, int mode);
static inline int serio_set(serio_fd *fd, int what, int value);
static inline int serio_get(serio_fd *fd, int what);
static inline int serio_break(serio_fd *fd);
static inline int serio_flush(serio_fd *fd, int mode);

#ifdef WIN32

//timeout to wait for reading or writing when using SERIO_SOME or SERIO_ANY when the OS does not support returing immediately
#ifndef SERIO_TIMEOUT_MS
#define SERIO_TIMEOUT_MS 5
#endif

#include <windows.h>
#include <malloc.h>

/*
 * Win32 implementation
 */


struct serio_fd {
	HANDLE handle;
};

static inline char **serio_list()
{
	HKEY serialcommKey;
	DWORD portCount, maxNameLength, maxValueLength, nameLength, valueLength, valueType;
	char *name;
	char **list;
	
	if (RegOpenKeyA(HKEY_LOCAL_MACHINE, "HARDWARE\\DEVICEMAP\\SERIALCOMM", &serialcommKey) != ERROR_SUCCESS)
		return NULL;
	
	if (RegQueryInfoKeyA(serialcommKey, NULL, NULL, NULL, NULL, NULL, NULL, &portCount, &maxNameLength, &maxValueLength, NULL, NULL) != ERROR_SUCCESS) {
		RegCloseKey(serialcommKey);
		return NULL;
	}

	name = (char*)_alloca(maxNameLength+1);

	list = (char**)malloc(sizeof(char*) * (portCount+1));
	for (int i=0; i < portCount; i++) {
		nameLength = maxNameLength;
		valueLength = maxValueLength;
		list[i] = (char*)malloc(maxValueLength+1);
		if (RegEnumValueA(serialcommKey, i, name, &nameLength, NULL, &valueType, (LPBYTE)list[i], &valueLength) != ERROR_SUCCESS || valueType != REG_SZ) {
			for(;i >= 0; i--) free(list[i]);
			free(list);
			RegCloseKey(serialcommKey);
			return NULL;
		}
	}
	list[portCount] = NULL;

	RegCloseKey(serialcommKey);
	
	return list;
}

static inline void serio_list_free(char **list)
{
	if (list == NULL) return;
	for(int i=0; list[i]; i++) free(list[i]);
	free(list);
}

static inline int serio_open(const char *name, serio_fd *fd)
{
	DCB serialParameters;
	serialParameters.DCBlength = sizeof(DCB);

	fd->handle = CreateFileA(name, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
	if (fd->handle == INVALID_HANDLE_VALUE) return -1;

	if (!GetCommState(fd->handle, &serialParameters)) {
		CloseHandle(fd->handle);
		return -1;
	}

	serialParameters.BaudRate = CBR_19200;
	serialParameters.ByteSize = 8;
	serialParameters.StopBits = ONESTOPBIT;
	serialParameters.Parity = NOPARITY;
	if (!SetCommState(fd->handle, &serialParameters)) {
		CloseHandle(fd->handle);
		return -1;
	}

	return 0;
}

static inline void serio_close(serio_fd *fd)
{
	CloseHandle(fd->handle);
}

static inline size_t serio_write(serio_fd *fd, const void *data, size_t len, int mode)
{
	DWORD written=0, writtenTotal=0;
	size_t minWrite;
	COMMTIMEOUTS commTimeouts;
	commTimeouts.ReadIntervalTimeout = MAXDWORD;
	commTimeouts.ReadTotalTimeoutConstant = 0;
	commTimeouts.ReadTotalTimeoutMultiplier = 0;
	commTimeouts.WriteTotalTimeoutMultiplier = 0;

	switch(mode) {
		case SERIO_ALL:
			commTimeouts.WriteTotalTimeoutConstant = 0;
			minWrite = len;
			break;

		case SERIO_SOME:
			commTimeouts.WriteTotalTimeoutConstant = SERIO_TIMEOUT_MS;
			minWrite = 1;
			break;

		case SERIO_ANY:
			commTimeouts.WriteTotalTimeoutConstant = SERIO_TIMEOUT_MS;
			minWrite = 0;
			break;

		default:
			return -1;
	}

	if (!SetCommTimeouts(fd->handle, &commTimeouts))
		return -1;

	do {
		if (!WriteFile(fd->handle, (char*)data + writtenTotal, len - writtenTotal, &written, NULL))
			return -1;

		writtenTotal += written;
	} while (writtenTotal < minWrite);

	return writtenTotal;
}

static inline size_t serio_read(serio_fd *fd, void *data, size_t len, int mode)
{
	DWORD read = 0, readTotal = 0;
	size_t minRead;
	COMMTIMEOUTS commTimeouts;
	commTimeouts.WriteTotalTimeoutConstant = 0;
	commTimeouts.WriteTotalTimeoutMultiplier = 0;
	commTimeouts.ReadTotalTimeoutMultiplier = 0;

	switch(mode) {
		case SERIO_ALL:
			commTimeouts.ReadIntervalTimeout = commTimeouts.ReadTotalTimeoutConstant = 0;
			minRead = len;
			break;

		case SERIO_SOME:
			commTimeouts.ReadIntervalTimeout = 0;
			commTimeouts.ReadTotalTimeoutConstant = SERIO_TIMEOUT_MS;
			minRead = 1;
			break;

		case SERIO_ANY:
			commTimeouts.ReadIntervalTimeout = MAXDWORD;
			commTimeouts.ReadTotalTimeoutConstant = 0;
			minRead = 0;
			break;

		default:
			return -1;
	}

	if (!SetCommTimeouts(fd->handle, &commTimeouts)) 
		return -1;

	do {
		if (!ReadFile(fd->handle, (char*)data + readTotal, len - readTotal, &read, NULL))
			return -1;

		readTotal += read;
	} while (readTotal < minRead);

	return readTotal;
}

static inline int serio_set(serio_fd *fd, int what, int value)
{
	DCB serialParameters;
	serialParameters.DCBlength = sizeof(DCB);
	if (!GetCommState(fd->handle, &serialParameters)) return -1;

	switch (what) {
		case SERIO_BAUDRATE:
			serialParameters.BaudRate = value;
			break;

		case SERIO_RTS:
			serialParameters.fRtsControl = value ? RTS_CONTROL_ENABLE : RTS_CONTROL_DISABLE;
			break;

		case SERIO_DTR:
			serialParameters.fDtrControl = value ? DTR_CONTROL_ENABLE : DTR_CONTROL_DISABLE;
			break;

		default:
			return -1;
	}

	return SetCommState(fd->handle, &serialParameters) ? 0 : -1;
}

static inline int serio_get(serio_fd *fd, int what)
{
	DCB serialParameters;
	DWORD modemState;


	switch (what) {
		case SERIO_BAUDRATE:
			serialParameters.DCBlength = sizeof(DCB);
			if (!GetCommState(fd->handle, &serialParameters)) return -1;
			return serialParameters.BaudRate;

		case SERIO_RTS:
			serialParameters.DCBlength = sizeof(DCB);
			if (!GetCommState(fd->handle, &serialParameters)) return -1;
			return serialParameters.fRtsControl == RTS_CONTROL_ENABLE ? 1 : 0;

		case SERIO_DTR:
			serialParameters.DCBlength = sizeof(DCB);
			if (!GetCommState(fd->handle, &serialParameters)) return -1;
			return serialParameters.fDtrControl == DTR_CONTROL_ENABLE ? 1 : 0;

		case SERIO_CTS:
			if (!GetCommModemStatus(fd->handle, &modemState)) return -1;
			return (modemState & MS_CTS_ON) != 0 ? 1 : 0;

		case SERIO_DSR:
			if (!GetCommModemStatus(fd->handle, &modemState)) return -1;
			return (modemState & MS_DSR_ON) != 0 ? 1 : 0;

		case SERIO_DCD:
			if (!GetCommModemStatus(fd->handle, &modemState)) return -1;
			return (modemState & MS_RLSD_ON) != 0 ? 1 : 0;

		default: 
			return -1;
	}
}

static inline int serio_break(serio_fd *fd)
{
	if (!SetCommBreak(fd))
		return -1;
	Sleep(500);
	if (!ClearCommBreak(fd))
		return -1;
	return 0;
}

static inline int serio_flush(serio_fd *fd, int mode)
{
	switch(mode) {
		case SERIO_IN:
			return PurgeComm(fd->handle, PURGE_RXCLEAR) ? 0 : -1;

		case SERIO_OUT:
			return FlushFileBuffers(fd->handle) ? 0 : -1;

		default:
			return -1;
	}
}

#else

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/*
 * UNIX/Linux implementation
 */

struct serio_fd {
	int fd;
};

static inline char **serio_list()
{
	struct dirent **namelist;
	char **list;
	int n, i, j;

	n = scandir("/dev", &namelist, 0, alphasort);

	if (n < 0)
		return NULL;

	for (i = j = 0; i < n; i++) {
		if (strncmp(namelist[i]->d_name, "ttyS", 4) && strncmp(namelist[i]->d_name, "ttyUSB", 6))
			continue;
		j++;
	}
	list = (char**)malloc(sizeof(char*)*j + 1);

	for (i = j = 0; i < n; i++) {
		if (strncmp(namelist[i]->d_name, "ttyS", 4) && strncmp(namelist[i]->d_name, "ttyUSB", 6))
			continue;
		char buffer[100];
		snprintf(buffer, 100, "/dev/%s", namelist[i]->d_name);
		list[j++] = strdup(buffer);
	}
	list[j] = NULL;
	
	while (n--)
		free(namelist[n]);
	free(namelist);

	return list;
}

static inline void serio_list_free(char **list)
{
	int i;
	for (i = 0; list[i]; i++)
		free(list[i]);
	free(list);
}

static inline int serio_open(const char *name, serio_fd *fd)
{
	fd->fd = open(name, O_RDWR);
	return fd->fd >= 0 ? 0 : -1;
}

static inline void serio_close(serio_fd *fd)
{
	close(fd->fd);
}

static inline size_t serio_write(serio_fd *fd, const void *data, size_t len, int mode)
{
	if (mode == SERIO_ALL) {
		int i = 0, rc;
		fcntl(fd->fd, F_SETFL, O_RDWR);
		while (i < (int)len) {
			rc = write(fd->fd, (const char*)data+i, len-i);
			if (rc <= 0)
				return -1;
			i += rc;
		}
		return i;
	}
	if (mode == SERIO_SOME) {
		fcntl(fd->fd, F_SETFL, O_RDWR);
		return write(fd->fd, data, len);
	}
	if (mode == SERIO_ANY) {
		fcntl(fd->fd, F_SETFL, O_RDWR|O_NONBLOCK);
		return write(fd->fd, data, len);
	}
	return -1;
}

static inline size_t serio_read(serio_fd *fd, void *data, size_t len, int mode)
{
	if (mode == SERIO_ALL) {
		int i = 0, rc;
		fcntl(fd->fd, F_SETFL, O_RDWR);
		while (i < (int)len) {
			rc = read(fd->fd, (char*)data+i, len-i);
			if (rc <= 0)
				return -1;
			i += rc;
		}
		return i;
	}
	if (mode == SERIO_SOME) {
		fcntl(fd->fd, F_SETFL, O_RDWR);
		return read(fd->fd, data, len);
	}
	if (mode == SERIO_ANY) {
		fcntl(fd->fd, F_SETFL, O_RDWR|O_NONBLOCK);
		return read(fd->fd, data, len);
	}
	return -1;
}

static inline int serio_set(serio_fd *fd, int what, int value)
{
	if (what == SERIO_BAUDRATE)
	{
		struct termios tio;
		memset(&tio, 0, sizeof(tio));
		tcgetattr(fd->fd, &tio);
		switch (value)
		{
		case 0:
			cfsetospeed(&tio, B0);
			break;
		case 50:
			cfsetospeed(&tio, B50);
			break;
		case 75:
			cfsetospeed(&tio, B75);
			break;
		case 110:
			cfsetospeed(&tio, B110);
			break;
		case 134:
			cfsetospeed(&tio, B134);
			break;
		case 150:
			cfsetospeed(&tio, B150);
			break;
		case 200:
			cfsetospeed(&tio, B200);
			break;
		case 300:
			cfsetospeed(&tio, B300);
			break;
		case 600:
			cfsetospeed(&tio, B600);
			break;
		case 1200:
			cfsetospeed(&tio, B1200);
			break;
		case 1800:
			cfsetospeed(&tio, B1800);
			break;
		case 2400:
			cfsetospeed(&tio, B2400);
			break;
		case 4800:
			cfsetospeed(&tio, B4800);
			break;
		case 9600:
			cfsetospeed(&tio, B9600);
			break;
		case 19200:
			cfsetospeed(&tio, B19200);
			break;
		case 38400:
			cfsetospeed(&tio, B38400);
			break;
		case 57600:
			cfsetospeed(&tio, B57600);
			break;
		case 115200:
			cfsetospeed(&tio, B115200);
			break;
		case 230400:
			cfsetospeed(&tio, B230400);
			break;
		default:
			return -1;
		}
		return tcsetattr(fd->fd, TCSANOW, &tio);
	}
	if (what == SERIO_RTS)
	{
		int arg = TIOCM_RTS;
		return ioctl(fd->fd, value ? TIOCMBIS : TIOCMBIC, &arg);
	}
	if (what == SERIO_DTR)
	{
		int arg = TIOCM_DTR;
		return ioctl(fd->fd, value ? TIOCMBIS : TIOCMBIC, &arg);
	}
	return -1;
}

static inline int serio_get(serio_fd *fd, int what)
{
	if (what == SERIO_BAUDRATE)
	{
		struct termios tio;
		memset(&tio, 0, sizeof(tio));
		tcgetattr(fd->fd, &tio);
		switch (cfgetospeed(&tio))
		{
		case B0:
			return 0;
		case B50:
			return 50;
		case B75:
			return 75;
		case B110:
			return 110;
		case B134:
			return 134;
		case B150:
			return 150;
		case B200:
			return 200;
		case B300:
			return 300;
		case B600:
			return 600;
		case B1200:
			return 1200;
		case B1800:
			return 1800;
		case B2400:
			return 2400;
		case B4800:
			return 4800;
		case B9600:
			return 9600;
		case B19200:
			return 19200;
		case B38400:
			return 38400;
		case B57600:
			return 57600;
		case B115200:
			return 115200;
		case B230400:
			return 230400;
		}
		return -1;
	}
	if (what == SERIO_RTS)
	{
		int arg;
		if (ioctl(fd->fd, TIOCMGET, &arg) < 0)
			return -1;
		return (arg & TIOCM_RTS) != 0;
	}
	if (what == SERIO_DTR)
	{
		int arg;
		if (ioctl(fd->fd, TIOCMGET, &arg) < 0)
			return -1;
		return (arg & TIOCM_DTR) != 0;
	}
	if (what == SERIO_CTS)
	{
		int arg;
		if (ioctl(fd->fd, TIOCMGET, &arg) < 0)
			return -1;
		return (arg & TIOCM_CTS) != 0;
	}
	if (what == SERIO_DSR)
	{
		int arg;
		if (ioctl(fd->fd, TIOCMGET, &arg) < 0)
			return -1;
		return (arg & TIOCM_DSR) != 0;
	}
	if (what == SERIO_DCD)
	{
		int arg;
		if (ioctl(fd->fd, TIOCMGET, &arg) < 0)
			return -1;
		return (arg & TIOCM_CAR) != 0;
	}
	return -1;
}

static inline int serio_break(serio_fd *fd)
{
	return tcsendbreak(fd->fd, 0);
}

static inline int serio_flush(serio_fd *fd, int mode)
{
	if (mode == SERIO_IN) {
		return tcflush(fd->fd, TCIFLUSH);
	}
	if (mode == SERIO_OUT) {
		return tcdrain(fd->fd);
	}
	return -1;
}

#endif

#ifdef __cplusplus
}
#endif

#endif

