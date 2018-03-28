import javax.microedition.midlet.*;
import javax.microedition.lcdui.*;

public class MyMidlet extends MIDlet {
   public MyMidlet() { // constructor
   }

   public void startApp() {
      Canvas canvas = new MyCanvas();
      Display display = Display.getDisplay(this);
      display.setCurrent(canvas);
   }

   public void pauseApp() {
   }

   public void destroyApp(boolean unconditional) {
   }
}
