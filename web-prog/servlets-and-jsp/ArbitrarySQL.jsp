<%@ page import="java.io.*,java.sql.*" %>
<%
  // keep code portable
  File tempDir = (File)application.getAttribute("javax.servlet.context.tempdir");
  String dbDir = tempDir.getAbsolutePath();
  String url = "jdbc:hsqldb:" + dbDir+"/jspbook";
  String user        = "sa"; // hsqldb default
  String password    = ""; // hsqldb default
  Statement sStatement  = null;
  Connection cConnection = null;
  // load JDBC driver - BAD! Use DataSource.
  Class.forName("org.hsqldb.jdbcDriver");

  cConnection = DriverManager.getConnection(url, user, password);
  try {
    sStatement  = cConnection.createStatement();

%>
<html>
  <head>
    <title>JSP Book Sample Database Creation</title>
  </head>
  <body>
<% // poor form, Model 1
  String query = request.getParameter("query");
  if (query != null && !query.equals("")) {
    out.println("<h2>Results of query:</h2>");
    out.println(query);
    ResultSet rs = sStatement.executeQuery(query);
    ResultSetMetaData rsmd = rs.getMetaData();
    out.print("<table border=\"1\"><tr>");
    for (int i=0; i<rsmd.getColumnCount();i++) {
      out.println("<td>"+rsmd.getColumnName(i+1)+"</td>");
    }
    out.print("</tr>");
    while (rs.next()) { // show results
      out.println("<tr>");
      for (int i=0; i<rsmd.getColumnCount();i++) {
        out.println("<td>"+rs.getString(i+1)+"</td>");
      }
      out.println("</tr>");
    }
    out.print("</table>");
    cConnection.close();
  }
%>
    <h2>New Query</h2>
    <form method="POST">
      Query: <textarea name="query" cols="65"></textarea><br>
      <input type="submit">
    </form>
<% 
  }
  catch (Exception e) {
    out.println("<h2>Problem with Query</h2>");
    out.print(e.getMessage());
  }
  finally {
    cConnection.close();
  }
%>
  </body>
</html>
