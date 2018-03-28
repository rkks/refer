import java.io.*;
import javax.microedition.io.*;
import javax.microedition.lcdui.*;
import javax.microedition.midlet.*;

/**
 * An example MIDlet to invoke a CGI script (POST method is used).
 */

public class InvokeCgiMidlet2 extends MIDlet {

   private Display display;

   String url = "http://www.javacourses.com/cgi-bin/pgrade.cgi";

   public InvokeCgiMidlet2() {
      display = Display.getDisplay(this);
   }

   /**
    * Initialization. Invoked when we activate the MIDlet.
    */
   public void startApp() {
      try {
         getGrade(url);
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

   void getGrade(String url) throws IOException {
      HttpConnection c = null;
      InputStream is = null;
      OutputStream os = null;
      StringBuffer b = new StringBuffer();
      TextBox t = null;
      try {
         c = (HttpConnection)Connector.open(url);
         c.setRequestMethod(HttpConnection.POST);
         c.setRequestProperty("CONTENT-TYPE","application/x-www-form-urlencoded");
         c.setRequestProperty("User-Agent","Profile/MIDP-1.0 Confirguration/CLDC-1.0");
         c.setRequestProperty("Content-Language", "en-CA");
         os = c.openOutputStream();

         // send input
         String str = "name=182016";
         byte postmsg[] = str.getBytes();
         for(int i=0;i<postmsg.length;i++) {
            os.write(postmsg[i]);
         }
         os.flush();
         is = c.openDataInputStream();
         int ch;
         // receive output
         while ((ch = is.read()) != -1) {
            b.append((char) ch);
            System.out.println((char)ch);
         }
         t = new TextBox("Final Grades", b.toString(), 1024, 0);
      } finally {
         if(is!= null) {
            is.close();
         }
         if(os != null) {
            os.close();
         }
         if(c != null) {
            c.close();
         }
      }
      display.setCurrent(t);
   }     
}
