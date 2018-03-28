import javax.microedition.midlet.*;
import javax.microedition.lcdui.*;

public class HelloMidlet extends MIDlet {

    // The display for this MIDlet
    private Display display;
    // TextBox to display text
    TextBox box = null;

    public HelloMidlet() {
    }

    public void startApp() {
        display = Display.getDisplay(this);
        box = new TextBox("Simple Example", "Hello World", 20, 0);
        display.setCurrent(box);
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
