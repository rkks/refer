<%@ page import="java.util.*, java.text.*"%>
<%
 ResourceBundle rb = ResourceBundle.getBundle("i18n_static");
 MessageFormat mf =
   new MessageFormat("{0,number,currency}");
 Double doub = new Double(rb.getString("price"));
 String price = mf.format(new Object[]{doub});
 request.setAttribute("price", price);
 Locale locale = request.getLocale();
 DateFormat df = DateFormat.getInstance();
 Date d = df.parse(rb.getString("date"));
 String date = DateFormat.getInstance().format(d);
 request.setAttribute("date", date);
%>
<%@ taglib uri="http://java.sun.com/jstl/core_rt" prefix="c" %>
<html>
  <head>
    <title><%= date %> - Today's Story</title>
  </head>
  <body>
  On <c:out value="${date}"/> the U.S. stock market reached an
  all time low with FooTech(FOO) stock reaching 
  <c:out value="${price}"/> per a share.
  </body>
</html>
