<%@ page import="java.util.*, com.jspbook.*"%>
<%
  Locale locale = request.getLocale();
  ResourceBundle  rb = ResourceBundle.getBundle("welcome", locale);
  Welcome content = new Welcome();
  content.setTitle(rb.getString("title"));
  content.setWelcome(rb.getString("welcome"));
  request.setAttribute("content", content);
%>
<%@ taglib uri="http://java.sun.com/jstl/core" prefix="c" %>
<html>
  <head>
    <title>${content.title}</title>
  </head>
  <body>
  ${content.welcome}
  </body>
</html>
