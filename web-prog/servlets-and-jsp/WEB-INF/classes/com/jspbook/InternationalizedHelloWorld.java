package com.jspbook;

import java.io.*;
import javax.servlet.*;
import javax.servlet.http.*;

public class InternationalizedHelloWorld extends HttpServlet {

  public void doGet(HttpServletRequest request,
                    HttpServletResponse response)
    throws IOException, ServletException {
      response.setContentType("text/html");
      PrintWriter out = response.getWriter();
      out.println("<html>");
      out.println("<head>");
      String greeting;
      greeting = getServletConfig().getInitParameter("greeting");
      out.println("<title>" +greeting+"</title>");
      out.println("</head>");
      out.println("<body>");
      out.println("<h1>" +greeting+"</h1>");
      out.println("</body>");
      out.println("</html>");
  }
}
