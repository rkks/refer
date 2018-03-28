package com.enterprisej2me.iFeedBack.midp.MVC;

import java.util.*;
import java.io.*;
import javax.microedition.lcdui.*;
import javax.microedition.rms.*;
import javax.microedition.io.*;
import com.enterprisej2me.iFeedBack.midp.Task.*;

public class ShowQuestion extends MVCComponent {  

  private static Displayable screen = null; 
  private static Command backCommand;
  private static Command saveCommand;
  private static TextField commentField;
  private static ChoiceGroup choicesList;
  private static RecordStore answerStore;
  private static boolean [] answers;

  // Model parameters
  private static int qid;
  private static String question;
  private static String [] choices;
  private static String comment;

  public static int getQid() {
    return qid;
  }

  public static void setQid(int i) {
    qid = i;
  }

  public static String getQuestion() {
    return question;
  }

  public static void setQuestion(String s) {
    question = s;
  }

  public static String getComment() {
    return comment;
  }

  public static void setComment(String s) {
    comment = s;
  }

  public static String [] getChoices() {
    return choices;
  }

  public static void setChoices(String [] s) {
    choices = s;
  }

  public ShowQuestion () { 
  }

  public Displayable getScreen () {
    return screen;
  }

  public void commandAction(Command c, Displayable s) {
    try {
      if (c == backCommand) {
        (new ChooseCourse()).showScreen();
      } else if (c == saveCommand) {
        comment = commentField.getString();
        answers = new boolean [choicesList.size()];
        choicesList.getSelectedFlags( answers );
        String answersStr = "";
        for ( int i = 0; i < answers.length; i++ ) {
          if ( answers[i] ) {
            answersStr = answersStr + i + ",";
          }
        }
        Calendar cal = Calendar.getInstance();
        long timestamp = cal.getTime().getTime();

        answerStore = RecordStore.openRecordStore("answer",
                                                  true);
        ByteArrayOutputStream baos = new ByteArrayOutputStream ();
        DataOutputStream dout = new DataOutputStream (baos);
        String [] tmp = ChooseCourse.getCourseEndpointURL ();
        dout.writeUTF (tmp[ChooseCourse.getIndex ()]);
        dout.writeInt (qid);
        dout.writeLong (timestamp);
        dout.writeUTF (answersStr);
        dout.writeUTF (comment);
        dout.flush();
        byte [] newrecord = baos.toByteArray();
        answerStore.addRecord(newrecord, 0, newrecord.length);
        dout.close();
        baos.close();
        answerStore.closeRecordStore(); 

        (new ChooseCourse()).showScreen();
      }
    } catch (Exception e) {
      Alert a = new Alert("Error in ShowQuestion");
      a.setTimeout(Alert.FOREVER);
      display.setCurrent(a);
    }
  }

  protected void initModel() throws Exception {
    if ( question == null )
      question = "Cannot download question";
    if ( choices == null )
      choices = new String [0];
    if ( comment == null )
      comment = "";
  }

  protected void createView() throws Exception {
    backCommand = new Command("MAIN", Command.SCREEN, 2);
    saveCommand = new Command("SAVE", Command.SCREEN, 1);
    choicesList = new ChoiceGroup("", Choice.MULTIPLE, 
                                  choices, null);
    commentField = new TextField("Comment:", comment,
                                 40, TextField.ANY);

    screen = new Form("Question");
    ((Form) screen).append( question );
    ((Form) screen).append( choicesList );
    ((Form) screen).append( commentField );
    screen.addCommand( backCommand );
    screen.addCommand( saveCommand );
  }

  protected void updateView() throws Exception {
    createView();
  }

}
