package com.enterprisej2me.iFeedBack.singlesignon;
 
import java.util.*;
import java.rmi.*;
 
import javax.xml.rpc.server.*;
import javax.xml.rpc.JAXRPCException;
 
public class ServerImpl implements ServiceLifecycle, Server {

  // Some canned data
  private String [] usernames =
             {"test01", "test02", "test03"};
  private String [] passwords =
             {"pass01", "pass02", "pass03"};
  private String [] userIDs =
             {"001", "002", "003"};
  private String [] randompass =
             {"111", "222", "333"};


  public void init( Object context ) throws JAXRPCException {
  }

  public void destroy () {
  }

  public boolean authenticate( String token )
                      throws RemoteException {
    try {
      StringTokenizer st = new StringTokenizer(
                            token, ":");
      String uid = st.nextToken();
      String upass = st.nextToken();
      for (int i=0; i < userIDs.length; i++) {
        if ( uid.equals(userIDs[i]) &&
             upass.equals(randompass[i]) ) {
          return true;
        }
      }
      return false;
    } catch (Exception e) {
      e.printStackTrace();
      return false;
    }
  }

  public String getToken( String username,
                          String password )
                      throws RemoteException {
    for (int i=0; i < usernames.length; i++) {
      if ( username.equals(usernames[i]) &&
           password.equals(passwords[i]) ) {
        return userIDs[i] + ":" + randompass[i];
      }
    }   
    return "-1:-1";
  }

}
