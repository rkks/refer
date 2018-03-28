// utf8purify.c
//
// Copyright (C) 2007 by Clifford Wolf <www.clifford.at>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// Read a wild mix of utf8 and latin1 from input
// Write pure utf8 to output
//
// A byte walks into a bar and orders a pint. Bartender asks him "What's
// wrong?" Byte says "Parity error." Bartender nods and says "Yeah, I
// thought you looked a bit off."

#include <unistd.h>

// lookahead buffer
int labuffer[3];

// read a byte from inpout and shift thru the lookahead buffer
int readbyte()
{
	int retval = labuffer[0];
	labuffer[0] = labuffer[1];
	labuffer[1] = labuffer[2];
	labuffer[2] = -1;

	unsigned char buffer;
	if (read(0, &buffer, 1) == 1)
		labuffer[2] = buffer;

	return retval;
}

// write a byte to the output
void writebyte(int ch)
{
	unsigned char buffer = (unsigned)ch;
	write(1, &buffer, 1);
}

int main()
{
	// fill lookahead buffer
	readbyte();
	readbyte();
	readbyte();

	// main loop
	while (1)
	{
		// read the first byte (rest is in the lookahead buffer)
		int ch = readbyte();

		// we have reached the end of the input
		if (ch < 0)
			break;

		// just normal 7-bit ascii
		if ((ch & 0x80) == 0) {
			writebyte(ch);
			continue;
		}

		// 2-byte utf-8 sequence (starting with '110')
		if ((ch & 0xE0) == 0xC0 && (labuffer[0] & 0xC0) == 0x80) {
			writebyte(ch);
			writebyte(readbyte());
			continue;
		}

		// 3-byte utf-8 sequence (starting with '1110')
		if ((ch & 0xF0) == 0xE0 && (labuffer[0] & 0xC0) == 0x80 &&
				(labuffer[1] & 0xC0) == 0x80) {
			writebyte(ch);
			writebyte(readbyte());
			writebyte(readbyte());
			continue;
		}

		// 4-byte utf-8 sequence (starting with '11110')
		if ((ch & 0xF8) == 0xF0 && (labuffer[0] & 0xC0) == 0x80 &&
				(labuffer[1] & 0xC0) == 0x80 &&
				(labuffer[2] & 0xC0) == 0x80) {
			writebyte(ch);
			writebyte(readbyte());
			writebyte(readbyte());
			writebyte(readbyte());
			continue;
		}

		// so it seams to be a latin1 character
		writebyte(0xC0 | (ch >> 6));
		writebyte(0x80 | (ch & 0x3f));
	}

	return 0;
}

