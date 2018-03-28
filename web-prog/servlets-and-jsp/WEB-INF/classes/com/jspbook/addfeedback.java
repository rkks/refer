package com.jspbook;

import javax.servlet.http.*;
import javax.servlet.*;
import java.io.*;
import java.sql.*;
import javax.sql.*;
import java.util.*;
import javax.naming.*;

public class addfeedback implements Control {
  public boolean doLogic(HttpServletRequest request,
                      HttpServletResponse response)
  throws ServletException, IOException {

    String feedback = request.getParameter("feedback");
    String author = request.getParameter("author");
    String email = request.getParameter("email");
    // check values are not null
    if (feedback == null) {
      return true;
    }

    try {
      InitialContext ctx = new InitialContext();
      DataSource ds =
       (DataSource)ctx.lookup("java:comp/env/jdbc/jspbook_site");
      Connection conn = ds.getConnection();
      try {
        Statement statement  = conn.createStatement();

        ServletContext sc =
          request.getSession().getServletContext();
        String dir = sc.getRealPath("/WEB-INF/feedback");
        new File(dir).mkdirs();
        long l = Calendar.getInstance().getTimeInMillis();

        // write out news item
        FileWriter fw = new FileWriter(dir+"/"+l+".jsp");
        fw.write("<p class=\"title\">"+author+" ("+email+")</p>");
        fw.write("<p class=\"news-content\">");
        fw.write(feedback);
        fw.write("</p>");
        fw.flush();
        fw.close();
        // add entry in db
        statement.executeQuery(
          "insert into feedback values ("+l+",0)");
        response.sendRedirect("index.jsp");
      } catch (Exception e) {
        throw new ServletException(e);
      } finally {
        conn.close();
      }
    } catch (SQLException e) {
      throw new ServletException(e);
    }
    catch (NamingException e) {
      throw new ServletException(e);
    }
    return false;
  }
}
