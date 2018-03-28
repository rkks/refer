package com.jspbook;

import java.io.*;
import javax.servlet.*;
import javax.servlet.http.*;

public class DisplayUsers extends HttpServlet {
  public void doGet(HttpServletRequest request,
                    HttpServletResponse response)
  throws IOException, ServletException {

    request.getSession();

    response.setContentType("text/html");
    PrintWriter out = response.getWriter();
    out.println("<html>");
    out.print("Users:");
    out.print(ConcurrentUserTracker.getConcurrentUsers());
    out.println("</html>");
  }
}
