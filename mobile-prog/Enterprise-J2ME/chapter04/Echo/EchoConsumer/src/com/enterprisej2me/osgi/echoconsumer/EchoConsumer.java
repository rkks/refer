package com.enterprisej2me.osgi.echoconsumer;

import org.osgi.framework.*;
import com.enterprisej2me.osgi.echoservice.*;

public class EchoConsumer
        implements BundleActivator {

  ServiceReference ref;

  public EchoConsumer () { }

  public void start (BundleContext context) {
    ref = context.getServiceReference (
                EchoService.class.getName());
    EchoService echoObj =
            (EchoService) context.getService (ref);
    System.out.println(
            echoObj.echo("The consumer bundle"));
    System.out.println(
            echoObj.echo("Has been started"));
  }

  public void stop (BundleContext context) {
    context.ungetService (ref);
  }
}
