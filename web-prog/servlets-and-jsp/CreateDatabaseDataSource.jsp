<%@ page import="java.io.*,java.sql.*, javax.naming.*, javax.sql.*" %>
<%
  InitialContext ctx = new InitialContext();
  DataSource ds =
    (DataSource)ctx.lookup("java:comp/env/jdbc/jspbook");
  Connection conn = ds.getConnection();
  try {
    Statement statement = conn.createStatement();
    String link = ""; // create LINK table
    link += "CREATE TABLE LINK(";
    link += "  URL VARCHAR(128) PRIMARY KEY,";
    link += "  TITLE VARCHAR(128),";
    link += "  DESCRIPTION VARCHAR(256)";
    link += ")";
    statement.executeQuery(link);
    String uri = ""; // create URI table
    uri += "CREATE TABLE URI(";
    uri += "  URI VARCHAR(40),";
    uri += "  URL VARCHAR(128),";
    uri += "  PRIMARY KEY(URI, URL)";
    uri += ")";
    statement.executeQuery(uri);
  }
  finally {
    conn.close();
  }
%>
<html>
  <head>
    <title>JSP Book Sample Database Creation</title>
  </head>
  <body>
  The Database has been successfully created.
  </body>
</html>
