package com.jspbook;

import java.util.*;
import java.io.*;
import javax.servlet.*;
import javax.servlet.http.*;

public class ShowHeaders extends HttpServlet {

  public void doGet(HttpServletRequest request,
                    HttpServletResponse response)
    throws IOException, ServletException {
      response.setContentType("text/html");
      PrintWriter out = response.getWriter();
      out.println("<html>");
      out.println("<head>");
      out.println("<title>Request's HTTP Headers</title>");
      out.println("</head>");
      out.println("<body>");
      out.println("<p>HTTP headers sent by your client:</p>");
      Enumeration enum = request.getHeaderNames();
      while (enum.hasMoreElements()) {
        String headerName = (String) enum.nextElement();
        String headerValue = request.getHeader(headerName);
        out.print("<b>"+headerName + "</b>: ");
        out.println(headerValue + "<br>");
      }
      out.println("</body>");
      out.println("</html>");
  }
}
