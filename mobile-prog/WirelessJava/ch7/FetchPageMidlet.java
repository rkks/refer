import java.io.*;
import javax.microedition.io.*;
import javax.microedition.lcdui.*;
import javax.microedition.midlet.*;

public class FetchPageMidlet extends MIDlet {

   private Display display;

   String url = "http://www.javacourses.com/hello.txt";

   public FetchPageMidlet() {
      display = Display.getDisplay(this);
   }

   /**
    * This will be invoked when we start the MIDlet
    */
   public void startApp() {
      try {
         getViaStreamConnection(url);
      } catch (IOException e) {
         //Handle Exceptions any other way you like.
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
    * read url via stream connection
    */
   void getViaStreamConnection(String url) throws IOException {
      StreamConnection c = null;
      InputStream s = null;
      StringBuffer b = new StringBuffer();
      TextBox t = null;
      try {
         c = (StreamConnection)Connector.open(url);
         s = c.openInputStream();
         int ch;
         while((ch = s.read()) != -1) {
            b.append((char) ch);
         }
         System.out.println(b.toString());
         t = new TextBox("Fetch Page", b.toString(), 1024, 0);
      } finally {
         if(s != null) {
            s.close();
         }
         if(c != null) {
            c.close();
         }
      }
      // display the contents of the file in a text box.
      display.setCurrent(t);
   }

}
