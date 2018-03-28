<%@ page import="java.io.*,java.sql.*,javax.sql.*,javax.naming.InitialContext" %>
<%
  InitialContext ctx = new InitialContext();
  DataSource ds = (DataSource)ctx.lookup("java:comp/env/jdbc/jspbook");
  Connection conn = ds.getConnection();
  try {
    Statement statement  = conn.createStatement();
%>
<html>
  <head>
    <title>Show LINK and URI Tables</title>
  </head>
  <body>
  <h2>Contents of LINK Table</h2>
  <table border="1">
  <tr>
    <td>URL</td>
    <td>Title</td>
    <td>Description</td>
  </tr>
<%
  ResultSet rs = statement.executeQuery("SELECT * FROM LINK");
  while (rs.next()) { // show results
    out.println("<tr>");
    out.println("<td>"+rs.getString("url")+"</td>");
    out.println("<td>"+rs.getString("title")+"</td>");
    out.println("<td>"+rs.getString("description")+"</td>");
    out.println("</tr>");
  }
%>
  </table>
  <h2>Contents of URI Table</h2>
  <table border="1">
  <tr>
    <td>URI</td>
    <td>URL</td>
  </tr>
<%
  rs = statement.executeQuery("SELECT * FROM URI");
  while (rs.next()) { // show results
    out.println("<tr>");
    out.println("<td>"+rs.getString("uri")+"</td>");
    out.println("<td>"+rs.getString("url")+"</td>");
    out.println("</tr>");
  }
%>
  </table>
<% 
  }
  catch (Exception e) {
    out.println("<h2>Problem with Query</h2>");
    out.print(e.getMessage());
  }
  finally {
    conn.close();
  }
%>
  </body>
</html>
