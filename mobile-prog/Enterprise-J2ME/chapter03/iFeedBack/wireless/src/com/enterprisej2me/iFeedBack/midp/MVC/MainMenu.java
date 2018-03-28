package com.enterprisej2me.iFeedBack.midp.MVC;

import java.io.*;
import javax.microedition.midlet.*;
import javax.microedition.lcdui.*;
import javax.microedition.rms.*;
import com.enterprisej2me.iFeedBack.midp.Task.*;
import com.enterprisej2me.iFeedBack.midp.*;

public class MainMenu extends MVCComponent {  

  private static Displayable screen = null; 
  private static Command exitCommand;
  private static Command selectCommand;
  private static Command doneCommand;

  // The calling mdlet. Set from outsdie
  public static Start m;

  public MainMenu () { 
  }

  public Displayable getScreen () {
    return screen;
  }

  public void commandAction(Command c, Displayable s) {
    try {
      if (c == exitCommand) {
        m.destroyApp(false);
        m.notifyDestroyed();

      } else if (c == doneCommand) {
        showScreen();

      } else if (c == List.SELECT_COMMAND ||
                 c == selectCommand ) {
        if (((List) screen).getSelectedIndex() == 0) {
          (new ChooseCourse()).showScreen();
        } else if (((List) screen).getSelectedIndex() == 1) {
          (new UpdateToken()).showScreen();
        } else if (((List) screen).getSelectedIndex() == 2) {
          (new AddCourse()).showScreen();
        } else if (((List) screen).getSelectedIndex() == 3) {
          SubmitAllTask t = new SubmitAllTask( display );
          t.go();
        } else if (((List) screen).getSelectedIndex() == 4) {

          UpdateToken.clearAll ();
          ChooseCourse.clearAll ();

          RecordStore rs;
          RecordEnumeration re;
          rs = RecordStore.openRecordStore("answer", true);
          re = rs.enumerateRecords(null, null, false);
          while ( re.hasNextElement() ) {
            int id = re.nextRecordId ();
            rs.deleteRecord( id );
          }
          re.destroy();
          rs.closeRecordStore();

          Alert a = new Alert("Cache cleared");
          a.setTimeout(Alert.FOREVER);
          display.setCurrent(a);

        } else if (((List) screen).getSelectedIndex() == 5) {
          String helpStr;
          Class clazz = this.getClass();
          InputStream is = clazz.getResourceAsStream("/help.txt");
          ByteArrayOutputStream bos = new ByteArrayOutputStream ();
          byte [] buf = new byte [256];
          while (true) {
            int rd = is.read (buf, 0, 256);
            if (rd == -1) break;
            bos.write (buf, 0, rd);
          }
          buf = bos.toByteArray ();
          helpStr = new String(buf);
          bos.close();
          is.close();
 
          Form form = new Form("Help");
          form.append( helpStr );
          form.addCommand(doneCommand);
          form.setCommandListener( (CommandListener) this);
          display.setCurrent( form );
        }
      }
    } catch (Exception e) {
      e.printStackTrace();
      Alert a = new Alert("Error in the Main Menu");
      a.setTimeout(Alert.FOREVER);
      display.setCurrent(a);
    }
  }

  protected void initModel() throws Exception {
    // nothing to do here
  }

  protected void createView() throws Exception {
    // static member, only init once
    exitCommand = new Command("EXIT", Command.SCREEN, 2);
    selectCommand = new Command("SELECT", Command.SCREEN, 1);
    doneCommand = new Command("DONE", Command.SCREEN, 1);

    screen = new List("Menu", List.IMPLICIT);
    ((List) screen).append("Choose Course", null);
    ((List) screen).append("Update Token", null);
    ((List) screen).append("Add Course", null);
    ((List) screen).append("Submit Answers", null);
    ((List) screen).append("Erase Cache", null);
    ((List) screen).append("Help", null);
    screen.addCommand( exitCommand );
    screen.addCommand( selectCommand );
  }

  protected void updateView() throws Exception {
    // nothing to do here
  }

}
