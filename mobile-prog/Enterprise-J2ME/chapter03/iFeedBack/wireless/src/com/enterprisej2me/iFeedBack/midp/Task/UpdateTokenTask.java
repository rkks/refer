package com.enterprisej2me.iFeedBack.midp.Task;
 
import java.util.*;
import java.io.*;
import javax.microedition.lcdui.*;
import org.ksoap.*;
import org.ksoap.transport.*;
import com.enterprisej2me.iFeedBack.midp.MVC.*;

public class UpdateTokenTask extends BackgroundTask {

  private String endPointURL;

  public UpdateTokenTask (String url, Display d) throws Exception {
    super (d);
    endPointURL = url;
    prevScreen = (new UpdateToken()).prepareScreen();
  }

  public void runTask () throws Exception {
    String username = UpdateToken.getUsername ();
    String password = UpdateToken.getPassword ();
    String token = getToken(username, password);

    if ( "-1:-1".equals(token) ) {
      needAlert = true;
      alertScreen = new Alert("Cannot update token");
      alertScreen.setString("Authentication failed!");
      alertScreen.setTimeout(Alert.FOREVER);
      nextScreen = prevScreen;
    } else {
      UpdateToken.setAll( username, password, token );
      needAlert = true;
      alertScreen = new Alert("Success!");
      alertScreen.setString("The new token is " + token);
      alertScreen.setTimeout(Alert.FOREVER);
      nextScreen = (new MainMenu()).prepareScreen();
    }
  }

  private String getToken(String username, String password) throws Exception {
//    String endPointURL = "http://localhost:8080/iFeedBackSSO/server";
    SoapObject method = 
       new SoapObject("urn:enterprisej2me/iFeedBackSSO/Server", "getToken");
    method.addProperty("String_1", username );
    method.addProperty("String_2", password );
    HttpTransport rpc = new HttpTransport(endPointURL, "\"\"");
    return (String) rpc.call (method);
  }

}
