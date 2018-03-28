package com.enterprisej2me.osgi.echoserviceimpl;

import com.enterprisej2me.osgi.echoservice.*;

public class EchoServiceImpl implements EchoService {

  EchoServiceImpl () { }

  public String echo (String s) {
    return s;
  }

}