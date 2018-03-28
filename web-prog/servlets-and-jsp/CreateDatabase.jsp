<%@ page import="java.io.*,java.sql.*" %>
<%
  // keep code portable
  File tempDir = (File)application.getAttribute("javax.servlet.context.tempdir");
  String dbDir = tempDir.getAbsolutePath();
  String url = "jdbc:hsqldb:" + dbDir+"/jspbook";
  String user        = "sa"; // hsqldb default
  String password    = ""; // hsqldb default
  // load JDBC driver - BAD! Use DataSource.
  Class.forName("org.hsqldb.jdbcDriver");
  Connection conn = DriverManager.getConnection(url, user, password);
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
