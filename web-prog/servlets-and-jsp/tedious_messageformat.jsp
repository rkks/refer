<%@ page import="java.util.*"%>
<%
  Locale locale = request.getLocale();
  ResourceBundle  rb =
    ResourceBundle.getBundle("tedious_messageformat", locale);
  request.setAttribute("message1", rb.getString("message1"));
  request.setAttribute("message2", rb.getString("message2"));
  request.setAttribute("message3", rb.getString("message3"));
  request.setAttribute("message4", rb.getString("message4"));

  String name = "foo";
  Date date = new Date();
  Integer integer = new Integer(10);
  request.setAttribute("name", name);
  request.setAttribute("date", date);
  request.setAttribute("integer", integer);
%>
<%@ taglib uri="http://java.sun.com/jstl/core" prefix="c" %>
<html>
  <head>
    <title>Message Format Example</title>
  </head>
  <body>
  ${message1}${name}${message2}${date}
  ${message3}${integer}${message4}
  </body>
</html>
