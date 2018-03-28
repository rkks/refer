package com.jspbook;

import java.util.*;
import javax.naming.*;
import java.sql.*;
import javax.sql.*;

public class LinkFactory {
  public static void create(LinkBean[] links) throws Exception {
    InitialContext ctx = new InitialContext();
    DataSource ds =
      (DataSource)ctx.lookup("java:comp/env/jdbc/jspbook");
    Connection conn = ds.getConnection();
    Statement statement  = conn.createStatement();
    try {
      for (int i=0; i<links.length;i++) {
        LinkBean link = links[i];
        if (link == null) continue;
        if (link.getUrl() == null) {
          throw new Exception("Link #"+i+" has null for url value!");
        }
        statement.executeQuery("INSERT INTO LINK VALUES('"+link.getUrl()+"', '"+link.getTitle()+"', '"+link.getDescription()+"')");
      }
    }
    finally {
      statement.close();
      conn.close();
    }
  }

  public static LinkBean[] read(String index) throws Exception {
    String sql = "SELECT * FROM LINK";
    if (index != null) {
      sql = "SELECT l.url, l.title, l.description FROM LINK l, URI u WHERE l.url = u.url AND u.uri = '"+index+"'";
    }
    InitialContext ctx = new InitialContext();
    DataSource ds =
      (DataSource)ctx.lookup("java:comp/env/jdbc/jspbook");
    Connection conn = ds.getConnection();
    Statement statement  = conn.createStatement();
    LinkedList ll = new LinkedList();
    try {
      ResultSet rs = statement.executeQuery(sql);
      while (rs.next()) { // show results
        LinkBean link = new LinkBean();
        link.setUrl(rs.getString(1));
        link.setTitle(rs.getString(2));
        link.setDescription(rs.getString(3));
        ll.add(link);
      }
      LinkBean[] links = new LinkBean[ll.size()];
      for(int i=0;ll.size() >0;i++) {
        links[i] = (LinkBean)ll.remove(0);
      }
      return links;
    }
    finally {
      conn.close();
    }
  }

  public static void update(LinkBean[] links) throws Exception {
    // validate input
    if (links == null || links.length == 0) return;
    InitialContext ctx = new InitialContext();
    DataSource ds =
      (DataSource)ctx.lookup("java:comp/env/jdbc/jspbook");
    Connection conn = ds.getConnection();
    Statement statement  = conn.createStatement();
    LinkedList ll = new LinkedList();
    try {
      for (int i=0;i<links.length;i++) {
        LinkBean link = links[i];
        if (link.getUrl() == null) continue;
        statement.executeQuery("UPDATE LINK SET title='"+link.getTitle()+"', description='"+link.getDescription()+"' WHERE url='"+link.getUrl()+"'");
      }
    }
    finally {
      conn.close();
    }
  }

  public static void delete(LinkBean[] links) throws Exception {
    // validate input
    if (links == null || links.length == 0) return;
    InitialContext ctx = new InitialContext();
    DataSource ds =
      (DataSource)ctx.lookup("java:comp/env/jdbc/jspbook");
    Connection conn = ds.getConnection();
    Statement statement  = conn.createStatement();
    LinkedList ll = new LinkedList();
    try {
      for (int i=0;i<links.length;i++) {
        LinkBean link = links[i];
        if (link.getUrl() == null) continue;
        statement.executeQuery("DELETE FROM LINK WHERE url='"+link.getUrl()+"'");
      }
    }
    finally {
      conn.close();
    }
  }
}
