import javax.microedition.midlet.*;
import javax.microedition.lcdui.*;

public class FontMidlet extends MIDlet {
   public FontMidlet() { // constructor
   }

   public void startApp() {
      Canvas canvas = new FontCanvas();
      Display display = Display.getDisplay(this);
      display.setCurrent(canvas);
   }

   public void pauseApp() {
   }

   public void destroyApp(boolean unconditional) {
   }
}
