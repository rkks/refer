<%@ page import="java.io.*,java.sql.*, javax.sql.*, javax.naming.*, java.util.*" %>
<%
  InitialContext ctx = new InitialContext();
  DataSource ds =
    (DataSource)ctx.lookup("java:comp/env/jdbc/jspbook");
  Connection conn = ds.getConnection();
  LinkedList links = new LinkedList();
  request.setAttribute("links", links);
  try {
    Statement statement  = conn.createStatement();
    String uri = request.getParameter("uri");
    if (uri != null && !uri.equals("")) {
      ResultSet rs = statement.executeQuery("SELECT l.url, l.title, l.description FROM LINK l, URI u WHERE l.url = u.url AND u.uri = '"+uri+"'");
      while (rs.next()) { // show results
        Hashtable map = new Hashtable();
        map.put("url", rs.getString(1));
        map.put("title", rs.getString(2));
        map.put("description", rs.getString(3));
        links.add(map);
      }
    }
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
  <%@ taglib uri="http://java.sun.com/jstl/core_rt" prefix="c" %>
  <c:forEach var="link" begin="0" items="${links}">
    <h3 style="margin-bottom:2px;">
      <a href="${link.url}">${link.title}</a>
    </h3>
    ${link.description}"
  </c:forEach>
  </body>
</html>
