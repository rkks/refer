package com.enterprisej2me.iFeedBack.singlesignon;
 
import java.rmi.*;
 
public interface Server extends Remote {

  public boolean authenticate( String token )
                      throws RemoteException;

  public String getToken( String username, 
                          String password )
                      throws RemoteException;

  /*
    Methods that are necessary for a generic single sign on 
    server but are not implemented in this version of 
    web service for simplicity.

    register();
    changePassword();
    emailPassword();
    removeUser()

  */
}
