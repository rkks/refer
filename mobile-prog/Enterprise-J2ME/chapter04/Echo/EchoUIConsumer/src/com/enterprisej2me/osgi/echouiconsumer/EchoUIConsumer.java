package com.enterprisej2me.osgi.echouiconsumer;

import org.osgi.framework.*;
import org.osgi.util.tracker.*;
import com.enterprisej2me.osgi.echoservice.*;

public class EchoUIConsumer
        implements BundleActivator {

  ServiceTracker echoTracker;
  EchoFrame frame;

  public EchoUIConsumer () { }

  public void start (BundleContext context) {
    echoTracker = new ServiceTracker (context,
               EchoService.class.getName(), null );
    echoTracker.open ();
    frame = new EchoFrame(250, 250,
               echoTracker, context.getBundle());
  }

  public void stop (BundleContext context) {
    frame.dispose ();
    echoTracker.close();
  }
}
