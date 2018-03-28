package com.jspbook;

import java.io.*;
import javax.servlet.*;
import javax.servlet.http.*;

public class Servlet2Servlet2 extends HttpServlet {

  public void doGet(HttpServletRequest request,
                    HttpServletResponse response)
    throws IOException, ServletException {
      response.setContentType("text/html");

      PrintWriter out = response.getWriter();
      out.println("<html>");
      out.println("<head>");
      out.println("<title>Servlet #2</title>");
      out.println("</head>");
      out.println("<body>");
      out.println("<h1>Servlet #2</h1>");
      String value = (String)request.getAttribute("value");
      if(value != null && !value.equals("")) {
        out.print("Servlet #1 passed a String object via ");
        out.print("request scope. The value of the String is: ");
        out.println("<b>"+value+"</b>.");
      }
      else {
        out.println("No value passed!");
      }
      out.println("</body>");
      out.println("</html>");
  }
}
