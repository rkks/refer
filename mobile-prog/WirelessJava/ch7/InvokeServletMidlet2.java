import javax.microedition.rms.*;
import javax.microedition.lcdui.*;
import javax.microedition.midlet.*;
import javax.microedition.io.*;
import java.io.*;

public class InvokeServletMidlet2 extends MIDlet implements CommandListener {
   Display display = null;
   List menu = null; 
   TextBox input = null;
   String user = null;    

   String url = "http://127.0.0.1:8080/examples/servlet/RequestServlet2";
   static final Command backCommand = new Command("Back", Command.BACK, 0);
   static final Command submitCommand = new Command("Submit", Command.OK, 2);
   static final Command exitCommand = new Command("Exit", Command.STOP, 3);
   String currentMenu = null;

   public InvokeServletMidlet2() {
   }

   public void startApp() throws MIDletStateChangeException {
      display = Display.getDisplay(this);
      menu = new List("Invoke Servlet", Choice.IMPLICIT);
      menu.append("Add a user", null);
      menu.addCommand(exitCommand);
      menu.setCommandListener(this);
      mainMenu();
   }

   public void pauseApp() {
   }

   public void destroyApp(boolean unconditional) {
      notifyDestroyed();
   }

   void mainMenu() {
      display.setCurrent(menu);
   }

   public void addName() {
      input = new TextBox("Enter first name:", "", 5, TextField.ANY);
      input.addCommand(submitCommand);
      input.addCommand(backCommand);
      input.setCommandListener(this);
      input.setString("");
      display.setCurrent(input);
   }

   void invokeServlet(String url) throws IOException {
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
         String str = "name="+user;
         byte postmsg[] = str.getBytes();
         System.out.println("Length: "+str.getBytes());
         for(int i=0;i<postmsg.length;i++) {
            os.write(postmsg[i]);
         }
         // or you can easily do:
         //os.write(("name="+user).getBytes()); 
         os.flush();

         is = c.openDataInputStream();
         int ch;
         while ((ch = is.read()) != -1) {
            b.append((char) ch);
            System.out.print((char)ch);
         }
         t = new TextBox("Second Servlet", b.toString(), 1024, 0);
         t.addCommand(backCommand);
         t.setCommandListener(this);
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

   public void commandAction(Command c, Displayable d) {
      String label = c.getLabel();
      if (label.equals("Exit")) {
         destroyApp(true);
      } else if (label.equals("Back")) {
         mainMenu();
      } else if (label.equals("Submit")) {
         user = input.getString();
         try {
            invokeServlet(url);
         }catch(IOException e) {}
      } else {
         addName();
      } 
   }
}
