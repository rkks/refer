package com.enterprisej2me.iFeedBack.midp.Task;
 
import java.io.*;
import javax.microedition.lcdui.*;
import javax.microedition.rms.*;
import javax.microedition.io.*;
import com.enterprisej2me.iFeedBack.midp.MVC.*;

public class SubmitAllTask extends BackgroundTask {

  public SubmitAllTask (Display d) 
                   throws Exception {
    super (d);
    prevScreen = (new MainMenu()).prepareScreen();
  }

  public void runTask () throws Exception {

    RecordStore answerStore = 
      RecordStore.openRecordStore("answer", true);
    RecordEnumeration re = 
      answerStore.enumerateRecords(null, null, false);
    int recordNum = 0;
    while ( re.hasNextElement() ) {
      recordNum++;
      int recordid = re.nextRecordId();
      ByteArrayInputStream bais = 
        new ByteArrayInputStream(
          answerStore.getRecord(recordid) );
      DataInputStream din = new DataInputStream(bais);
      String url = din.readUTF();
      int qid = din.readInt();
      long timestamp = din.readLong();
      String answer = din.readUTF();
      String comment = din.readUTF();

      HttpConnection conn = null;
      DataInputStream hdin = null;
      DataOutputStream hdout = null;
      try {
        conn = (HttpConnection) Connector.open( url );
        conn.setRequestMethod(HttpConnection.POST);
        hdout = conn.openDataOutputStream();
        hdout.writeInt(1); // Submit opcode
        hdout.writeUTF( UpdateToken.getToken () );
        hdout.writeLong( timestamp );
        hdout.writeInt( qid );
        hdout.writeUTF( answer );
        hdout.writeUTF( comment );
        hdout.flush();

        hdin = conn.openDataInputStream();
        boolean authsucc = hdin.readBoolean();
        if (authsucc) {
          boolean updatesucc = hdin.readBoolean();
          if ( updatesucc ) {
            answerStore.deleteRecord( recordid );
          } else {
            needAlert = true;
            alertScreen = new Alert("Server error");
            alertScreen.setString("Answers NOT submitted.");
            alertScreen.setTimeout(Alert.FOREVER);
          }
          nextScreen = prevScreen;
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
        if (hdin != null) {
          try { hdin.close(); }
          catch (Throwable e) { }
          hdin = null;
        }
        if (hdout != null) {
          try { hdout.close(); }
          catch (Throwable e) { }
          hdout = null;
        }
      }
    }
    re.destroy();
    answerStore.closeRecordStore(); 

    needAlert = true;
    if (recordNum == 0) {
      alertScreen = new Alert("No record");
      alertScreen.setString("You do not have saved answers");
      alertScreen.setTimeout(Alert.FOREVER);
    } else {
      alertScreen = new Alert("Records saved");
      alertScreen.setString(recordNum + 
            " records have been submitted to the server");
      alertScreen.setTimeout(Alert.FOREVER);
    }
    nextScreen = prevScreen;
  }

}
