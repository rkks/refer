package com.enterprisej2me.iFeedBack.midp.Task;
 
import java.io.*;
import javax.microedition.lcdui.*;
import javax.microedition.io.*;
import com.enterprisej2me.iFeedBack.midp.MVC.*;

public class ShowQuestionTask extends BackgroundTask {

  public ShowQuestionTask (Display d) throws Exception {
    super (d);
    prevScreen = (new ChooseCourse()).prepareScreen();
  }

  public void runTask () throws Exception {

    HttpConnection conn = null;
    DataInputStream din = null;
    DataOutputStream dout = null;
    try {
      String [] endpoints = ChooseCourse.getCourseEndpointURL ();
      String url = endpoints[ChooseCourse.getIndex ()];
      conn = (HttpConnection) Connector.open( url );
      conn.setRequestMethod(HttpConnection.POST);
      dout = conn.openDataOutputStream();
      dout.writeInt(0); // Get opcode
      dout.writeUTF( UpdateToken.getToken () );
      dout.flush();
 
      din = conn.openDataInputStream();
      boolean succ = din.readBoolean();
      if ( succ ) {
        ShowQuestion.setQid ( din.readInt() );
        ShowQuestion.setQuestion ( din.readUTF() );
        int numchoices = din.readInt();
        String [] choices = new String [numchoices];
        for (int i=0; i < numchoices; i++) {
          choices[i] = din.readUTF();
        }
        ShowQuestion.setChoices(choices);
        ShowQuestion.setComment("");

        nextScreen = (new ShowQuestion()).prepareScreen();
      } else {
        needAlert = true;
        alertScreen = new Alert("Auth failed!");
        alertScreen.setString("Maybe your token has expired");
        alertScreen.setTimeout(Alert.FOREVER);
        nextScreen = (new UpdateToken()).prepareScreen();
      }
    } finally { 
      if (conn != null) {
        try { conn.close(); } 
        catch (Throwable e) { }
        conn = null;
      }
      if (din != null) {
        try { din.close(); }
        catch (Throwable e) { }
        din = null;
      }
      if (dout != null) {
        try { dout.close(); }
        catch (Throwable e) { }
        dout = null;
      }
    }
  }

}
