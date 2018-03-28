<%@ page import="java.util.*"%>
<html>
  <head>
    <title>Request's HTTP Headers</title>
  </head>
  <body>
    <p>HTTP headers sent by your client:</p>
<%
   Enumeration enum = request.getHeaderNames();
   while (enum.hasMoreElements()) {
     String headerName = (String) enum.nextElement();
     String headerValue = request.getHeader(headerName);
%>
    <b><%= headerName %></b>: <%= headerValue %><br>
<% } %>
  </body>
</html>
