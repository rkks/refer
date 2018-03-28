

/*
 * Copyright 2001, 2002, 2003 Sun Microsystems, Inc. All Rights Reserved.
 * Except for any files in PNG format (which are marked with the filename
 * extension ".png"), GIF format (which are marked with the filename
 * extension ".gif"), or JPEG format (which are marked with the filename
 * extension ".jpg"), redistribution and use in source and binary forms,
 * with or without modification, are permitted provided that the
 * following conditions are met:
 * - Redistribution of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * - Redistribution in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * You may compile, use, perform and display the following files with
 * original Java Smart Ticket Sample Application code obtained from Sun
 * Microsystems, Inc. only:
 * - files in PNG format and having the ".png" extension
 * - files in GIF format and having the ".gif" extension
 * - files in JPEG format and having the ".jpg" extension
 * You may not modify or redistribute .png, .gif, or .jpg files in any
 * form, in whole or in part, by any means without prior written
 * authorization from Sun Microsystems, Inc. and its licensors, if any.
 * Neither the name of Sun Microsystems, Inc., the 'Java Smart Ticket
 * Sample Application', 'Java', 'Java'-based names, or the names of
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * This software is provided "AS IS," without a warranty of any kind. ALL
 * EXPRESS OR IMPLIED CONDITIONS, REPRESENTATIONS AND WARRANTIES,
 * INCLUDING ANY IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE OR NON-INFRINGEMENT, ARE HEREBY EXCLUDED. SUN
 * MIDROSYSTEMS, INC. ("SUN") AND ITS LICENSORS SHALL NOT BE LIABLE FOR
 * ANY DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING OR
 * DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES. IN NO EVENT WILL SUN OR
 * ITS LICENSORS BE LIABLE FOR ANY LOST REVENUE, PROFIT OR DATA, OR FOR
 * DIRECT, INDIRECT, SPECIAL, CONSEQUENTIAL, INCIDENTAL OR PUNITIVE
 * DAMAGES, HOWEVER CAUSED AND REGARDLESS OF THE THEORY OF LIABILITY,
 * ARISING OUT OF THE USE OF OR INABILITY TO USE THIS SOFTWARE, EVEN IF
 * SUN HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * You acknowledge that this software is not designed, licensed or
 * intended for use in the design, construction, operation or maintenance
 * of any nuclear facility.
 * $Id: Seating.java,v 1.3 2003/03/20 01:46:53 zull Exp $
 */
package com.sun.j2me.blueprints.smartticket.server.ejb.show;

import java.io.*;

/**
 * Represents a seating plan for a show. The seating plan is rectangular, with array-style indices.
 * That is, rows are indexed from 0 to row-1 inclusive, where row is the number of rows in the seating plan, and seats are
 * indexed from 0 to seat-1 inclusive, where seat is the number of seats in the seating plan.
 */
public class Seating implements Serializable {
    public static final byte AISLE = 9;
    public static final byte AVAILABLE = 0;
    public static final byte UNAVAILABLE = 1;
    public static final byte BOOKED = 2;
    private byte[] seats;
    private int rowCount;
    private int rowLength;

    public Seating() {}

    public Seating(int rowCount, int rowLength) {
        this(new byte[rowCount * rowLength], rowCount, rowLength);

        return;
    }

    public Seating(byte[] seats, int rowCount, int rowLength) {
        if (rowCount * rowLength != seats.length) {
            throw new IllegalArgumentException("rowCount * rowLength != length");
        } 

        this.rowCount = rowCount;
        this.rowLength = rowLength;
        this.seats = seats;

        return;
    }

    public Seating(Seating seating) {
        rowCount = seating.rowCount;
        rowLength = seating.rowLength;
        seats = new byte[seating.seats.length];

        System.arraycopy(seating.seats, 0, seats, 0, seating.seats.length);

        return;
    }

    public int getRowCount() {
        return rowCount;
    } 

    public int getRowLength() {
        return rowLength;
    } 

    public byte[] getSeatData() {
        return seats;
    } 

    public boolean isAisle(int row, int seat) {
        return getStatusOf(row, seat) == AISLE;
    } 

    public boolean isAvailable(int row, int seat) {
        return getStatusOf(row, seat) == AVAILABLE;
    } 

    public boolean isBooked(int row, int seat) {
        return getStatusOf(row, seat) == BOOKED;
    } 

    public boolean isUnavailable(int row, int seat) {
        return getStatusOf(row, seat) == UNAVAILABLE;
    } 

    public byte getStatusOf(int row, int seat) {
        if (seat > rowLength) {
            throw new IllegalArgumentException("Seat > rowLength");
        } 

        return seats[seatIndex(row, seat)];
    } 

    public void setStatusOf(int row, int seat, byte status) {
        if (row > rowCount || seat > rowLength) {
            throw new IllegalArgumentException("row or seat too large");
        } 

        seats[seatIndex(row, seat)] = status;
    } 

    private int seatIndex(int row, int seat) {
        if (row > rowCount || seat > rowLength) {
            throw new IllegalArgumentException("row or seat too large");
        } 

        return row * rowLength + seat;
    } 

    public void setAvailable(int row, int seat) {
        seats[seatIndex(row, seat)] = AVAILABLE;
    } 

    public void setBooked(int row, int seat) {
        seats[seatIndex(row, seat)] = BOOKED;
    } 

    public void setUnavailable(int row, int seat) {
        seats[seatIndex(row, seat)] = UNAVAILABLE;
    } 

    public boolean hasBooking(int row) {
        int i = row * rowLength;
        int end = i + rowLength;

        for (; i < end; i++) {
            if (seats[i] == BOOKED) {
                return true;
            } 
        } 

        return false;
    } 

    public void setRow(int row, String seats) {
        for (int seat = 0; seat < seats.length(); seat++) {
            setStatusOf(row, seat, 
                        (byte) Byte.parseByte(new Character(seats.charAt(seat)).toString()));
        } 
    } 

    public String getRow(int row) {
        StringBuffer sb = new StringBuffer();

        for (int seat = 0; seat < getRowLength(); seat++) {
            sb.append(String.valueOf(new Byte(getStatusOf(row, 
                    seat)).intValue()).charAt(0));
        } 

        return sb.toString();
    } 

    public boolean bookSeats(Seat[] seats) {
        for (int i = 0; i < seats.length; i++) {
            if (!isAvailable(seats[i].getRowNb(), seats[i].getSeatNb())) {
                return false;
            } 
        } 

        for (int i = 0; i < seats.length; i++) {
            setBooked(seats[i].getRowNb(), seats[i].getSeatNb());
        } 

        return true;
    } 

    public boolean unbookSeats(Seat[] seats) {
        for (int i = 0; i < seats.length; i++) {
            if (isBooked(seats[i].getRowNb(), seats[i].getSeatNb())) {
                setAvailable(seats[i].getRowNb(), seats[i].getSeatNb());
            } 
        } 

        return true;
    } 

    public boolean confirmBookedSeats(Seat[] seats) {
        for (int i = 0; i < seats.length; i++) {
            if (isBooked(seats[i].getRowNb(), seats[i].getSeatNb())) {
                setUnavailable(seats[i].getRowNb(), seats[i].getSeatNb());
            } 
        } 

        return true;
    } 

}

