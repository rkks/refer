package com.jspbook;

import java.io.*;
import javax.servlet.*;
import javax.servlet.http.*;
import java.util.*;

public class ShowSource extends HttpServlet {

  public void doGet(HttpServletRequest request,
                    HttpServletResponse response)
    throws IOException, ServletException {

    PrintWriter out = response.getWriter();

    // Get the ServletContext
    ServletConfig config = getServletConfig();
    ServletContext sc = config.getServletContext();

    // Check to see if a resource was requested
    String resource = request.getParameter("resource");
    if (resource != null && !resource.equals("")) {
      
      // Use getResourceAsStream() to properly get the file.
      InputStream is = sc.getResourceAsStream(resource);
      if (is !=  null) {
        response.setContentType("text/plain");
        StringWriter sw = new StringWriter();
        for (int c = is.read(); c != -1; c = is.read()) {
          sw.write(c);
        }
        out.print(sw.toString());
      }
    }
    // Show the HTML form.
    else {
      response.setContentType("text/html");
      out.println("<html>");
      out.println("<head>");
      out.println("<title>Source-Code Servlet</title>");
      out.println("</head>");
      out.println("<body>");
      out.println("<form>");
      out.println("Choose a resource to see the source.<br>");
      out.println("<select name=\"resource\">");
      // List all the resources in this Web Application
      listFiles(sc, out, "/");
      out.println("</select><br>");
      out.print("<input type=\"submit\" ");
      out.println("value=\"Show Source\">");
      out.println("</body>");
      out.println("</html>");
    }
  }

  // Recursively list all resources in Web App
  void listFiles(ServletContext sc, PrintWriter out,String base){
      Set set = sc.getResourcePaths(base);
      Iterator i = set.iterator();
      while (i.hasNext()) {
        String s = (String)i.next();
        if (s.endsWith("/")) {
          listFiles(sc, out, s);
        }
        else {
          out.print("<option value=\""+s);
          out.println("\">"+s+"</option>");
        }
      }
  }
}
