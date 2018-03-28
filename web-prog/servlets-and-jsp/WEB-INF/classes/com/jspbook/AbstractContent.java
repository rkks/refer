package com.jspbook;

import javax.servlet.http.*;
import javax.servlet.*;
import java.io.*;
import java.util.*;
import java.sql.*;
import javax.sql.*;
import javax.naming.*;
import java.text.*;

public abstract class AbstractContent implements Control {
  public abstract boolean doLogic(HttpServletRequest request,
                         HttpServletResponse response)
  throws ServletException, IOException;

  public boolean doLogic(HttpServletRequest request,
                         HttpServletResponse response, String table)
  throws ServletException, IOException {
    LinkedList content = new LinkedList();
    request.setAttribute(table, content);

    try {
      InitialContext ctx = new InitialContext();
      DataSource ds =
        (DataSource)ctx.lookup("java:comp/env/jdbc/jspbook_site");
      Connection conn = ds.getConnection();
      try {
        Statement statement  = conn.createStatement();
        // changed to sort by date
        ResultSet rs =
          statement.executeQuery("select * from "+table+" where visible=true order by ts desc");
        ResultSetMetaData rsmd = rs.getMetaData();
        while (rs.next()) { // show results
          long l = rs.getLong(1);
          Timestamp ts = new Timestamp(l);
          SimpleDateFormat sdf =
            new SimpleDateFormat("EEE, MMM d, yyyy HH:mm:ss z");
          java.util.Date d = new java.util.Date(l);
          String con =
            "<p class=\"date\"><a name=\""+l+"\"></a>"+sdf.format(d)+"</p>";
          String u=
            "/WEB-INF/"+table+"/"+new Long(l).toString()+".jsp";

          // fix this...
          StringWriter sw = new StringWriter();
          String file = request.getSession().getServletContext()
            .getRealPath(SimpleI18nFilter.getLanguage(u, request));
          FileInputStream fis = new FileInputStream(file);
          int i = 0;
          while ((i = fis.read()) != -1) {
            sw.write(i);
          }

          con += new String(sw.toString());
          content.add(con);
        }
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
    return true;
  }
}
