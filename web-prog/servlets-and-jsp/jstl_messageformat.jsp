<%@ taglib uri="http://java.sun.com/jstl/fmt_rt" prefix="fmt"%>
<fmt:bundle basename="messageformat">
<html>
  <head>
    <title></title>
  </head>
  <body>
  <%
    java.util.Date date = new java.util.Date();
    request.setAttribute("date", date);
  %>
  <fmt:message key="message">
    <fmt:param value="foo"/>
    <fmt:param value="${date}"/>
    <fmt:param value="${10}"/>
  </fmt:message>
  </body>
</html>
</fmt:bundle>
