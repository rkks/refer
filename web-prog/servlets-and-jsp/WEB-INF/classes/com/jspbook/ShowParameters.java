package com.jspbook;

import java.util.*;
import java.io.*;
import javax.servlet.*;
import javax.servlet.http.*;

public class ShowParameters extends HttpServlet {

  public void doGet(HttpServletRequest request,
                    HttpServletResponse response)
    throws IOException, ServletException {

    response.setContentType("text/html");
    PrintWriter out = response.getWriter();
    out.println("<html>");
    out.println("<head>");
    out.println("<title>Request HTTP Parameters Sent</title>");
    out.println("</head>");
    out.println("<body>");
    out.println("<p>Parameters sent with request:</p>");
    Enumeration enum = request.getParameterNames();
    while (enum.hasMoreElements()) {
      String pName = (String) enum.nextElement();
      String[] pValues = request.getParameterValues(pName);
      out.print("<b>"+pName + "</b>: ");
      for (int i=0;i<pValues.length;i++) {
        out.print(pValues[i]);
      }
      out.print("<br>");
    }
    out.println("</body>");
    out.println("</html>");
  }
  public void doPost(HttpServletRequest request,
                    HttpServletResponse response)
    throws IOException, ServletException {
    doGet(request, response);
  }
}
