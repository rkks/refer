package com.enterprisej2me.SmartPhrases.midp.MIDlets;
 
import javax.microedition.lcdui.*;

/**
 * Displays an error alert screen with the specified title, error message.
 * Also, displays exception and jumps to next screen if one specified.
 *
 */
public class ErrorAlert  {

    /**
     * Displays an error alert screen with the specified error message.
     *
     * @param title The title for the <code>Alert</code>.
     * @param body The message body for the <code>Alert</code>.
     * @param e The exception to display
     * @param nextscreen The <code>Displayable</code> to show after the
     *        <code>Alert</code> is discarded.
     * @param disp the LCDUI display context
     */
    public ErrorAlert(String      title,
                      String      body,
                      Exception   e,
                      Displayable nextscreen,
                      Display     disp) {

        String msg = body;

        if (e != null)
            msg = body + ": " + e.toString();

        Alert a = new Alert(title, msg, null, AlertType.ERROR);

        a.setTimeout(Alert.FOREVER);

        if(nextscreen != null)
            disp.setCurrent(a, nextscreen);
        else
            disp.setCurrent(a);

    }

}
