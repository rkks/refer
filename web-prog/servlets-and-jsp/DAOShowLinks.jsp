<%@ page import="com.jspbook.*" %>
<%
  String uri = request.getParameter("uri");
  LinkBean[] links = LinkFactory.read(uri);
  request.setAttribute("links", links);
%>
<html>
  <head>
    <title>JSP Book Sample Database Creation</title>
  </head>
  <body>
  <%@ taglib uri="http://java.sun.com/jstl/core_rt" prefix="c" %>
  <c:forEach var="link" begin="0" items="${links}">
    <h3 style="margin-bottom:0px;">
      <a href="${link.url}">${link.title}</a>
    </h3>
    ${link.description}
  </c:forEach>
  </body>
</html>
