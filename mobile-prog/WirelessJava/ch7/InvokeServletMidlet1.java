import java.io.*;
import javax.microedition.io.*;
import javax.microedition.lcdui.*;
import javax.microedition.midlet.*;

/**
 * An example MIDlet to invoke a servlet.
 */

public class InvokeServletMidlet1 extends MIDlet {

   private Display display;
    
   String url = "http://127.0.0.1:8080/examples/servlet/HelloServlet";
    

   public InvokeServletMidlet1() {
      display = Display.getDisplay(this);
   }

   /**
    * Initialization. Invoked when we activate the MIDlet.
    */
   public void startApp() {
      try {
         invokeServlet(url);
      } catch (IOException e) {
         System.out.println("IOException " + e);
         e.printStackTrace();
      }
   }

   /**
    * Pause, discontinue ....
    */
   public void pauseApp() {
   }

   /**
    * Destroy must cleanup everything.
    */
   public void destroyApp(boolean unconditional) {
   }

   /**
    * Retrieve a grade....
    */
   void invokeServlet(String url) throws IOException {
      HttpConnection c = null;
      InputStream is = null;
      StringBuffer b = new StringBuffer();
      TextBox t = null;
      try {
         c = (HttpConnection)Connector.open(url);
         c.setRequestMethod(HttpConnection.GET);
         c.setRequestProperty("User-Agent","Profile/MIDP-1.0 Confirguration/CLDC-1.0");
         c.setRequestProperty("Content-Language", "en-CA");
         is = c.openDataInputStream();
         int ch;
         while ((ch = is.read()) != -1) {
            b.append((char) ch);
         }
         t = new TextBox("First Servlet", b.toString(), 1024, 0);
      } finally {
         if(is!= null) {
            is.close();
         }
         if(c != null) {
            c.close();
         }
      }
      display.setCurrent(t);
   }     
}
