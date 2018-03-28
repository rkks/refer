import javax.microedition.midlet.*;
import javax.microedition.lcdui.*;

public class PaymentMIDlet extends MIDlet {

    // The display for this MIDlet
    private Display display;
    // List to display payment methods
    List options = null;

    public PaymentMIDlet() {
       options = new List("Method of Payment", Choice.EXCLUSIVE); 
    }

    public void startApp() {
        display = Display.getDisplay(this);
        options.append("Visa", null);
        options.append("MasterCard", null);
        options.append("Amex", null);
        display.setCurrent(options);
    }

    /**
     * Pause is a no-op since there are no background activities or
     * record stores that need to be closed.
     */
    public void pauseApp() {
    }

    /**
     * Destroy must cleanup everything not handled by the garbage collector.
     * In this case there is nothing to cleanup.
     */
    public void destroyApp(boolean unconditional) {
    }
}
