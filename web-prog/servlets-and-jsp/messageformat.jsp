<%@ page import="java.text.*, java.util.*"%>
<%
  Locale locale = request.getLocale();
  ResourceBundle rb =
    ResourceBundle.getBundle("messageformat", locale);
  Object[] objects = {"foo", new Date(), new Integer(10) };
  MessageFormat mf =
    new MessageFormat(rb.getString("message"), locale);
  String result = mf.format(objects);
  request.setAttribute("result", result);
%>
<%@ taglib uri="http://java.sun.com/jstl/core" prefix="c" %>
<html>
  <head>
    <title>Message Format Example</title>
  </head>
  <body>
  ${result}
  </body>
</html>
