package com.jspbook;

import java.io.*;
import java.util.logging.*;
import javax.servlet.*;
import javax.servlet.http.*;

public class SiteLogger implements ServletContextListener {

  // Site Logger
  private static Logger logger;

  public static Logger getLogger() throws IOException {
    return logger;
  }

  public void contextInitialized(ServletContextEvent e) {
    ServletContext sc = e.getServletContext();

    // Get an instance of a Logger
    logger = Logger.getLogger("global");
    logger.setLevel(Level.INFO);

    try {
      FileHandler fh = null;
      String root = sc.getRealPath("/");
      fh = new FileHandler(root+"WEB-INF/log.txt");
      fh.setFormatter(new CustomFormatter());
      logger.addHandler(fh);
    } catch (IOException ee) {
      System.err.println("Can't load logger: " +ee.getMessage());
    }

    sc.setAttribute("com.jspbook.SiteLogger", logger);
  }

  public void contextDestroyed(ServletContextEvent e) {
    ServletContext sc = e.getServletContext();
    sc.removeAttribute("com.jspbook.SiteLogger");
    logger = null;
  }
}
