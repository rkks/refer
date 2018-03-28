package com.jspbook;

import javax.servlet.http.*;
import javax.servlet.*;
import java.io.*;
import java.util.*;
import java.sql.*;
import javax.sql.*;
import javax.naming.*;
import java.text.*;

public class edit implements Control {
  public boolean doLogic(HttpServletRequest request,
                      HttpServletResponse response)
  throws ServletException, IOException {

    // filter random requests
    String table = request.getParameter("table");
    if (table == null) {
      response.sendRedirect("index.jsp");
      return false;
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

        ResultSet rs =
          statement.executeQuery("select * from "+table);
        LinkedList dates = new LinkedList();
        LinkedList longs = new LinkedList();
        LinkedList visibles = new LinkedList();
        LinkedList texts = new LinkedList();
        while (rs.next()) {
          long l = rs.getLong(1);
          longs.add(new Long(l).toString());
          Timestamp ts = new Timestamp(l);
          SimpleDateFormat sdf =
            new SimpleDateFormat("EEE, MMM d, yyyy HH:mm:ss z");
          java.util.Date d = new java.util.Date(l);
          dates.add(sdf.format(d));

          if (rs.getBoolean(2)) {
            visibles.add("checked");
          } else {
            visibles.add("");
          }

          ServletContext sc =
            request.getSession().getServletContext();
          String f = sc.getRealPath(
            "/WEB-INF/"+table+"/"+String.valueOf(l)+".jsp");

          StringWriter sw = new StringWriter();
          FileReader fr = new FileReader(f);
          for (int i=fr.read();i!=-1;i=fr.read()) {
            sw.write(i);
          }
          texts.add(sw.toString());

        }
        request.setAttribute("dates", 
          dates.toArray(new String [0]));
        request.setAttribute("longs",
          longs.toArray(new String [0]));
        request.setAttribute("visibles",
          visibles.toArray(new String [0]));
        request.setAttribute("texts",
         texts.toArray(new String [0]));
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
