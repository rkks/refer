package com.jspbook;

import java.io.*;
import javax.servlet.*;
import javax.servlet.http.*;

public class Servlet2Servlet extends HttpServlet {

  public void doGet(HttpServletRequest request,
                    HttpServletResponse response)
    throws IOException, ServletException {
      response.setContentType("text/html");

      String param = request.getParameter("value");
      if(param != null && !param.equals("")) {
        request.setAttribute("value", param);
        RequestDispatcher rd = request.getRequestDispatcher("/Servlet2Servlet2");
        rd.forward(request, response);
        return;
      }

      PrintWriter out = response.getWriter();
      out.println("<html>");
      out.println("<head>");
      out.println("<title>Servlet #1</title>");
      out.println("</head>");
      out.println("<body>");
      out.println("<h1>A form from Servlet #1</h1>");
      out.println("<form>");
      out.println("Enter a value to send to Servlet #2.");
      out.println("<input name=\"value\"><br>");
      out.print("<input type=\"submit\" ");
      out.println("value=\"Send to Servlet #2\">");
      out.println("</form>");
      out.println("</body>");
      out.println("</html>");
  }
}
