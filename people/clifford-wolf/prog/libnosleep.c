/*
 *  LibNoSleep - An LD_PRELOAD library to ignore simple sleeps
 *
 *  Copyright (C) 2008  Clifford Wolf <clifford@clifford.at>
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
 *  Usage Example:
 *  gcc -o libnosleep.so -shared libnosleep.c
 *  LD_PRELOAD=$PWD/libnosleep.so sleep 100
 */

#include <pth.h>

int pth_nanosleep(const struct timespec *x, struct timespec *y) {
	return 0;
}

int pth_usleep(unsigned int x) {
	return 0;
}

unsigned int pth_sleep(unsigned int x) {
	return 0;
}

#include <time.h>

int nanosleep (__const struct timespec *__requested_time, struct timespec *__remaining) {
	return 0;
}

#include <unistd.h>

int usleep (__useconds_t __useconds) {
	return 0;
}

