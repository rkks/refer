<html>
  <head>
    <title>useBean Example</title>
  </head>
  <body>
  <jsp:useBean id="user" class="com.jspbook.User" />
  <%
  user.setName("Bob");
  user.setPassword("password");
  %>
  Hello <jsp:getProperty name="user" property="name"/>, welcome to this web page!
  </body>
</html>
