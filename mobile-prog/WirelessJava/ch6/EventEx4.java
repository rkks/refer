import javax.microedition.midlet.*;
import javax.microedition.lcdui.*;

public class EventEx3 extends MIDlet {
   Display display;
   Command exit;


   public EventEx3() {
      display = Display.getDisplay(this);
   }

   public void destroyApp (boolean unconditional) {
   }

   public void pauseApp () {
      System.out.println("App paused.");
   }

   public void startApp () {
      display = Display.getDisplay(this);

      Canvas canvas = new Canvas() { // anonymous class     
         public void paint(Graphics g) {
         }

         protected void keyPressed(int keyCode) {
            if (keyCode > 0) {
               System.out.println("keyPressed " +((char)keyCode));
            } else {
               System.out.println("keyPressed action " +getGameAction(keyCode));
            }                  
         }

         protected void keyReleased(int keyCode) {
            if (keyCode > 0) {
               System.out.println("keyReleased " +((char)keyCode));
            } else {
               System.out.println("keyReleased action " +getGameAction(keyCode));
            } 
         }
      }; // end of anonymous class

      exit = new Command("Exit", Command.STOP, 1);
      canvas.addCommand(exit);
      canvas.setCommandListener(new CommandListener() {
         public void commandAction(Command c, Displayable d) {
            if(c == exit) {
               notifyDestroyed();
            } else {
               System.out.println("Saw the command: "+c);
            }
         }
      });
      display.setCurrent(canvas);
   }
}
