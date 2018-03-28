package com.jspbook;

import javax.servlet.http.*;
import javax.servlet.*;
import java.io.*;
import java.util.logging.*;

public class error implements Control {
  public boolean doLogic(HttpServletRequest request,
                      HttpServletResponse response)
  throws ServletException, IOException {

    // get logger
    Logger logger = com.jspbook.SiteLogger.getLogger();
    // get exception
    Exception e =
   (Exception)request.getAttribute("javax.servlet.jsp.jspException");

    //log the stack trace
    if (e != null) {
      StringWriter message = new StringWriter();
      e.printStackTrace(new PrintWriter(message));
      logger.severe(message.toString());
    }
    return true;
  }
}
