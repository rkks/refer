<%@ page import="java.io.*,java.sql.*, javax.sql.*, javax.naming.*, java.util.*, java.net.*" %>
<%
  InitialContext ctx = new InitialContext();
  DataSource ds =
    (DataSource)ctx.lookup("java:comp/env/jdbc/jspbook");
  Connection conn = ds.getConnection();
  LinkedList deleted = new LinkedList();
  request.setAttribute("deleted", deleted);
  try {
    Statement statement  = conn.createStatement();
    ResultSet rs = statement.executeQuery("SELECT * FROM LINK");
    while (rs.next()) { // show results
      String url = rs.getString("url");
      String title = rs.getString("title");
      String description = rs.getString("description");
      boolean failed = false;
      try {
        URL u = new URL(url);
        HttpURLConnection huc = (HttpURLConnection)u.openConnection();
        int responseCode = huc.getResponseCode();
        if (responseCode >= 400 && responseCode < 500) {
          failed = true;
        }
        huc.disconnect();
      }
      catch (Exception e) {
        failed = true;
      }
      if (failed) {
        Properties link = new Properties();
        link.setProperty("url", url);
        link.setProperty("title", title);
        link.setProperty("description", description);
        deleted.add(link);
        statement.execute("DELETE FROM LINK WHERE url = '"+url+"'");
      }
    }
  }
  finally {
    conn.close();
  }
%>
<html>
  <head>
    <title>Link Checker</title>
  </head>
  <body>
  <h2>Links Deleted</h2>
  <%@ taglib uri="http://java.sun.com/jstl/core_rt" prefix="c" %>
  <c:forEach var="link" begin="0" items="${deleted}">
    <hr>
    <h3>
      <a href="${link.url}">${link.title}</a>
    </h3>
    ${link.description}
  </c:forEach>
  </body>
</html>
