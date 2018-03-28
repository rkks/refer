<%@ page import="java.io.*,java.sql.*, javax.sql.*, javax.naming.*" %>
<%
  InitialContext ctx = new InitialContext();
  DataSource ds =
    (DataSource)ctx.lookup("java:comp/env/jdbc/jspbook");
  Connection conn = ds.getConnection();
  try {
    Statement statement  = conn.createStatement();
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
    ResultSet rs = statement.executeQuery(query);
    ResultSetMetaData rsmd = rs.getMetaData();
    out.print("<table border=\"1\"><tr>");
    for (int i=0; i<rsmd.getColumnCount();i++) {
      out.println("<td>"+rsmd.getColumnName(i+1)+"</td>");
    }
    out.print("</tr>");
    while (rs.next()) { // show results
      out.print("<tr>");
      for (int i=0; i<rsmd.getColumnCount();i++) {
        out.println("<td>"+rs.getString(i+1)+"</td>");
      }
      out.print("</tr>");
    }
    out.print("</td></table>");
    //conn.close();
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
    conn.close();
  }
%>
  </body>
</html>
