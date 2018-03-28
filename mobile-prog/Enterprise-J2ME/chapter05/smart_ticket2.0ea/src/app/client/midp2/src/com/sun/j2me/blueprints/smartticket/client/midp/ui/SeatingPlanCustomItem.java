

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
 * $Id: SeatingPlanCustomItem.java,v 1.1 2003/04/05 00:41:51 zull Exp $
 */
package com.sun.j2me.blueprints.smartticket.client.midp.ui;

import javax.microedition.lcdui.*;
import java.util.*;
import com.sun.j2me.blueprints.smartticket.shared.midp.model.TheaterSchedule;
import com.sun.j2me.blueprints.smartticket.shared.midp.model.SeatingPlan;
import com.sun.j2me.blueprints.smartticket.shared.midp.ApplicationException;

public class SeatingPlanCustomItem extends CustomItem {
    // Often-used colors.
    private static final int WHITE = 0xffffff;
    private static final int BLACK = 0x000000;
    private static final int RED = 0xff6d00;
    private static final int GREEN = 0x006d55;
    private static final int BLUE = 0x0000ff;
    private static final int BURGUNDY = 0xb60055;
    private int currentRowNumber;
    private int currentSeatNumber;
    // Off-screen buffer.
    private Image buffer = null;
    // Canvas and individual seat dimensions.
    private int width, height;
    private int seatH, seatW;
    private Graphics g;
    private int marginT, marginB;
    // Is the cursor on?
    private Timer timer;
    private boolean cursorOn;
    private UIController uiController;
    private SeatingPlan seatingPlan;

    /**
     * @link dependency
     */
    /* # TicketPurchaseUI lnkTicketPurchaseUI; */
    public SeatingPlanCustomItem(UIController uiController, String label) {
        super(label);    // no label

        this.uiController = uiController;
    }

    public void init(SeatingPlan seatingPlan) {
        this.seatingPlan = seatingPlan;
    } 

    public int getMinContentHeight() {
        return UIConstants.SEATING_PLAN_MIN_HEIGHT;
    } 

    public int getMinContentWidth() {
        return UIConstants.SEATING_PLAN_MIN_WIDTH;
    } 

    public int getPrefContentHeight(int h) {
        return UIConstants.SEATING_PLAN_PREF_HEIGHT;
    } 

    public int getPrefContentWidth(int w) {
        return UIConstants.SEATING_PLAN_PREF_WIDTH;
    } 

    public void paint(Graphics graphics, int width, int height) {
        if (buffer == null) {
            this.width = width;
            this.height = height;
            buffer = Image.createImage(width, height);

            Graphics g = buffer.getGraphics();
            Font f = g.getFont();

            // Paint the canvas background.
            g.setColor(WHITE);
            g.fillRect(0, 0, width, height);

            // Adjust left and top margins to center seating plan.
            marginT = f.getHeight() * 2 
                      + (height % seatingPlan.getRowCount()) / 2 - 1;
            marginB = 0;
            // Calculate seat width and height.
            seatW = width / seatingPlan.getRowLength();
            seatH = (height - (marginT + marginB)) 
                    / seatingPlan.getRowCount();

            // Position the cursor on the first empty seat.
            for (int r = 0; r != seatingPlan.getRowCount(); r++) {
                for (int s = 0; s != seatingPlan.getRowLength(); s++) {
                    if (seatingPlan.getStatusOf(r, s) 
                            == SeatingPlan.AVAILABLE) {
                        currentRowNumber = r;
                        currentSeatNumber = s;
                    } 
                } 
            } 

            // Draw the seating plan.
            for (int r = 0; r != seatingPlan.getRowCount(); r++) {
                for (int s = 0; s != seatingPlan.getRowLength(); s++) {
                    drawSeat(r, s);
                } 
            } 

            drawSeatID();

            String screen = " [SCREEN] ";
            int screenWidth = f.stringWidth(screen);

            g.setColor(BLACK);
            g.fillRect(width / 2 - screenWidth / 2, f.getHeight(), 
                       screenWidth, f.getHeight());
            g.setColor(WHITE);
            g.drawString(screen, width / 2 - screenWidth / 2, f.getHeight(), 
                         Graphics.TOP | Graphics.LEFT);
        } 

        graphics.drawImage(buffer, 0, 0, Graphics.TOP | Graphics.LEFT);
    } 

    /**
     * Draw individual seat with current status.
     */
    void drawSeat(int rowNumber, int seatNumber) {
        drawSeat(rowNumber, seatNumber, -1);
    } 

    /**
     * Draw individual seat with over-riding color.
     * @param  color the color to draw seat.
     */
    synchronized void drawSeat(int rowNumber, int seatNumber, int color) {
        int x = seatNumber * seatW;
        int y = marginT + rowNumber * seatH;

        // If no color was specified, pick an appropriate color.
        if (color == -1) {
            switch (seatingPlan.getStatusOf(rowNumber, seatNumber)) {

            case SeatingPlan.AVAILABLE:
                color = WHITE;

                break;

            case SeatingPlan.BOOKED:
                color = BLUE;

                break;

            case SeatingPlan.UNAVAILABLE:
                color = RED;

                break;

            default:
                return;
            }
        } 

        // Paint the seat and draw its border.
        if (buffer != null 
                && seatingPlan.getStatusOf(rowNumber, seatNumber) 
                   != SeatingPlan.AISLE) {
            Graphics g = buffer.getGraphics();

            g.setColor(color);
            g.fillRect(x, y, seatW, seatH);
            g.setColor(GREEN);
            g.drawRect(x, y, seatW, seatH);
            repaint(x, y, seatW, seatH);
        } 
    } 

    /**
     * Draw the position on the screen.
     */
    synchronized void drawSeatID() {
        if (buffer != null) {
            Graphics g = buffer.getGraphics();
            Font f = g.getFont();
            String pos = "R" + currentRowNumber + " S" + currentSeatNumber;

            g.setColor(WHITE);
            g.fillRect(0, 0, f.stringWidth(pos + "  "), f.getHeight());
            g.setColor(BLUE);
            g.drawString(pos, 0, 0, Graphics.TOP | Graphics.LEFT);
            repaint(0, 0, f.stringWidth(pos + "  "), f.getHeight());
        } 
    } 

    /**
     * Draw the cursor object.
     */
    void drawCursor() {
        if (cursorOn) {
            drawSeat(currentRowNumber, currentSeatNumber, BURGUNDY);
        } else {
            drawSeat(currentRowNumber, currentSeatNumber);
        } 
    } 

    /**
     * Book or unbook the given seat.
     */
    void toggleSeat(int rowNumber, int seatNumber) {
        if (seatingPlan.getStatusOf(rowNumber, seatNumber) 
                == SeatingPlan.BOOKED) {
            seatingPlan.setStatusOf(rowNumber, seatNumber, 
                                    SeatingPlan.AVAILABLE);
        } else if (seatingPlan.getStatusOf(rowNumber, seatNumber) 
                   == SeatingPlan.AVAILABLE) {
            seatingPlan.setStatusOf(rowNumber, seatNumber, 
                                    SeatingPlan.BOOKED);
        } else {
            return;
        } 
    } 

    protected void keyPressed(int keyCode) {
        int oldRowNumber = currentRowNumber;
        int oldSeatNumber = currentSeatNumber;
        int action = getGameAction(keyCode);

        if (action == Canvas.FIRE) {
            toggleSeat(currentRowNumber, currentSeatNumber);
        } 
    } 

    protected void traverseout() {}

    protected boolean traverse(int dir, int viewportW, int viewportH, 
                               int[] visRect_inout) {
        int oldRowNumber = currentRowNumber;
        int oldSeatNumber = currentSeatNumber;

        visRect_inout[0] = 0;
        visRect_inout[1] = 0;
        visRect_inout[2] = width;
        visRect_inout[3] = height;

        // assume we have both traverse_vertical & traverse_horizontal
        switch (dir) {

        case Canvas.LEFT:
            for (int s = (currentSeatNumber - 1); s >= 0; s--) {
                int status = seatingPlan.getStatusOf(currentRowNumber, s);

                if (status == SeatingPlan.BOOKED 
                        || status == SeatingPlan.AVAILABLE) {
                    currentSeatNumber = s;

                    break;
                } 
            } 

            break;

        case Canvas.RIGHT:
            for (int s = (currentSeatNumber + 1); 
                    s < seatingPlan.getRowLength(); s++) {
                int status = seatingPlan.getStatusOf(currentRowNumber, s);

                if (status == SeatingPlan.BOOKED 
                        || status == SeatingPlan.AVAILABLE) {
                    currentSeatNumber = s;

                    break;
                } 
            } 

            break;

        case Canvas.UP:
            if (currentRowNumber == 0) {
                return false;
            } 

            for (int r = (currentRowNumber - 1); r >= 0; r--) {
                int status = seatingPlan.getStatusOf(r, currentSeatNumber);

                if (status == SeatingPlan.BOOKED 
                        || status == SeatingPlan.AVAILABLE) {
                    currentRowNumber = r;

                    break;
                } 
            } 

            break;

        case Canvas.DOWN:
            if (currentRowNumber == seatingPlan.getRowCount()) {
                return false;
            } 

            for (int r = (currentRowNumber + 1); 
                    r < seatingPlan.getRowCount(); r++) {
                int status = seatingPlan.getStatusOf(r, currentSeatNumber);

                if (status == SeatingPlan.BOOKED 
                        || status == SeatingPlan.AVAILABLE) {
                    currentRowNumber = r;

                    break;
                } 
            } 

            break;

        default:
            return true;
        }

        // Draw new seat and re-draw old seat.
        drawCursor();
        drawSeat(oldRowNumber, oldSeatNumber);
        drawSeatID();

        return true;
    } 

    protected void pointerPressed(int x, int y) {
        // Get the clicked position based on x and y coordinates.
        int rowNumber = (y - marginT) / seatH;
        int seatNumber = x / seatW;

        if (rowNumber < 0 || rowNumber >= seatingPlan.getRowCount() 
                || seatNumber < 0 
                || seatNumber >= seatingPlan.getRowLength()) {
            return;
        } 

        int oldRowNumber = currentRowNumber;
        int oldSeatNumber = currentSeatNumber;
        int currentStatus = seatingPlan.getStatusOf(currentRowNumber, 
                                                    currentSeatNumber);

        if (currentStatus == SeatingPlan.AVAILABLE 
                || currentStatus == SeatingPlan.BOOKED) {
            currentRowNumber = rowNumber;
            currentSeatNumber = seatNumber;

            toggleSeat(currentRowNumber, currentSeatNumber);
            // Draw new seat and re-draw old seat.
            drawCursor();
            drawSeat(oldRowNumber, oldSeatNumber);
            drawSeatID();
        } 
    } 

    protected void showNotify() {
        // Start the timer for the cursor.
        timer = new Timer();

        TimerTask task = new TimerTask() {
            public void run() {
                cursorOn = !cursorOn;

                drawCursor();
            } 

        };

        timer.schedule(task, 750, 750);
    } 

    protected void hideNotify() {
        // Stop the timer for the cursor.
        timer.cancel();
    } 

}

