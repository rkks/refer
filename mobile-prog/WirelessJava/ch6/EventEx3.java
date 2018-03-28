import javax.microedition.midlet.*;
import javax.microedition.lcdui.*;

public class EventEx3 extends MIDlet {
   Display display;

   public EventEx3() {
      display = Display.getDisplay(this);
   }

   public void destroyApp (boolean unconditional) {
      notifyDestroyed();
      System.out.println("App destroyed ");
   }

   public void pauseApp () {
      display = null;
      System.out.println("App paused.");
   }

   public void startApp () {
      Form form = new Form("Change Date");
        
      ItemStateListener listener = new ItemStateListener() {
         java.util.Calendar cal = 
           java.util.Calendar.getInstance(java.util.TimeZone.getDefault());

         public void itemStateChanged(Item item) {
            cal.setTime(((DateField)item).getDate());
            System.out.println("\nDate has changed");
         }
      };

      // register for events
      form.setItemStateListener(listener);
        
      // get today's date
      java.util.Date now = new java.util.Date();
      DateField dateItem = new DateField("Today's date:", DateField.DATE);
      dateItem.setDate(now);

      // add date to the Form screen
      form.append(dateItem);
      display.setCurrent(form);
   }
}
