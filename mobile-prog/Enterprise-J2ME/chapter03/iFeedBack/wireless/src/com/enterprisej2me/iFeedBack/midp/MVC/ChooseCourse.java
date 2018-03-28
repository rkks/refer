package com.enterprisej2me.iFeedBack.midp.MVC;

import javax.microedition.lcdui.*;
import javax.microedition.rms.*;
import java.io.*;
import com.enterprisej2me.iFeedBack.midp.Task.*;

public class ChooseCourse extends MVCComponent {  

  private static Displayable screen = null; 
  private static Command backCommand;
  private static Command addCommand;
  private static Command selectCommand;
  private static RecordStore courseStore;

  // Model parameters
  private static String [] courseNickname;
  private static String [] courseEndpointURL;
  private static int index;

  public static String [] getCourseNickname () {
    return courseNickname;
  }

  public static String [] getCourseEndpointURL () {
    return courseEndpointURL;
  }

  public static int getIndex () {
    return index;
  }

  public static void setIndex (int i) {
    index = i;
  }

  public static void addPair (String nickname, String endpoint) 
                               throws Exception {
    // Expand the internal array
    int size;
    if ( courseEndpointURL == null ) {
      size = 0;
    } else {
      size = courseEndpointURL.length;
    }

    String [] tmp = new String[size+1];
    for ( int i = 0; i < size; i++ ) {
      tmp[i] = courseNickname[i];
    }
    tmp[size] = nickname;
    courseNickname = tmp;

    tmp = new String[size+1];
    for ( int i = 0; i < size; i++ ) {
      tmp[i] = courseEndpointURL[i];
    }
    tmp[size] = endpoint;
    courseEndpointURL = tmp;
    
    // Add stuff into the persistent storage
    courseStore = RecordStore.openRecordStore("course",
                                              true);
    ByteArrayOutputStream baos = new ByteArrayOutputStream ();
    DataOutputStream dout = new DataOutputStream (baos);
    dout.writeUTF (nickname);
    dout.writeUTF (endpoint);
    dout.flush();
    byte [] newrecord = baos.toByteArray();
    courseStore.addRecord(newrecord, 0, newrecord.length);
    dout.close();
    baos.close();
    courseStore.closeRecordStore();
  }

  public static void clearAll () throws Exception {
    courseNickname = new String[0];
    courseEndpointURL = new String[0];
    index = 0;
    courseStore = RecordStore.openRecordStore("course", true);
    RecordEnumeration re = 
          courseStore.enumerateRecords(null, null, false);
    while ( re.hasNextElement() ) {
      int id = re.nextRecordId ();
      courseStore.deleteRecord( id );
    }
    re.destroy();
    courseStore.closeRecordStore();
  }

  public ChooseCourse () { 
  }

  public Displayable getScreen () {
    return screen;
  }

  public void commandAction(Command c, Displayable s) {
    try {
      if (c == backCommand) {
        (new MainMenu()).showScreen();
      } else if (c == addCommand) {
        (new AddCourse()).showScreen();
      } else if (c == List.SELECT_COMMAND ||
                 c == selectCommand ) {
        index = ((List) screen).getSelectedIndex();
        ShowQuestionTask t = new ShowQuestionTask(display);
        t.go();
      }
    } catch (Exception e) {
      Alert a = new Alert("Error in ShowQuestion");
      a.setTimeout(Alert.FOREVER);
      display.setCurrent(a);
    }
  }

  protected void initModel() throws Exception {
    try {
      courseStore = RecordStore.openRecordStore("course",
                                                true);

      int courseNum = courseStore.getNumRecords();
      courseNickname = new String [courseNum];
      courseEndpointURL = new String [courseNum];

      RecordEnumeration re = courseStore.enumerateRecords(null,
                                                          null,
                                                          false);
      int i = 0;
      while ( re.hasNextElement() ) {
        ByteArrayInputStream bais = new ByteArrayInputStream(
                                        re.nextRecord() );
        DataInputStream din = new DataInputStream(bais);
        courseNickname[i] = din.readUTF();
        courseEndpointURL[i] = din.readUTF();
        din.close();
        bais.close();
        i++;
      }
      courseStore.closeRecordStore();
    } catch (Exception e) {
      courseNickname = new String[0];
      courseEndpointURL = new String[0];
    }
  }

  protected void createView() throws Exception {
    backCommand = new Command("MAIN", Command.SCREEN, 2);
    addCommand = new Command("ADD", Command.SCREEN, 1);
    selectCommand = new Command("SELECT", Command.SCREEN, 1);

    if (courseNickname.length == 0 ) {
      Form form = new Form("No course found");
      form.append("Please ADD course server URLs first.");
      form.addCommand( backCommand );
      form.addCommand( addCommand );
      screen = form;
    } else {
      screen = new List("Available Courses", List.IMPLICIT, 
                        courseNickname, null);
      screen.addCommand( backCommand );
      screen.addCommand( selectCommand );
    }
  }

  protected void updateView() throws Exception {
    // initModel();
    createView();
  }

}
