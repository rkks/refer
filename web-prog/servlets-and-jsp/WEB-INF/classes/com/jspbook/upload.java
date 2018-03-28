package com.jspbook;

import javax.servlet.http.*;
import javax.servlet.*;
import java.io.*;
import java.sql.*;
import javax.sql.*;
import javax.naming.*;
import java.util.Calendar;
import org.apache.commons.fileupload.*;
import java.util.*;

public class upload implements Control {
  public boolean doLogic(HttpServletRequest request,
                         HttpServletResponse response)
  throws ServletException, IOException {
    long ts = Calendar.getInstance().getTimeInMillis();

    ServletContext sc = request.getSession().getServletContext();
    String path = sc.getRealPath("/images");
    org.apache.commons.fileupload.FileUpload fu = new
        org.apache.commons.fileupload.FileUpload();
    fu.setSizeMax(-1);
    fu.setRepositoryPath(path);
    try {
      List l = fu.parseRequest(request);
      Iterator i = l.iterator();
      while (i.hasNext()) {
        FileItem fi = (FileItem)i.next();
        String postFix = ".unknown";
        String name = fi.getName();
        int lastDot = name.lastIndexOf(".");
        if (lastDot != -1) {
          postFix = fi.getName().substring(lastDot, name.length());
        }
        fi.write(path+"/"+ts+postFix);

        // update db
        try {
          InitialContext ctx = new InitialContext();
          DataSource ds =
            (DataSource)ctx.lookup("java:comp/env/jdbc/jspbook_site");
          Connection conn = ds.getConnection();
          try {
            Statement statement  = conn.createStatement();
            statement.executeQuery(
              "insert into images values('"+ts+postFix+"', 0)");
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
      }
    } catch (Exception e) {
      throw new ServletException(e);
    }

    response.sendRedirect("index.jsp");
    return false;
  }
}
