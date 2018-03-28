package com.enterprisej2me.osgi.echoserviceimpl;

import org.osgi.framework.*;
import com.enterprisej2me.osgi.echoservice.*;


public class EchoActivator implements BundleActivator {

  private ServiceRegistration reg;

  public EchoActivator () { }

  public void start (BundleContext context) throws Exception {
    EchoServiceImpl impl = new EchoServiceImpl ();
    reg = context.registerService (
        EchoService.class.getName(), impl, null);
  }

  public void stop (BundleContext context) throws Exception {
    reg.unregister ();
  }

}