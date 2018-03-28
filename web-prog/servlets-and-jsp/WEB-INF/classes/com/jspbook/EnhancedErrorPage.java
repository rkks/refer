package com.jspbook;

import java.io.*;
import javax.servlet.*;
import javax.servlet.http.*;

public class EnhancedErrorPage extends HttpServlet {

  public void doGet(HttpServletRequest request,
                    HttpServletResponse response)
    throws IOException, ServletException {

    response.setContentType("text/html");
    PrintWriter out = response.getWriter();
    
    // Load contact email from application inital parameters
    ServletContext sc = getServletConfig().getServletContext();
    String adminEmail = sc.getInitParameter("admin email");

    // Get the exception passed by JSP
    Exception e = (Exception)request.getAttribute("javax.servlet.jsp.jspException");
    
    out.println("<html>");
    out.println("<head>");
    out.println("<title>An Error Has Occurred!</title>");
    out.println("</head>");
    out.println("<body>");
    out.println("<h3>An Error Has Occurred</h3>");
    out.println("Sorry, but this site is unavailable to render");
    out.println(" the service you requested. A bug in the");
    out.println("system has caused an error to occur. Please ");
    out.println("send a description of the problem to ");
    out.println(adminEmail +" with, \"" + e.getMessage() + "\", ");
    out.println(" listed as the cause of the error.");
    out.println("</body>");
    out.println("</html>");
    }
}
