package com.jspbook;

import javax.servlet.http.*;
import javax.servlet.*;
import java.io.*;
import java.util.*;
import java.sql.*;
import javax.sql.*;
import javax.naming.*;
import java.text.*;

public class update implements Control {
  public boolean doLogic(HttpServletRequest request,
                      HttpServletResponse response)
  throws ServletException, IOException {

    // filter random requests
    String table = request.getParameter("table");
    if (table == null) {
      response.sendRedirect("index.jsp");
    }
    else if(!table.equals("faq") && !table.equals("news") &&
           !table.equals("errata") && !table.equals("feedback")){
      response.sendRedirect("index.jsp");
      return false;
    }

    try {
      InitialContext ctx = new InitialContext();
      DataSource ds =
       (DataSource)ctx.lookup("java:comp/env/jdbc/jspbook_site");
      Connection conn = ds.getConnection();
      try {
        Statement statement  = conn.createStatement();

        // loop through all posted items
        for (int i=0;request.getParameter("date"+i)!=null;i++) {
          // get variables we care about
          String date = request.getParameter("date"+i);
          String text = request.getParameter("text"+i);
          String visible = request.getParameter("visible"+i);

          // optionally delete
          String delete = request.getParameter("delete"+i);
          if (delete != null && !delete.equals("")) {
            ServletContext sc =
              request.getSession().getServletContext();
            String f =
              sc.getRealPath("/WEB-INF/news/"+date+".jsp");
            new File(f).delete();
            statement.executeQuery(
              "delete from "+table+" where ts="+date);
            continue;
          }

          ServletContext sc =
            request.getSession().getServletContext();
          String dir = sc.getRealPath("/WEB-INF/"+table);

          // write out news item
          FileWriter fw = new FileWriter(dir+"/"+date+".jsp");
          fw.write(text);
          fw.flush();
          fw.close();

          if (visible != null && visible.equals("on")) {
            visible = "1";
          } else {
            visible = "0";
          }
          statement.executeQuery(
      "update "+table+" set visible="+visible+" where ts="+date);
        }
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
