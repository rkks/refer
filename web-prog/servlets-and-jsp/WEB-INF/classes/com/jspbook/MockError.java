package com.jspbook;

import java.io.*;
import javax.servlet.*;
import javax.servlet.http.*;

public class MockError extends HttpServlet {

  public void doGet(HttpServletRequest request,
                    HttpServletResponse response)
    throws IOException, ServletException {

    response.setContentType("text/html");
    PrintWriter out = response.getWriter();
    out.println("<html>");
    out.println("<head>");
    out.println("<title>An Error Has Occurred!</title>");
    out.println("</head>");
    out.println("<body>");
    ServletContext sc = getServletConfig().getServletContext();
    String adminEmail = sc.getInitParameter("admin email");
    out.println("<h1>Error Page</h1>");
    out.print("Sorry! An unexpected error has occurred.");
    out.print("Please send an error report to "+adminEmail+".");
    out.println("</body>");
    out.println("</html>");
  }
}
