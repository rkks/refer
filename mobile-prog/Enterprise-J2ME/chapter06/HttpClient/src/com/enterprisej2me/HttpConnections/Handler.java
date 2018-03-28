package com.enterprisej2me.HttpConnections;

import javax.microedition.io.*;

public interface Handler {

  // Add headers to c
  public void prepareHeaders (HttpConnection c) throws Exception;

  // Process headers. If we need re connect (e.g. authen hand shake)
  // return true; Otherwise return false.
  public boolean processHeaders (HttpConnection c) throws Exception;

}
